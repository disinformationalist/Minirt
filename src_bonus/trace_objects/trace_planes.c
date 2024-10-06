#include "minirt.h"

bool	ray_plane_intersect(t_plane plane, t_ray ray, double *t)
{
	ray = transform(ray, plane.transform);

	if (fabs(ray.dir.y) < 1e-5)
		return (false);
	*t = -ray.origin.y / ray.dir.y;
	if (*t > 0)
		return (true);
	return (false);
}

void	check_planes(t_plane *planes, t_track_hits *closest, t_ray ray, double *t)
{
	t_plane		*curr_pl;

	if (planes == NULL)
		return ;
	curr_pl = planes;
	while (true)
	{
		if (ray_plane_intersect(*curr_pl, ray, t))
		{
			if (*t < closest->t)
			{
				closest->t = *t;
				closest->object = curr_pl;
				closest->object_type = PLANE;
			}
		}
		curr_pl = curr_pl->next;
		if (curr_pl == planes)
			break;
	}
}

t_comps	set_plcomps(t_plane *plane, double t, t_ray r)
{
	t_comps	comps;
	
	comps.t = t;
	comps.point = add_vec(r.origin, scale_vec(t, r.dir));
	comps.normal = plane->norm;
	comps.eyev = neg(r.dir);
	comps.mat = plane->mat;
	if (dot_product(comps.normal, comps.eyev) < 0)
	{
		comps.inside = true;
		comps.normal = neg(comps.normal);
	}
	else
		comps.inside = false;
	comps.over_pnt = add_vec(comps.point, scale_vec(1e-5, comps.normal));
	return (comps);
}

t_norm_color	color_plane(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_plane			*plane;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*curr_lt;
	t_norm_color	color1;

	plane = (t_plane *)closest->object;
	lt_color = color(0, 0, 0);
	if (trace->lights)
	{
		comps = set_plcomps(plane, closest->t, r);
		curr_lt = trace->lights;
		while (true)
		{
			comps.light_dir = norm_vec(subtract_vec(curr_lt->center, comps.point));
			handle_light(trace, &comps, &lt_color, curr_lt);
			curr_lt = curr_lt->next;
			if (curr_lt == trace->lights)
				break;
		}	
	//plane->color = stripe_at(int_pnt, plane->transform);//trying color function
	color1 = checker_at(comps.point, plane->transform);//works passing in to get_fin_col
	//color1 = plane->color;
	//color1 = ring_at(int_pnt, plane->transform);
	//color1 = gradient_at(int_pnt, plane->transform, color(0, 255, 0), color(0, 0, 255));
	}
	else
		color1 = plane->color;
	return (get_final_color1(trace, color1, lt_color, comps.mat.amb));
}
