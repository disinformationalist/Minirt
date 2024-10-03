#include "minirt.h"

//check spheres for intersection within dist

static inline bool	check_sp_dist(t_sphere *spheres, double dist, t_ray ray, double t)
{
	t_sphere	*curr_sp;

	if (spheres == NULL)
		return (false);
	curr_sp = spheres;
	while (true)
	{
		if (ray_sphere_intersect(*curr_sp, ray, &t))
		{
			if (t < dist)
				return (true);
		}
		curr_sp = curr_sp->next;
		if (curr_sp == spheres)
			break;
	}
	return (false);
}

//check planes for intersection within dist

static inline bool	check_pl_dist(t_plane *planes, double dist, t_ray ray, double t)
{
	t_plane	*curr_pl;

	if (planes == NULL)
		return (false);
	curr_pl = planes;
	while (true)
	{
		if (ray_plane_intersect(*curr_pl, ray, &t))
		{
			if (t < dist)
				return (true);
		}
		curr_pl = curr_pl->next;
		if (curr_pl == planes)
			break;
	}
	return (false);
}

//check cylinders for intersection within dist

static inline bool	check_cy_dist(t_cylinder *cylinders, double dist, t_ray ray, double t)
{
	t_cylinder	*curr_cy;

	if (cylinders == NULL)
		return (false);
	curr_cy = cylinders;
	while (true)
	{
		if (ray_cylinder_intersect(*curr_cy, ray, &t))
		{
			if (t < dist)
				return (true);
		}
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

//check it any object in any list blocks light

static inline bool	check_all_dist(t_trace *trace, double dist, t_ray s_ray)
{
	bool	hit;
	double	t;

	t = INFINITY;
	hit = check_sp_dist(trace->spheres, dist, s_ray, t);
	if (hit)
		return (true);
	t = INFINITY;
	hit = check_pl_dist(trace->planes, dist, s_ray, t);
	if (hit)
		return (true);
	t = INFINITY;
	hit = check_cy_dist(trace->cylinders, dist, s_ray, t);
	if (hit)
		return (true);
	t = INFINITY;
	hit = check_le_dist(trace->lenses, dist, s_ray, t);
	if (hit)
		return (true);
	return (false);
}

//function to check if any object blocks light

bool	obscured(t_trace *trace, t_point pnt, t_vec3 light_dir, t_vec3 normal)
{
	t_ray	s_ray;
	double	light_dist;

	s_ray.dir = light_dir;
	s_ray.origin = add_vec(pnt, scale_vec(1e-6, normal));	
	light_dist = magnitude(subtract_vec(trace->lights->center, pnt));// use in previous to get light dir test times.
	return (check_all_dist(trace, light_dist, s_ray));
}

//in bonus must use the center of the correct light.
bool	obscured_b(t_trace *trace, t_ray s_ray, t_point lt_pos, t_point int_pnt)
{
	double	light_dist;

	light_dist = magnitude(subtract_vec(lt_pos, int_pnt));
	return (check_all_dist(trace, light_dist, s_ray));
}