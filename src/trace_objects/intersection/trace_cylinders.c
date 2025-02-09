#include "minirt.h"

//add an intersection to the list

static inline void	intersect(t_intersects *intersects, void *object, double t, t_type type)
{
	int	i;

	i = intersects->count - 1;
	while (i >= 0 && intersects->hits[i].t > t)
	{
		intersects->hits[i + 1] = intersects->hits[i];
		i--;
	}
	i++;
	intersects->hits[i].t = t;
	intersects->hits[i].object = object;
	intersects->hits[i].object_type = type;
	intersects->count++;
}

static inline bool	check_trunk_solutions(t_vec3 abc, double *t1, double *t2)
{
	double	discrim;
	double	inv_2a;
	double	sq_discrim;

	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-6)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	*t1 = (-abc.y - sq_discrim) * inv_2a;
	*t2 = (-abc.y + sq_discrim) * inv_2a;
	return (true);
}

void	compute_abc(t_vec3 *abc, t_ray ray)
{
	abc->x = ray.dir.x * ray.dir.x + ray.dir.z * ray.dir.z;
	abc->y = 2 * (ray.origin.x * ray.dir.x + ray.origin.z * ray.dir.z);
	abc->z = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - 1;
}

bool	within_height(t_ray ray, double t)
{
	double	y;

	if (t < 1e-6)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	if (y > -1 && y < 1)
		return (true);
	return (false);
}

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

void	ray_cylinder_intersect(t_cylinder *cylinder, \
t_ray ray, t_intersects *intersects)
{
	t_vec3	abc;
	double	t1;
	double	t2;
	double	t3;
	double	t4;

	ray = transform(ray, cylinder->transform);
	compute_abc(&abc, ray);
	if (fabs(abc.x) < 1e-6)
		return ;
	if (check_trunk_solutions(abc, &t1, &t2))
	{
		if (within_height(ray, t1))
			intersect(intersects, cylinder, t1, CYLINDER);
		if (within_height(ray, t2))
			intersect(intersects, cylinder, t2, CYLINDER);
	}
	if (cylinder->caps && intersect_caps(ray, &t3, &t4))
	{
		if (t3 < INFINITY)
			intersect(intersects, cylinder, t3, CYLINDER);
		if (t4 < INFINITY)
			intersect(intersects, cylinder, t4, CYLINDER);
	}
}

void	check_cylinders(t_cylinder *cylinders, \
t_intersects *intersects, t_ray ray)
{
	t_cylinder	*curr_cy;

	if (cylinders == NULL)
		return ;
	curr_cy = cylinders;
	while (true)
	{
		ray_cylinder_intersect(curr_cy, ray, intersects);
		curr_cy = curr_cy->next;
		if (curr_cy == cylinders)
			break ;
	}
}
