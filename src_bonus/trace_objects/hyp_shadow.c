#include "minirt.h"

static inline bool	within_height2(t_ray ray, double t)
{
	double	y;

	if (t < 1e-5)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	if (y > -1 && y < 1)
		return (true);
	return (false);
}

static inline bool	check_trunk_solutions2_hy(t_vec3 abc, \
t_ray ray, double dist)
{
	double	discrim;
	double	inv_2a;
	double	sq_discrim;
	double	t;

	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-5)
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

static inline bool	check_cap2_hy(t_ray ray, double t, t_hyperboloid hyperboloid)
{
	double	x;
	double	z;

	x = ray.origin.x + t * ray.dir.x;
	z = ray.origin.z + t * ray.dir.z;
	if ((x * x + z * z) <= 1 + hyperboloid.waist_val)
		return (true);
	return (false);
}

static inline bool	intersect_caps2_hy(t_ray ray, double dist, t_hyperboloid hyperboloid)
{
	double	t;
	bool	hit;

	if (fabs(ray.dir.y) < 1e-6)
		return (false);
	t = (-1 - ray.origin.y) / ray.dir.y;
	hit = check_cap2_hy(ray, t, hyperboloid);
	if (hit && t > 0 && t < dist)
		return (true);
	t = (1 - ray.origin.y) / ray.dir.y;
	hit = check_cap2_hy(ray, t, hyperboloid);
	if (hit && t > 0 && t < dist)
		return (true);
	return (false);
}

static inline bool	ray_hyperboloid_intersect2(t_hyperboloid hyperboloid, \
t_ray ray, double dist)
{
	t_vec3	abc;

	ray = transform(ray, hyperboloid.transform);
	compute_abc_hy(&abc, ray, &hyperboloid);//make a copy 2 inline here
	if (abc.x == 0)
		return (false);
	if (check_trunk_solutions2_hy(abc, ray, dist))
		return (true);
	if (intersect_caps2_hy(ray, dist, hyperboloid))
		return (true);
	return (false);
}

bool	check_hy_dist(t_hyperboloid *hyperboloids, \
t_ray ray, double dist)
{
	t_hyperboloid	*curr_hy;

	if (hyperboloids == NULL)
		return (false);
	curr_hy = hyperboloids;
	while (true)
	{
		if (curr_hy->shadow && ray_hyperboloid_intersect2(*curr_hy, ray, dist))
			return (true);
		curr_hy = curr_hy->next;
		if (curr_hy == hyperboloids)
			break ;
	}
	return (false);
}
