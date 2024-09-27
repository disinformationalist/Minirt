#include "minirt.h"

//og world space method

/* static inline bool check_solutions(double a, double b, double c, double *t1, double *t2)//change  store vals
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
	sol_2 = (-b + sqr_discrim) * inv_2a;
	if (sol_2 > 0)
		*t2 = sol_2;
	sol_1 = (-b - sqr_discrim) * inv_2a;
	if (sol_1 > 0)
		*t1 = sol_1;
	if (sol_1 > 0 || sol_2 > 0)
		return (true);
	return (false);
} */

static inline bool check_solutions(double a, double b, double c, double *t1, double *t2)//change  store vals
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
	sol_2 = (-b + sqr_discrim) * inv_2a;
	*t2 = sol_2;
	sol_1 = (-b - sqr_discrim) * inv_2a;
	*t1 = sol_1;
	if (*t1 < INFINITY || *t2 < INFINITY)
		return (true);
	return (false);
}

/*t_ray	transform(t_ray r, t_matrix_4x4 m)
{
	t_ray new;

	new.dir = mat_vec_mult(m, r.dir);
	new.origin = mat_vec_mult(m, r.origin);
	return (new);
}*/
bool	ray_lens_sphere_intersect(t_sphere sphere, t_ray ray, double *t1, double *t2)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;

	ray = transform(ray, sphere.transform);
	oc = ray.origin;
	a = dot_product(ray.dir, ray.dir);
	b = 2.0 * dot_product(oc, ray.dir);
	c = dot_product(oc, oc) - 1;
	if (check_solutions(a, b, c, t1, t2))
		return (true);
	return (false);
}

bool	lens_hit(double t1_s1, double t2_s1, double t1_s2, double t2_s2)
{
	if ((t1_s1 < t2_s1) && (t1_s1 < t1_s2) && (t2_s1 > t1_s2) && (t1_s2 < t2_s2))
		return (true);
	if ((t1_s1 < t2_s1) && (t1_s1 > t1_s2) && (t1_s1 < t2_s2) && (t1_s2 < t2_s2))
		return (true);
	return (false);
}

bool	ray_lens_intersect(t_lens lens, t_ray ray, double *t)
{
	double	t1_s1;
	double	t2_s1;
	double	t1_s2;
	double	t2_s2;
	double	t_temp;

	t1_s1 = INFINITY;
	t2_s1 = INFINITY;
	t1_s2 = INFINITY;
	t2_s2 = INFINITY;
	if (ray_lens_sphere_intersect(lens.sphere_1, ray, &t1_s1, &t2_s1)
		&& ray_lens_sphere_intersect(lens.sphere_2, ray, &t1_s2, &t2_s2)
		&& (t1_s1 != INFINITY && t2_s1 != INFINITY && t1_s2 != INFINITY && t2_s2 != INFINITY))
	{
			if (lens_hit(t1_s1, t2_s1, t1_s2, t2_s2))
			{
				*t = fmin(fmax(t1_s1, t2_s1), fmin(t1_s2, t2_s2));
				t_temp = fmin(fmin(t1_s1, t2_s1), fmax(t1_s2, t2_s2));
				if (t_temp < *t)
				*t = t_temp;
				return (true);
			}
	}
	return (false);
}

//using object space

void	check_lenses(t_lens *lenses, t_track_hits *closest, t_ray ray, double *t)
{
	t_lens		*curr_le;

	if (lenses == NULL)
		return ;
	curr_le = lenses;
	while (true)
	{
		if (ray_lens_intersect(*curr_le, ray, t))
		{
			if (*t < closest->t && *t > 0)
			{
				closest->t = *t;
				closest->object = curr_le;
				closest->object_type = LENS;
			}
		}
		curr_le = curr_le->next;
		if (curr_le == lenses)
			break;
	}
}

//diff plus specular for sp

/*double	get_light_int(t_vec3 norm, t_vec3 light_dir, t_vec3 view_dir)//, t_mat sphere->mat)
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

}*/

//transform testing

t_vec3	le_sp_normal_at(t_point int_pnt, t_matrix_4x4 transform)
{
	t_vec3	norm;
	t_point obj_pnt;

	obj_pnt = mat_vec_mult(transform, int_pnt); //get the normal using the transform
	norm = mat_vec_mult(transpose(transform), obj_pnt);
	norm.w = 0;
	return (norm_vec(norm));
}

//this function will check each sphere passed to it

/*unsigned int color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)
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
		if (dot_product(norm, r.dir) > 0)//maybe using transform for view takes care of this?
			norm = neg(norm);
		//loop here for multiple lights. sum total lights * lt_colors *intensity, return a total color due to colored light
		light_dir = norm_vec(subtract_vec(trace->lights->center, int_pnt));
		if (!obscured(trace, int_pnt, light_dir, norm))
			light_int = trace->lights->brightness * get_light_int(norm, light_dir, neg(r.dir));//diff + spec here for each light
	}
	//sphere->color = stripe_at(int_pnt, sphere->transform);//trying color function
	return (get_final_color(trace, sphere->color, light_int));
}*/

unsigned int color_lens(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_lens			*lens;
	t_sphere		*sphere_1;
	//t_sphere		*sphere_2;
	t_vec3			norm_1;
	//t_vec3			norm_2;
	t_point			int_pnt;
	t_vec3			light_dir; 
	double			light_int;

	lens = (t_lens *)closest->object;
	sphere_1 = &lens->sphere_1;
	//sphere_2 = &lens->sphere_2;
	light_int = 0;
	if (trace->lights)
	{
		int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		norm_1 = le_sp_normal_at(int_pnt, sphere_1->transform);
		//norm_2 = le_sp_normal_at(int_pnt, sphere_2->transform);
		if (dot_product(norm_1, r.dir) > 0)//maybe using transform for view takes care of this?
			norm_1 = neg(norm_1);
		//if (dot_product(norm_2, r.dir) > 0)//maybe using transform for view takes care of this?
		//	norm_1 = neg(norm_2);
		//loop here for multiple lights. sum total lights * lt_colors *intensity, return a total color due to colored light
		light_dir = norm_vec(subtract_vec(trace->lights->center, int_pnt));
		if (!obscured(trace, int_pnt, light_dir, norm_1))
			light_int = trace->lights->brightness * get_light_int(norm_1, light_dir, neg(r.dir));//diff + spec here for each light
		//if (!obscured(trace, int_pnt, light_dir, norm_2))
		//	light_int = trace->lights->brightness * get_light_int(norm_2, light_dir, neg(r.dir));//diff + spec here for each light
	}
	//sphere->color = stripe_at(int_pnt, sphere->transform);//trying color function
	return (get_final_color(trace, sphere_1->color, light_int)); //, get_final_color(trace, sphere_2->color, light_int));
}