#include "minirt.h"

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

double	get_light_int(t_comps comps, t_mat mat)
{
	t_vec3	ref;
	double	spec;
	double	light_int;

	ref = subtract_vec(scale_vec(2 * comps.cos_angle, comps.normal), comps.light_dir);
	spec = pow(fmax(dot_product(ref, comps.eyev), 0), mat.shine);
	light_int = mat.diff * fmax(comps.cos_angle, 0.0) + mat.spec * spec;
	//return (spotlight(comps.light_dir) * light_int);//trying sp_light
	return (light_int);
}


/* typedef struct t_comps
{
	double	t;
	//void	*object;
	//t_type	object_type;
	t_vec3	point;
	t_vec3	eyev;
	t_vec3	normal;
	t_vec3	reflectv;
	bool	inside;
}	t_comps; */
//this function will check each sphere passed to it


t_vec3	sp_normal_at(t_point int_pnt, t_matrix_4x4 transform)
{
	t_vec3	norm;
	t_point obj_pnt;

	obj_pnt = mat_vec_mult(transform, int_pnt);
	norm = mat_vec_mult(transpose(transform), obj_pnt);
	norm.w = 0;
	return (norm_vec(norm));
}

t_comps	set_spcomps(t_sphere *sphere, double t, t_ray r)
{
	t_comps	comps;
	
	comps.t = t;
	comps.point = add_vec(r.origin, scale_vec(t, r.dir));
	comps.normal = sp_normal_at(comps.point, sphere->transform);
	comps.eyev = neg(r.dir);
	if (dot_product(comps.normal, comps.eyev) < 0)
	{
		comps.inside = true;
		comps.normal = neg(comps.normal);
	}
	else
		comps.inside = false;
	return (comps);
}

t_norm_color color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)//working. now make spotlights...
{
	t_sphere		*sphere;
	t_comps			comps;
	t_norm_color	lt_color;
	t_norm_color	color1;
	t_light			*curr_lt;

	sphere = (t_sphere *)closest->object;
	lt_color = color(0, 0, 0);
	if (trace->lights)
	{
		comps = set_spcomps(sphere, closest->t, r);
		//loop here for multiple lights. sum total lights * lt_colors *intensity, return a total color due to colored light// SEP FUNCITON?
		curr_lt = trace->lights;
		while (true)
		{
			comps.light_dir = norm_vec(subtract_vec(curr_lt->center, comps.point));
			comps.cos_angle = dot_product(comps.normal, comps.light_dir);
			comps.reflectv = subtract_vec(scale_vec(2 * comps.cos_angle, comps.normal), comps.light_dir);
			/* if (!obscured(trace, comps.point, comps.light_dir, comps.normal))
				light_int = trace->lights->brightness * get_splight_int(comps, sphere->mat); */
			if (!obscured_b(trace, ray(comps.light_dir, add_vec(comps.point, scale_vec(1e-5, comps.normal))), curr_lt->center, comps.point))
				lt_color = sum_rgbs(lt_color, mult_color(curr_lt->brightness * get_light_int(comps, sphere->mat), curr_lt->color));
			curr_lt = curr_lt->next;
			if (curr_lt == trace->lights)
				break;
		}	

	//sphere->color = stripe(int_pnt);//trying color function
	//sphere->color = stripe_at(int_pnt, sphere->transform);//trying color function
	//color1 = checker_at(int_pnt, sphere->transform);
	//color1 = gradient_at(int_pnt, sphere->transform, color(0, 255, 0), color(0, 0, 255));

	}
	color1 = sphere->color;
	return (get_final_color1(trace, color1, lt_color));//send in material here? mult mat.amb * ambcomponent....
}

//old
/* t_norm_color color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_sphere		*sphere;
	t_comps			comps;
	t_vec3			light_dir; 
	double			light_int;
	t_norm_color	color1;

	sphere = (t_sphere *)closest->object;
	light_int = 0;
	if (trace->lights)
	{
		comps = set_spcomps(sphere, closest->t, r);
		//int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		//norm = sp_normal_at(int_pnt, sphere->transform);
		//if (dot_product(norm, r.dir) > 0)
		//	norm = neg(norm);
		//loop here for multiple lights. sum total lights * lt_colors *intensity, return a total color due to colored light
		light_dir = norm_vec(subtract_vec(trace->lights->center, comps.point));
		if (!obscured(trace, comps.point, light_dir, comps.normal))
			light_int = trace->lights->brightness * get_splight_int(comps.normal, light_dir, comps.eyev, sphere->mat);



	//sphere->color = stripe(int_pnt);//trying color function
	//sphere->color = stripe_at(int_pnt, sphere->transform);//trying color function
	//color1 = checker_at(int_pnt, sphere->transform);
	//color1 = gradient_at(int_pnt, sphere->transform, color(0, 255, 0), color(0, 0, 255));

	}
	color1 = sphere->color;
	return (get_final_color(trace, color1, light_int));
} */