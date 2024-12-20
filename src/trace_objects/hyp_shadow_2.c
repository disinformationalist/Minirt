#include "minirt.h"

static inline bool	check_cap2_hy(t_ray ray, double t, t_hyperboloid hyp)
{
	double	x;
	double	z;

	x = ray.origin.x + t * ray.dir.x;
	z = ray.origin.z + t * ray.dir.z;
	if ((x * x + z * z) <= 1 + hyp.waist_val)
		return (true);
	return (false);
}

bool	intersect_caps2_hy(t_ray ray, double dist,
				t_hyperboloid hyperboloid)
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
