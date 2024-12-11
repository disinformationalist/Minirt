#include "minirt.h"

bool	check_cap_hy(t_ray ray, double t, t_hyperboloid *hyperboloid)
{
	double	x;
	double	z;
	double	cap_rad_sq;

	cap_rad_sq = 1 + hyperboloid->waist_val;
	x = (ray.origin.x + t * ray.dir.x);
	z = (ray.origin.z + t * ray.dir.z);
	if (((x * x) + (z * z)) <= cap_rad_sq)
		return (true);
	return (false);
}

//later return false if cyl.closed == false

bool	intersect_caps_hy(t_ray ray, double *t3, double *t4, t_hyperboloid *hyp)
{
	double	t;
	bool	hit1;
	bool	hit2;

	*t3 = INFINITY;
	*t4 = INFINITY;
	if (fabs(ray.dir.y) < 1e-5)
		return (false);
	t = (-1 - ray.origin.y) / ray.dir.y;
	hit1 = check_cap_hy(ray, t, hyp);
	if (hit1)
		*t4 = t;
	t = (1 - ray.origin.y) / ray.dir.y;
	hit2 = check_cap_hy(ray, t, hyp);
	if (hit2)
		*t3 = t;
	if (hit1 || hit2)
		return (true);
	return (false);
}
