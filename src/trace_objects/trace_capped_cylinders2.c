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

bool	intersect_caps(t_ray ray, double half_h, double *t3, double *t4)
{
	double	t;
	bool	hit1;
	bool	hit2;

	*t3 = INFINITY;
	*t4 = INFINITY;
	if (fabs(ray.dir.y) < 1e-5)
		return (false);
	t = (-half_h - ray.origin.y) / ray.dir.y;
	hit1 = check_cap(ray, t);
	if (hit1)
		*t4 = t;
	t = (half_h - ray.origin.y) / ray.dir.y;
	hit2 = check_cap(ray, t);
	if (hit2)
		*t3 = t;
	if (hit1 || hit2)
		return (true);
	return (false);
}

bool	within_height(t_ray ray, double min, double max, double t)
{
	double	y;

	if (t < 1e-5)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	if (y > min && y < max)
		return (true);
	return (false);
}

void	set_t_to_smaller_value(double *t, double *t3, double *t4)
{
	if (*t3 > 0 && *t3 < *t)
		*t = *t3;
	if (*t4 > 0 && *t4 < *t)
		*t = *t4;
}

bool	ray_cylinder_intersect(t_cylinder cylinder, t_ray ray, double *t)
{
	t_vec3	abc;
	double	t1;
	double	t2;
	double	t3;
	double	t4;

	ray = transform(ray, cylinder.transform);
	compute_abc(&abc, ray);
	if (abc.x != 0)
	{
		if (check_trunk_solutions(abc, &t1, &t2))
		{
			if (within_height(ray, -cylinder.half_h, cylinder.half_h, t1)
				&& t1 < *t)
				*t = t1;
			if (within_height(ray, -cylinder.half_h, cylinder.half_h, t2)
				&& t2 < *t)
				*t = t2;
		}
	}
	if (intersect_caps(ray, cylinder.half_h, &t3, &t4))
		set_t_to_smaller_value(t, &t3, &t4);
	if (*t < INFINITY)
		return (true);
	return (false);
}
