#include "minirt.h"

static inline bool	ray_plane_intersect2(t_plane plane, t_ray ray, double dist)
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
		if (curr_pl->shadow && ray_plane_intersect2(*curr_pl, ray, dist))
			return (true);
		curr_pl = curr_pl->next;
		if (curr_pl == planes)
			break ;
	}
	return (false);
}

static inline double	dot_product1(t_vec3 vec1, t_vec3 vec2)
{
	return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z);
}

//check for light obstruction

bool	obscured_b(t_trace *trace, t_point lt_pos, t_comps comps)
{
	double	light_dist;
	t_ray	s_ray;

	s_ray = ray(comps.light_dir, comps.over_pnt);
//	light_dist = magnitude(subtract_vec(lt_pos, comps.point));

	t_vec3	to_light;
	//piece->stats.shadow_rays++;

	to_light = subtract_vec(lt_pos, comps.over_pnt);
	light_dist = dot_product1(to_light, comps.light_dir);
	if (check_hier_dist(trace->bvh, light_dist, s_ray))
		return (true);
	if (check_pl_dist(trace->planes, s_ray, light_dist))
		return (true);

	return (false);
}


//testing

bool	obscured_b_testing(t_trace *trace, t_point lt_pos, t_comps comps, t_intersects *intersects)
{
	double	light_dist;
	t_ray	s_ray;

	s_ray = ray(comps.light_dir, comps.over_pnt);

	t_vec3	to_light;
	intersects->stats.shadow_rays++;

	to_light = subtract_vec(lt_pos, comps.over_pnt);
	light_dist = dot_product1(to_light, comps.light_dir);
	if (check_hier_dist_testing(trace->bvh, light_dist, s_ray, intersects))
		return (true);
	if (check_pl_dist(trace->planes, s_ray, light_dist))
		return (true);

	return (false);
}