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

static inline bool	check_solutions(t_vec3 abc, double *t1, double *t2)
{
	double	discrim;
	double	inv_2a;
	double	sq_discrim;

	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-5)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	*t1 = (-abc.y - sq_discrim) * inv_2a;
	*t2 = (-abc.y + sq_discrim) * inv_2a;
	return (true);
}

//using object space

void	ray_sphere_intersect(t_sphere *sphere, t_ray ray, \
t_intersects *intersects)
{
	double	t1;
	double	t2;
	double	a;
	double	b;
	double	c;

	ray = transform(ray, sphere->transform);
	a = dot_product(ray.dir, ray.dir);
	b = 2.0 * dot_product(ray.origin, ray.dir);
	c = dot_product(ray.origin, ray.origin) - 1;
	if (check_solutions(vec(a, b, c, 0), &t1, &t2))
	{
		intersect(intersects, sphere, t1, SPHERE);
		intersect(intersects, sphere, t2, SPHERE);
	}
}

void	check_spheres(t_sphere *spheres, t_intersects *intersects, t_ray ray)
{
	t_sphere	*curr_sp;

	if (spheres == NULL)
		return ;
	curr_sp = spheres;
	while (true)
	{
		ray_sphere_intersect(curr_sp, ray, intersects);
		curr_sp = curr_sp->next;
		if (curr_sp == spheres)
			break ;
	}
}
