#include "minirt.h"

//og world space method

/* bool	ray_sphere_intersect(t_sphere sphere, t_ray ray, double *t)
{
	t_vec3	oc;//ray origin to center
	double	a;//coefficients of quadratic
	double	b;
	double	c;

	oc = subtract_vec(ray.origin, sphere.center);
	a = dot_product(ray.dir, ray.dir);
	b = 2.0 * dot_product(oc, ray.dir);
	c = dot_product(oc, oc) - sphere.radius * sphere.radius;
	if (check_solutions(a, b, c, t))
		return (true);
	return (false);
} */

static inline bool check_solutions(t_vec3 abc, double *t1, double *t2)//change  store vals
{
	double	discrim;
	double	inv_2a;
	double 	sq_discrim;
	
	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-5)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	*t1 = (-abc.y - sq_discrim) * inv_2a;
	*t2 = (-abc.y + sq_discrim) * inv_2a;
	return (true);
}

//using object space

bool	ray_sphere_intersect(t_sphere sphere, t_ray ray, double *t)
{
	double	t1;
	double	t2;
	double	a;
	double	b;
	double	c;

	ray = transform(ray, sphere.transform);
	a = dot_product(ray.dir, ray.dir);
	b = 2.0 * dot_product(ray.origin, ray.dir);
	c = dot_product(ray.origin, ray.origin) - 1;
	if (check_solutions(vec(a, b, c, 0), &t1, &t2))
	{
		if (t1 > 0 && t1 < *t)
			*t = t1;
		if (t2 > 0 && t2 < *t)
			*t = t2;
	}
	if (*t < INFINITY)
		return (true);
	return (false);
}

void	check_spheres(t_sphere *spheres, t_track_hits *closest, t_ray ray, double *t)
{
	t_sphere	*curr_sp;

	if (spheres == NULL)
		return ;
	curr_sp = spheres;
	while (true)
	{
		if (ray_sphere_intersect(*curr_sp, ray, t))
		{
			if (*t < closest->t && *t > 0)
			{
				closest->t = *t;
				closest->object = curr_sp;
				closest->object_type = SPHERE;
			}
		}
		curr_sp = curr_sp->next;
		if (curr_sp == spheres)
			break;
	}
}

//diff plus specular for sp

double	get_light_int(t_vec3 norm, t_vec3 light_dir, t_vec3 view_dir)//, t_mat sphere->mat)
{
	t_vec3	ref;
	double	spec;
	double	light_int;
	double	cos_angle;

	cos_angle = dot_product(norm, light_dir);
	ref = subtract_vec(scale_vec(2 * cos_angle, norm), light_dir);
	spec = pow(fmax(dot_product(ref, view_dir), 0), 200);
	//spec = pow(fmax(dot_product(ref, view_dir), 0), mat.shine); //use these when mat is assigned
	light_int = fmax(cos_angle, 0.0) + .5 * spec;
	//light_int = mat.diff * fmax(cos_angle, 0.0) + mat.spec * spec;
	//return (spotlight(light_dir) * light_int);//trying sp_light
	return (light_int);

}

t_vec3	sp_normal_at(t_point int_pnt, t_matrix_4x4 transform)
{
	t_vec3	norm;
	t_point obj_pnt;

	obj_pnt = mat_vec_mult(transform, int_pnt);
	norm = mat_vec_mult(transpose(transform), obj_pnt);
	norm.w = 0;
	return (norm_vec(norm));
}

//this function will check each sphere passed to it

unsigned int color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_sphere		*sphere;
	t_vec3			norm;
	t_point			int_pnt;
	t_vec3			light_dir; 
	double			light_int;

	sphere = (t_sphere *)closest->object;
	light_int = 0;
	if (trace->lights)
	{
		int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		norm = sp_normal_at(int_pnt, sphere->transform);
		if (dot_product(norm, r.dir) > 0)
			norm = neg(norm);
		//loop here for multiple lights. sum total lights * lt_colors *intensity, return a total color due to colored light
		light_dir = norm_vec(subtract_vec(trace->lights->center, int_pnt));
		if (!obscured(trace, int_pnt, light_dir, norm))
			light_int = trace->lights->brightness * get_light_int(norm, light_dir, neg(r.dir));//diff + spec here for each light
	}
	//sphere->color = stripe(int_pnt);//trying color function
	
	//sphere->color = stripe_at(int_pnt, sphere->transform);//trying color function
	return (get_final_color(trace, sphere->color, light_int));
}

//loop version through all lights, seems to be working... add checks for type, early exit opti in spotlights, color handling..
//needs some changes now, using transforms
/* unsigned int color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_sphere		*sphere;
	t_point			int_pnt;//light intersect with surface
	t_vec3			norm;
	t_vec3			light_dir; 
	double			light_int;//total non amb lt intensity

	t_light			*curr_lt;

	sphere = (t_sphere *)closest->object;

	light_int = 0;//use this simplification on all
	if (trace->lights)// getting the light intensity at each intersection point
	{
		int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		norm = norm_vec(subtract_vec(int_pnt, sphere->center));
		if (dot_product(norm, r.dir) > 0)
			norm = neg(norm);
		//loop here for multiple lights. sum total lights * lt_colors *intensity, return a total color due to colored light
		curr_lt = trace->lights;
		while (true)
		{
			light_dir = norm_vec(subtract_vec(curr_lt->center, int_pnt));
			//hard shadows...

			if (!obscured_b(trace, ray(light_dir, add_vec(int_pnt, scale_vec(1e-5, norm))), curr_lt->center, int_pnt))
				light_int += curr_lt->brightness * get_light_int(norm, light_dir, neg(r.dir));//diff + spec here for each light
			curr_lt = curr_lt->next;
			if (curr_lt == trace->lights)
				break;
		}	
	}
	//sphere->color = stripe(int_pnt);//trying color function
	return (get_final_color(trace, sphere->color, light_int));
} */



//cast shadow ray from intersect point toward light source
//s_ray.dir = norm(L - P) light cen - int point
//s.origin = P + e-6, prevent s_ray form intersecting surface of origination
//test intersects until light 
//light distance = mag(L -P)

//og

/* unsigned int color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_sphere		*sphere;
	t_point			int_pnt;//light intersect with surface
	t_vec3			norm;
	t_vec3			light_dir; 
	double			light_int;//total non amb lt intensity

	sphere = (t_sphere *)closest->object;//pass in at check intersects, do for each shape ...
	
	light_int = 0;//use this simplification on all
	if (trace->lights)// getting the light intensity at each intersection point
	{
		int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		norm = norm_vec(subtract_vec(int_pnt, sphere->center));
		if (dot_product(norm, r.dir) > 0)
			norm = neg(norm);
		//loop here for multiple lights. sum total lights * lt_colors *intensity, return a total color due to colored light
		light_dir = norm_vec(subtract_vec(trace->lights->center, int_pnt));
		//hard shadows...
		if (!obscured(trace, int_pnt, light_dir, norm))
			light_int = trace->lights->brightness * get_light_int(norm, light_dir, neg(r.dir));//diff + spec here for each light
			//trace->lights = trace->lights->next
	}
	//sphere->color = stripe(int_pnt);//trying color function
	return (get_final_color(trace, sphere->color, light_int));
} */