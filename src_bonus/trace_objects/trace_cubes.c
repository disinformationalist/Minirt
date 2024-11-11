#include "minirt.h"

void ft_swap(double *a, double *b)
{
	double temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static inline bool check_axis(double origin, double dir, double *min, double *max)
{
	double	t_min_num;
	double	t_max_num;
	bool	status;

	t_min_num = -(1 + origin);
	t_max_num = (1 - origin);
	if (fabs(dir) > 0)
	{
		status = true;
		*min = t_min_num / dir;
		*max = t_max_num / dir;
	}
	else
	{
		status = false;
		*min = t_min_num * INFINITY;
		*max = t_max_num * INFINITY;
	}
	if (*min > *max)
		ft_swap(min, max);
	return (status);
}

//using object space

void	ray_cube_intersect(t_cube *cube, t_ray ray, t_intersects *intersects)
{
	t_vec3	mins;
	t_vec3	maxs;
	double	t_min;
	double	t_max;

	ray = transform(ray, cube->transform);
	check_axis(ray.origin.x, ray.dir.x, &mins.x, &maxs.x);
	check_axis(ray.origin.y, ray.dir.y, &mins.y, &maxs.y);
	check_axis(ray.origin.z, ray.dir.z, &mins.z, &maxs.z);
	t_min = fmax(fmax(mins.x, mins.y), mins.z);
	t_max = fmin(fmin(maxs.x, maxs.y), maxs.z);
	if (t_min > t_max)
		return ;
	intersect(intersects, cube, t_min, CUBE);
	intersect(intersects, cube, t_max, CUBE);
}

//failed opti here..
/* static inline bool check_axis(double origin, double dir, double *min, double *max)
{
	double	t_min_num;
	double	t_max_num;
	double	t_min;
	double	t_max;

	t_min_num = -(1 + origin);
	t_max_num = (1 - origin);
	if (fabs(dir) > 0)
	{
		t_min = t_min_num / dir;
		t_max = t_max_num / dir;
		if (t_min > t_max)
			ft_swap(min, max);
	}
	else
	{
		if (origin < -1 || origin > 1)
			return (false);
		else
		{
			t_min = -INFINITY;
			t_max = INFINITY;
		}
	}
	t_min = fmax(*min, t_min);
	t_max = fmin(*max, t_max);
	return (*min <= t_max);
}

//using object space

static inline void	ray_cube_intersect(t_cube *cube, t_ray ray, t_intersects *intersects)
{
	double	min;
	double	max;

	min = -INFINITY;
	max = INFINITY;
	ray = transform(ray, cube->transform);

	if (check_axis(ray.origin.x, ray.dir.x, &min, &max))
		return ;
	if (check_axis(ray.origin.y, ray.dir.y, &min, &max))
		return ;
	if (check_axis(ray.origin.z, ray.dir.z, &min, &max))
		return ;
	intersect(intersects, cube, min, CUBE);
	intersect(intersects, cube, max, CUBE);
} */

void	check_cubes(t_cube *cubes, t_intersects *intersects, t_ray ray)
{
	t_cube	*curr_cu;

	if (cubes == NULL)
		return ;
	curr_cu = cubes;
	while (true)
	{
		ray_cube_intersect(curr_cu, ray, intersects);
		curr_cu = curr_cu->next;
		if (curr_cu == cubes)
			break;
	}
}

void	check_arealts(t_light *lights, t_intersects *intersects, t_ray ray)
{	
	t_light	*curr_lt;

	if (lights == NULL)
		return ;
	curr_lt = lights;
	while (true)
	{
		if (curr_lt->type == AREA)
			ray_cube_intersect(curr_lt->emitter, ray, intersects);
	curr_lt = curr_lt->next;
	if (curr_lt == lights)
		break;
	}
}