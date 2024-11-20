
#include "minirt.h"

static inline bool	within_height2(t_ray ray, double min, double max, double t)
{
	double y;
	
	if (t < 1e-5)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	if (y > min && y < max)
		return (true);
	return (false);
}

static inline bool	check_trunk_solutions2_hy(t_vec3 abc, t_ray ray, double dist, double half_h)
{
	double	discrim;
	double	inv_2a;
	double 	sq_discrim;
	double	t;

	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-5)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	t = (-abc.y - sq_discrim) * inv_2a;
	if (within_height2(ray, -half_h, half_h, t) && t < dist)
					return (true);
	t = (-abc.y + sq_discrim) * inv_2a;
	if (within_height2(ray, -half_h, half_h, t) && t < dist)
					return (true);
	return (false);
}


bool	ray_hyperboloid_intersect2(t_hyperboloid hyperboloid, t_ray ray, double dist)
{
	t_vec3	abc;
	double	half_h;

	half_h = hyperboloid.half_h;
	ray = transform(ray, hyperboloid.transform);
	compute_abc_hy(&abc, ray, &hyperboloid);
	if (abc.x != 0)
	{
		if (check_trunk_solutions2_hy(abc, ray, dist, half_h))
			return (true);
	}
	return (false);
}
