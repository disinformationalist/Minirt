#include "minirt.h"

static inline void	ray_plane_intersect(t_plane *plane, t_ray ray, t_intersects *intersects)
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

//finish this for setting colors/patterns/txs/bump

/* void set_pl_color(t_trace *trace, t_comps *comps, t_plane plane)
{
	if (plane.use_tx)//boolean maybe not bool but int 1 = col, 2 = text 3 = pattern
		comps->color = texture_plane_at();//pass in comps and text.
	if (plane.)

} */


t_comps	set_plcomps(t_plane *plane, t_intersects *intersects, t_ray r, t_trace *trace)
{
	t_comps	comps;
	t_point obj_pnt;
	
	(void)trace;
	comps.t = intersects->closest->t;
	comps.ray = r;
	comps.point = add_vec(r.origin, scale_vec(comps.t, r.dir));
	comps.normal = plane->norm;
	obj_pnt = mat_vec_mult(plane->transform, comps.point);
	
	//make a set color function to choose between color, pattern texture, bump
	if (plane->texture)
		comps.color = texture_plane_at(trace, obj_pnt, plane);//if texturing
	else 
		comps.color = checker_at(comps.point, plane->transform);
	//	comps.color = plane->color;

	comps.eyev = neg(r.dir);
	comps.mat = plane->mat;
	set_indicies(intersects, &comps.n1, &comps.n2);//if transp
	if (dot_product(comps.normal, comps.eyev) < 0)
	{
		comps.inside = true;
		comps.normal = neg(comps.normal);
	}
	else
		comps.inside = false;
	comps.over_pnt = add_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.under_pnt = subtract_vec(comps.point, scale_vec(1e-6, comps.normal));
	return (comps);
}

t_norm_color	color_plane(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths)
{
	t_plane			*plane;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*curr_lt;
	t_norm_color	 ref_col;
	t_norm_color	refr_col;

	plane = (t_plane *)intersects->closest->object;
	lt_color = color(0, 0, 0);
	comps = set_plcomps(plane, intersects, r, trace);
	if (trace->lights)//maybe require light?or place outside of this.
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
	ref_col = get_reflected(trace, comps, intersects, depths);
	refr_col = get_refracted(trace, comps, intersects, depths);
	return (get_final_color3(trace, comps, lt_color, ref_col, refr_col));
}