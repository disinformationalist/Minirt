#include "minirt.h"

//check spheres for intersection within dist

static inline bool	check_sp_dist(t_sphere *spheres, float dist, t_ray ray, double t)
{
	t_sphere	*curr_sp;

	if (spheres == NULL)
		return (false);
	curr_sp = spheres;

	while (true)
	{
		if (ray_sphere_intersect(*curr_sp, ray.dir, ray.origin, &t))
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

static inline bool	check_pl_dist(t_plane *planes, float dist, t_ray ray, double t)
{
	t_plane	*curr_pl;

	if (planes == NULL)
		return (false);
	curr_pl = planes;
	while (true)
	{
		if (ray_plane_intersect(*curr_pl, ray.dir, ray.origin, &t))
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

static inline bool	check_cy_dist(t_cylinder *cylinders, float dist, t_ray ray, double t)
{
	t_cylinder	*curr_cy;

	if (cylinders == NULL)
		return (false);
	curr_cy = cylinders;
	while (true)
	{
		if (ray_cylinder_intersect(*curr_cy, ray.dir, ray.origin, &t))
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

//check it any object in any list blocks light

static inline bool	check_all_dist(t_trace *trace, float dist, t_ray s_ray)
{
	bool	hit;
	double	t;

	t = dist;
	hit = check_sp_dist(trace->spheres, dist, s_ray, t);
	if (hit)
		return (true);
	hit = check_pl_dist(trace->planes, dist, s_ray, t);
	if (hit)
		return (true);
	hit = check_cy_dist(trace->cylinders, dist, s_ray, t);
	if (hit)
		return (true);
	return (false);
}

//function to check if any object blocks light
 //light center is on surface gives issue
bool	obscured(t_trace *trace, t_point pnt, t_vec3 light_dir, t_vec3 normal)
{
	t_ray	s_ray;
	float	light_dist;

	s_ray.dir = light_dir;//norm_vec(subtract_vec(trace->lights->center, int_pnt));
	s_ray.origin = add_vec(pnt, scale_vec(1e-5, normal));
	light_dist = magnitude(subtract_vec(trace->lights->center, pnt));// use in previous to get light dir test times.
	//check all intersects within distance
	return (check_all_dist(trace, light_dist, s_ray));
}