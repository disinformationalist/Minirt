#include "minirt.h"

//this function will check each sphere passed to it

static inline bool check_solutions(double a, double b, double c, double *t)
{
	double	discrim;
	double sol_1;
	double sol_2;
	double sqr_discrim;
	double inv_2a;

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

static inline bool	ray_sphere_intersect(t_sphere sphere, t_vec3 ray_dir, t_vec3 ray_orig, double *t)
{
	t_vec3 oc;//ray origin to center
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
}

void	check_spheres(t_sphere *spheres, t_track_hits *closest, t_ray ray, double *t)
{
	t_sphere		*curr_sp;

	if (spheres == NULL)
		return ;
	curr_sp = spheres;
	while (true)
	{
		if (ray_sphere_intersect(*curr_sp, ray.direction, ray.origin, t))
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

unsigned int color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_vec3			intersect_pnt;//light intersect with surface
	t_vec3			normal;
	t_vec3			light_dir; 
	double			light_intensity;
	double			cos_angle;
	t_sphere		*sphere;

	sphere = (t_sphere *)closest->object;

	if (trace->lights)
	{
		//plug closest->t back into ray eq for intersect point;
		intersect_pnt = add_vec(r.origin, scalar_mult_vec(closest->t, r.direction));
		normal = normalize_vec(subtract_vec(intersect_pnt, sphere->center));
		light_dir = normalize_vec(subtract_vec(trace->lights->center, intersect_pnt));
		cos_angle = dot_product(normal, light_dir);
		light_intensity	= trace->lights->brightness * fmax(cos_angle, 0.0);
	}
	else
		light_intensity = 0;
	return (get_final_color(trace, sphere->color, light_intensity));
	
}