#include "minirt.h"

bool	ray_plane_intersect2(t_plane plane, t_ray ray, double dist)
{
	double	t;

	ray = transform(ray, plane.transform);

	if (fabs(ray.dir.y) < 1e-6)
		return (false);
	t = -ray.origin.y / ray.dir.y;
	if (t > 0 && t < dist)
		return (true);
	return (false);
}

//check planes for intersection within dist

static inline bool	check_pl_dist(t_plane *planes, t_ray ray, double dist)
{
	t_plane	*curr_pl;

	if (planes == NULL)
		return (false);
	curr_pl = planes;
	while (true)
	{
		if (!curr_pl->shadow)
			return (false);
		if (ray_plane_intersect2(*curr_pl, ray, dist))
				return (true);
		curr_pl = curr_pl->next;
		if (curr_pl == planes)
			break;
	}
	return (false);
}

//check cylinders for intersection within dist

static inline bool	check_cy_dist(t_cylinder *cylinders, t_ray ray, double dist)
{
	t_cylinder	*curr_cy;

	if (cylinders == NULL)
		return (false);
	curr_cy = cylinders;
	while (true)
	{
		if (!curr_cy->shadow)
			return (false);
		if (ray_cylinder_intersect2(*curr_cy, ray, dist))
				return (true);
		curr_cy = curr_cy->next;
		if (curr_cy == cylinders)
			break;
	}
	return (false);
}

static inline void check_axis2(double origin, double dir, double *min, double *max)
{
	double t_min_num;
	double t_max_num;

	t_min_num = -(1 + origin);
	t_max_num = (1 - origin);
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

static inline bool	ray_cube_intersect2(t_matrix_4x4 ctransform, t_ray ray, double dist)
{
	t_vec3	mins;
	t_vec3	maxs;
	double	t_min;
	double	t_max;

	ray = transform(ray, ctransform);
	check_axis2(ray.origin.x, ray.dir.x, &mins.x, &maxs.x);
	check_axis2(ray.origin.y, ray.dir.y, &mins.y, &maxs.y);
	check_axis2(ray.origin.z, ray.dir.z, &mins.z, &maxs.z);
	t_min = fmax(fmax(mins.x, mins.y), mins.z);
	t_max = fmin(fmin(maxs.x, maxs.y), maxs.z);
	if (t_min > t_max)
		return (false);
	if ((t_min > 0 && t_min < dist) || (t_max > 0 && t_max < dist))
		return (true);
	return (false);
}

static inline bool	check_cu_dist(t_cube *cubes, t_ray ray, double dist)
{
	t_cube	*curr_cu;

	if (cubes == NULL)
		return (false);
	curr_cu = cubes;
	while (true)
	{
		if (!curr_cu->shadow)
			return (false);
		if (ray_cube_intersect2(curr_cu->transform, ray, dist))
				return (true);
		curr_cu = curr_cu->next;
		if (curr_cu == cubes)
			break;
	}
	return (false);
}

static inline bool	check_le_dist(t_lens *lenses, double dist, t_ray ray, double t)
{
	t_lens	*curr_le;

	if (lenses == NULL)
		return (false);
	curr_le = lenses;
	while (true)
	{
		if (ray_lens_intersect(*curr_le, ray, &t))
		{
			if (t < dist)
				return (true);
		}
		curr_le = curr_le->next;
		if (curr_le == lenses)
			break;
	}
	return (false);
}

//check for light obstruction. currently used
bool	obscured_b(t_trace *trace, t_point lt_pos, t_comps comps)
{
	double	light_dist;
	t_ray	s_ray;

	s_ray = ray(comps.light_dir, comps.over_pnt);
	light_dist = magnitude(subtract_vec(lt_pos, comps.point));//opt this place in comps when getting light dir
	if (check_sp_dist(trace->spheres, s_ray, light_dist))
		return (true);
	if (check_pl_dist(trace->planes, s_ray, light_dist))
		return (true);
	if (check_cy_dist(trace->cylinders, s_ray, light_dist))
		return (true);
	if (check_cu_dist(trace->cubes, s_ray, light_dist))
		return (true);
	return (false);
}

//function to check if any object blocks light. this is only used by lens now.

bool	obscured(t_trace *trace, t_point pnt, t_vec3 light_dir, t_vec3 normal)
{
	t_ray	s_ray;
	double	light_dist;

	s_ray.dir = light_dir;
	s_ray.origin = add_vec(pnt, scale_vec(1e-6, normal));	
	light_dist = magnitude(subtract_vec(trace->lights->center, pnt));// use in previous to get light dir test times.
	if (check_sp_dist(trace->spheres, s_ray, light_dist))
		return (true);
	if (check_pl_dist(trace->planes, s_ray, light_dist))
		return (true);
	if (check_cy_dist(trace->cylinders, s_ray, light_dist))
		return (true);
	return (false);
}
