#include "minirt.h"

void	ray_plane_intersect(t_plane *plane, t_ray ray, t_intersects *intersects)
{
	double	t;

	ray = transform(ray, plane->transform);
	if (fabs(ray.dir.y) < 1e-6)
		return ;
	t = -ray.origin.y / ray.dir.y;
	intersect(intersects, plane, t, PLANE);
}

void	check_planes(t_plane *planes, t_intersects *intersects, t_ray ray)
{
	t_plane		*curr_pl;

	if (planes == NULL)
		return ;
	curr_pl = planes;
	while (true)
	{
		ray_plane_intersect(curr_pl, ray, intersects);
		curr_pl = curr_pl->next;
		if (curr_pl == planes)
			break;
	}
}

//for setting colors/patterns/txs/bump  1 = col, 2 = text, 3 = pattern

t_norm_color set_pl_color(t_comps *comps, t_plane plane, t_point obj_pnt)//add sine selection to this..
{
	t_norm_color out;

	if (plane.option == 0)
		out = plane.color;
	if (plane.option == 1)
	{
		out = texture_plane_at(obj_pnt, plane, comps);
		if (plane.bump && !plane.sine)// and not sine
			bump_pl(obj_pnt, plane, comps);
			//can make option for color to be normal color
	}
	if (plane.option == 2)
		out = pattern_at(plane.pattern,  planar_map(obj_pnt));
		//out = uv_pattern_at(plane.pattern,  planar_map(obj_pnt));
	if (plane.sine)
		sine_ring_norm(obj_pnt, comps, plane);
	return (out);
}

t_comps	set_plcomps(t_plane *plane, t_intersects *intersects, t_ray r)
{
	t_comps	comps;
	t_point obj_pnt;
	
	comps.t = intersects->closest->t;
	comps.ray = r;
	comps.point = add_vec(r.origin, scale_vec(comps.t, r.dir));
	obj_pnt = mat_vec_mult(plane->transform, comps.point);
	comps.normal = plane->norm;	
	comps.eyev = neg(r.dir);
	comps.mat = plane->mat;
	if (comps.mat.transp)
		set_indicies(intersects, &comps.n1, &comps.n2);
	if (dot_product(comps.normal, comps.eyev) < 0)
	{
		comps.normal = neg(comps.normal);
		comps.inside = true;
	}
	else
		comps.inside = false;
	comps.color = set_pl_color(&comps, *plane, obj_pnt);
	comps.under_pnt = subtract_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.over_pnt = add_vec(comps.point, scale_vec(1e-6, comps.normal));
	if (plane->w_frost)
		comps.normal = frost(comps.normal);
	return (comps);
}

t_norm_color	color_plane(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths)
{
	t_plane			*plane;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*curr_lt;

	plane = (t_plane *)intersects->closest->object;
	lt_color = color(0, 0, 0);
	comps = set_plcomps(plane, intersects, r);
	if (trace->lights)
	{
		curr_lt = trace->lights;
		while (true)
		{
			comps.light_dir = norm_vec(subtract_vec(curr_lt->center, comps.point));
			handle_light(trace, &comps, &lt_color, curr_lt);
			curr_lt = curr_lt->next;
			if (curr_lt == trace->lights)
				break;
		}
	}
	comps.refl_col = get_reflected(trace, comps, intersects, depths);
	comps.refr_col = get_refracted(trace, comps, intersects, depths);
	return (get_final_color4(trace, comps, lt_color));	
}