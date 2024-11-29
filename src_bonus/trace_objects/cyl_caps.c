#include "minirt.h"

bool	check_cap(t_ray ray, double t)
{
	double	x;
	double	z;

	x = ray.origin.x + t * ray.dir.x;
	z = ray.origin.z + t * ray.dir.z;
	if ((x * x + z * z) <= 1)
		return (true);
	return (false);
}

//later return false if cyl.closed == false

bool	intersect_caps(t_ray ray, double *t3, double *t4)
{
	double	t;
	bool	hit1;
	bool	hit2;

	*t3 = INFINITY;
	*t4 = INFINITY;
	if (fabs(ray.dir.y) < 1e-6)
		return (false);
	t = (-1 - ray.origin.y) / ray.dir.y;
	hit1 = check_cap(ray, t);
	if (hit1)
		*t4 = t;
	t = (1 - ray.origin.y) / ray.dir.y;
	hit2 = check_cap(ray, t);
	if (hit2)
		*t3 = t;
	if (hit1 || hit2)
		return (true);
	return (false);
}
