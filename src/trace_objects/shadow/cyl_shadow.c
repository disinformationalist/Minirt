#include "minirt.h"

static inline bool	within_height2(t_ray ray, double t)
{
	double	y;

	if (t < 1e-6)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	if (y > -1 && y < 1)
		return (true);
	return (false);
}

static inline bool	check_trunk_solutions2(t_vec3 abc, t_ray ray, double dist)
{
	double	discrim;
	double	inv_2a;
	double	sq_discrim;
	double	t;

	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-6)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	t = (-abc.y - sq_discrim) * inv_2a;
	if (within_height2(ray, t) && t < dist)
		return (true);
	t = (-abc.y + sq_discrim) * inv_2a;
	if (within_height2(ray, t) && t < dist)
		return (true);
	return (false);
}

static inline bool	check_cap2(t_ray ray, double t)
{
	double	x;
	double	z;

	x = ray.origin.x + t * ray.dir.x;
	z = ray.origin.z + t * ray.dir.z;
	if ((x * x + z * z) <= 1)
		return (true);
	return (false);
}

static inline bool	intersect_caps2(t_ray ray, double dist)
{
	double	t;
	bool	hit;

	if (fabs(ray.dir.y) < 1e-6)
		return (false);
	t = (-1 - ray.origin.y) / ray.dir.y;
	hit = check_cap2(ray, t);
	if (hit && t > 0 && t < dist)
		return (true);
	t = (1 - ray.origin.y) / ray.dir.y;
	hit = check_cap2(ray, t);
	if (hit && t > 0 && t < dist)
		return (true);
	return (false);
}

bool	ray_cylinder_intersect2(t_cylinder cylinder, t_ray ray, double dist)
{
	t_vec3	abc;

	ray = transform(ray, cylinder.transform);
	abc.x = ray.dir.x * ray.dir.x + ray.dir.z * ray.dir.z;
	abc.y = 2 * (ray.origin.x * ray.dir.x + ray.origin.z * ray.dir.z);
	abc.z = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - 1;
	if (fabs(abc.x) < 1e-6)
		return (false);
	if (check_trunk_solutions2(abc, ray, dist))
		return (true);
	if (cylinder.caps && intersect_caps2(ray, dist))
		return (true);
	return (false);
}
