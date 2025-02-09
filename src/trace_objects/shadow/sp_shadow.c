#include "minirt.h"

static inline bool	check_solutions2(double a, double b, double c, double dist)
{
	double	discrim;
	double	inv_2a;
	double	sq_discrim;
	double	sol;

	discrim = b * b - 4 * a * c;
	if (discrim < 1e-5)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / a;
	sol = (-b - sq_discrim) * inv_2a;
	if (sol > 0 && sol < dist)
		return (true);
	sol = (-b + sq_discrim) * inv_2a;
	if (sol > 0 && sol < dist)
		return (true);
	return (false);
}

static inline bool	ray_sphere_intersect2(t_sphere sphere, \
t_ray ray, double dist)
{
	double	a;
	double	b;
	double	c;

	ray = transform(ray, sphere.transform);
	a = dot_product(ray.dir, ray.dir);
	b = 2.0 * dot_product(ray.origin, ray.dir);
	c = dot_product(ray.origin, ray.origin) - 1;
	if (check_solutions2(a, b, c, dist))
		return (true);
	return (false);
}

//check spheres for intersection within dist

bool	check_sp_dist(t_sphere *spheres, t_ray ray, double dist)
{
	t_sphere	*curr_sp;

	if (spheres == NULL)
		return (false);
	curr_sp = spheres;
	while (true)
	{
		if (curr_sp->shadow && ray_sphere_intersect2(*curr_sp, ray, dist))
			return (true);
		curr_sp = curr_sp->next;
		if (curr_sp == spheres)
			break ;
	}
	return (false);
}
