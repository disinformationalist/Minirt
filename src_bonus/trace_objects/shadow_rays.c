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
		if (ray_cylinder_intersect2(*curr_cy, ray, dist))
				return (true);
		curr_cy = curr_cy->next;
		if (curr_cy == cylinders)
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
bool	obscured_b(t_trace *trace, t_ray s_ray, t_point lt_pos, t_point int_pnt)
{
	double	light_dist;

	light_dist = magnitude(subtract_vec(lt_pos, int_pnt));

	if (check_sp_dist(trace->spheres, s_ray, light_dist))
		return (true);
	if (check_pl_dist(trace->planes, s_ray, light_dist))
		return (true);
	if (check_cy_dist(trace->cylinders, s_ray, light_dist))
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
