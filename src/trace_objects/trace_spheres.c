#include "minirt.h"

//this function will check each sphere passed to it

static inline bool check_solutions(double a, double b, double c, double *t)
{
	double	discrim;
	double	sol_1;
	double	sol_2;
	double	sqr_discrim;
	double	inv_2a;

	discrim = b * b - 4 * a * c;
	if (discrim < 0)
		return (false);
	sqr_discrim = sqrt(discrim);
	inv_2a = 0.5 / a;
	sol_1 = (-b - sqr_discrim) * inv_2a;
	if (sol_1 > 0)
	{
		*t = sol_1;
		return (true);
	}
	sol_2 = (-b + sqr_discrim) * inv_2a;
	if (sol_2 > 0)
	{
		*t = sol_2;
		return (true);
	}
	return (false);
}

//og world space method

/* bool	ray_sphere_intersect(t_sphere sphere, t_vec3 ray_dir, t_vec3 ray_orig, double *t)
{
	t_vec3	oc;//ray origin to center
	double	a;//coefficients of quadratic
	double	b;
	double	c;

	oc = subtract_vec(ray_orig, sphere.center);
	a = dot_product(ray_dir, ray_dir);
	b = 2.0 * dot_product(oc, ray_dir);
	c = dot_product(oc, oc) - sphere.radius * sphere.radius;
	if (check_solutions(a, b, c, t))
		return (true);
	return (false);
} */

//using object space
bool	ray_sphere_intersect(t_sphere sphere, t_vec3 ray_dir, t_vec3 ray_orig, double *t)
{
	t_vec3	oc;//ray origin to center
	double	a;//coefficients of quadratic
	double	b;
	double	c;

	//apply transform to ray
	ray_orig = subtract_vec(ray_orig, sphere.center);//translation //1st, order matters
	ray_orig = scale_vec(1.0 / sphere.radius, ray_orig);//scale
	ray_dir = scale_vec(1.0 / sphere.radius, ray_dir);//scale

	oc = ray_orig;//trying with sphere at center then translate to center
	a = dot_product(ray_dir, ray_dir);
	b = 2.0 * dot_product(oc, ray_dir);
	c = dot_product(oc, oc) - 1;
	if (check_solutions(a, b, c, t))
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
		if (ray_sphere_intersect(*curr_sp, ray.dir, ray.origin, t))
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

//playing with spotlight adjustment here
//check the condition first to see if there is any need for the compute of diff/spec to opti.
double spotlight(t_vec3 light_dir)
{
	t_vec3	ln;//spot light pointing direction
	double	cos_theta;
	double	inner_cone = cos(M_PI / 12);
	double	outer_cone = cos(M_PI / 8);

	ln = norm_vec(vec(0, 1, 0));//invert orientation 
	cos_theta = dot_product(ln, light_dir);
	if (cos_theta > inner_cone)
		return (1.0);
	else if (cos_theta > outer_cone)
	{
		return ((cos_theta - outer_cone) / (inner_cone - outer_cone));
	}
	else
		return (0);
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
	return (light_int);//trying sp_light

}

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

//transform testing

unsigned int color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_sphere		*sphere;
	t_point			int_pnt;//light intersect with surface
	t_vec3			norm;
	t_vec3			light_dir; 
	double			light_int;//total non amb lt intensity

	sphere = (t_sphere *)closest->object;//pass in at check intersects, do for each shape ...

	//r.dir = subtract_vec(r.dir, sphere->center);//testing transform...
	//r.origin = subtract_vec(r.origin, sphere->center);

	
	light_int = 0;//use this simplification on all
	if (trace->lights)// getting the light intensity at each intersection point
	{
		int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		//can just use norm_vec(int point), and then the inverse of the transform....normal_at(int_pnt)
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
}

//loop version through all lights, seems to be working... add checks for type, early exit opti in spotlights, color handling..

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