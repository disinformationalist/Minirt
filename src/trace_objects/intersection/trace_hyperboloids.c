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

static inline void	compute_abc(t_vec3 *abc, t_ray r, double waist)
{
	double	factor;

	factor = waist + 1;
	abc->x = (r.dir.x * r.dir.x + r.dir.z * r.dir.z) * factor - r.dir.y * r.dir.y;
	abc->y = 2 * ((r.origin.x * r.dir.x + r.origin.z * r.dir.z) * factor - r.origin.y * r.dir.y);
	abc->z = (r.origin.x * r.origin.x + r.origin.z * r.origin.z) * factor - r.origin.y * r.origin.y - waist;
}

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

static inline bool	intersect_caps(t_ray r, double *t3, double *t4)
{
	double	t;
	bool	hit1;
	bool	hit2;

	*t3 = INFINITY;
	*t4 = INFINITY;
	if (fabs(r.dir.y) < 1e-6)
		return (false);
	t = (-1 - r.origin.y) / r.dir.y;
	hit1 = check_cap(r, t);
	if (hit1)
		*t4 = t;
	t = (1 - r.origin.y) / r.dir.y;
	hit2 = check_cap(r, t);
	if (hit2)
		*t3 = t;
	if (hit1 || hit2)
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

void	r_hype_intersect(t_hyperboloid *hype, \
t_ray r, t_intersects *intersects)
{
	t_vec3	abc;
	double	t1;
	double	t2;
	double	t3;
	double	t4;

	r = transform(r, hype->transform);
	compute_abc(&abc, r, hype->waist3);
	if (fabs(abc.x) < 1e-6)
		return ;
	if (check_trunk_solutions(abc, &t1, &t2))
	{
		if (within_height(r, t1))
			intersect(intersects, hype, t1, HYPERBOLOID);
		if (within_height(r, t2))
			intersect(intersects, hype, t2, HYPERBOLOID);
	}
	if (hype->caps && intersect_caps(r, &t3, &t4))
	{
		if (t3 < INFINITY)
			intersect(intersects, hype, t3, HYPERBOLOID);
		if (t4 < INFINITY)
			intersect(intersects, hype, t4, HYPERBOLOID);
	}
}

void	check_hyperboloids(t_hyperboloid *hypes, \
t_intersects *intersects, t_ray r)
{
	t_hyperboloid	*curr_hy;

	if (hypes == NULL)
		return ;
	curr_hy = hypes;
	while (true)
	{
		r_hype_intersect(curr_hy, r, intersects);
		curr_hy = curr_hy->next;
		if (curr_hy == hypes)
			break ;
	}
}