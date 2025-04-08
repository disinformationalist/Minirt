#include "minirt.h"

static inline void	check_axis_box(double origin, double dir, \
	double *min, double *max, t_vec2 cmm)
{
	double t_min_num;
	double t_max_num;

	t_min_num = cmm.x - origin;
	t_max_num = cmm.y - origin;
	if (fabs(dir) > 0)
	{
		*min = t_min_num / dir;
		*max = t_max_num / dir;
	}
	else
	{
		*min = t_min_num * INFINITY;
		*max = t_max_num * INFINITY;
	}
	if (*min > *max)
		ft_swap(min, max);
}

//might need: ray = transform(ray, ctransform);

bool	ray_box_intersect(t_box *box, t_mat4 ctransform, t_ray ray)
{
	t_vec3	mins;
	t_vec3	maxs;
	t_vec2	cmm;
	double	t_min;
	double	t_max;

	(void)ctransform;
	cmm.x = box->min.x;
	cmm.y = box->max.x;
	check_axis_box(ray.origin.x, ray.dir.x, &mins.x, &maxs.x, cmm);
	cmm.x = box->min.y;
	cmm.y = box->max.y;
	check_axis_box(ray.origin.y, ray.dir.y, &mins.y, &maxs.y, cmm);
	cmm.x = box->min.z;
	cmm.y = box->max.z;
	check_axis_box(ray.origin.z, ray.dir.z, &mins.z, &maxs.z, cmm);
	t_min = fmax(fmax(mins.x, mins.y), mins.z);
	t_max = fmin(fmin(maxs.x, maxs.y), maxs.z);
	return (t_min <= t_max  && t_max >= 0);
}

//cube intersect check 
/* static inline bool	check_cu_dist(t_cube *cubes, t_ray ray, double dist)
{
	t_cube	*curr_cu;

	if (cubes == NULL)
		return (false);
	curr_cu = cubes;
	while (true)
	{
		if (curr_cu->shadow && ray_cube_intersect2(curr_cu->transform, \
			ray, dist))
				return (true);
		curr_cu = curr_cu->next;
		if (curr_cu == cubes)
			break;
	}
	return (false);
} */