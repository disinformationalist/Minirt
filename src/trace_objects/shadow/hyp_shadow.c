#include "minirt.h"

static inline bool	check_cap(t_ray r, double t)
{
	double	x;
	double	z;

	x = r.origin.x + t * r.dir.x;
	z = r.origin.z + t * r.dir.z;
	if ((x * x + z * z) <= 1)
		return (true);
	return (false);
}

static inline bool	intersect_caps(t_ray ray, double dist)
{
	double	t;
	bool	hit;

	if (fabs(ray.dir.y) < 1e-6)
		return (false);
	t = (-1 - ray.origin.y) / ray.dir.y;
	hit = check_cap(ray, t);
	if (hit && t > 0 && t < dist)
		return (true);
	t = (1 - ray.origin.y) / ray.dir.y;
	hit = check_cap(ray, t);
	if (hit && t > 0 && t < dist)
		return (true);
	return (false);
}

static inline bool	within_height(t_ray r, double t)
{
	double	y;

	if (t < 1e-6)
		return (false);
	y = r.origin.y + t * r.dir.y;
	if (y > -1 && y < 1)
		return (true);
	return (false);
}

static inline bool	check_trunk_solutions(t_vec3 abc, \
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
	if (within_height(ray, t) && t < dist)
		return (true);
	t = (-abc.y + sq_discrim) * inv_2a;
	if (within_height(ray, t) && t < dist)
		return (true);
	return (false);
}

static inline void	compute_abc(t_vec3 *abc, t_ray r, double waist)
{
	double	factor;

	factor = waist + 1;
	abc->x = (r.dir.x * r.dir.x + r.dir.z * r.dir.z) * factor - r.dir.y * r.dir.y;
	abc->y = 2 * ((r.origin.x * r.dir.x + r.origin.z * r.dir.z) * factor - r.origin.y * r.dir.y);
	abc->z = (r.origin.x * r.origin.x + r.origin.z * r.origin.z) * factor - r.origin.y * r.origin.y - waist;
}

static inline bool	ray_hyperboloid_intersect2(t_hyperboloid hyperboloid, \
t_ray ray, double dist)
{
	t_vec3	abc;

	ray = transform(ray, hyperboloid.transform);
	compute_abc(&abc, ray, hyperboloid.waist3);
	if (fabs(abc.x) < 1e-6)
		return (false);
	if (check_trunk_solutions(abc, ray, dist))
		return (true);
	if (intersect_caps(ray, dist))
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
