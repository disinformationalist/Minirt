#include "minirt.h"

static inline bool check_solutions(t_vec3 abc, double *t1, double *t2)
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

static inline double	get_splight_int(t_vec3 norm, t_vec3 light_dir, t_vec3 view_dir)
{
	t_vec3	ref;
	double	spec;
	double	light_int;
	double	cos_angle;

	cos_angle = dot_product(norm, light_dir);
	ref = subtract_vec(scale_vec(2 * cos_angle, norm), light_dir);
	spec = pow(fmax(dot_product(ref, view_dir), 0), 200);
	light_int = fmax(cos_angle, 0.0) + .5 * spec;
	return (light_int);
}

//compute normal at each intersection point using transform

static inline t_vec3	sp_normal_at(t_point int_pnt, t_matrix_4x4 transform)
{
	t_vec3	norm;
	t_point obj_pnt;

	obj_pnt = mat_vec_mult(transform, int_pnt);
	norm = mat_vec_mult(transpose(transform), obj_pnt);
	norm.w = 0;
	return (norm_vec(norm));
}

//this function will check each sphere passed to it

t_norm_color color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)
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
		light_dir = norm_vec(subtract_vec(trace->lights->center, int_pnt));
		if (!obscured(trace, int_pnt, light_dir, norm))
			light_int = trace->lights->brightness * get_splight_int(norm, light_dir, neg(r.dir));
	}
	return (get_final_color(trace, sphere->color, light_int));
}
