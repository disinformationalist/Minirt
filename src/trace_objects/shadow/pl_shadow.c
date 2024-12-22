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

bool	check_pl_dist(t_plane *planes, t_ray ray, double dist)
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
