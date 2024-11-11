#include "minirt.h"

static inline t_comps	set_tricomps(t_tri *tri, t_intersects *intersects, t_ray r, t_trace *trace)
{
	t_comps	comps;
	//t_point obj_pnt;
	
	(void)trace;
	comps.t = intersects->closest->t;
	comps.ray = r;
	comps.point = add_vec(r.origin, scale_vec(comps.t, r.dir));
	/* obj_pnt = mat_vec_mult(tri->transform, comps.point);
	comps.normal = sp_normal_at(obj_pnt, tri->transform); */
	comps.normal = tri->norm;
	comps.eyev = neg(r.dir);
	comps.mat = tri->mat;
	set_indicies(intersects, &comps.n1, &comps.n2);
	if (dot_product(comps.normal, comps.eyev) < 0)
	{
		comps.inside = true;
		comps.normal = neg(comps.normal);
	}
	else
		comps.inside = false;
	comps.under_pnt = subtract_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.over_pnt = add_vec(comps.point, scale_vec(1e-6, comps.normal));
	
		comps.color = tri->color;
	return (comps);
}

t_norm_color color_tri(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths)
{
	t_tri			*tri;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*curr_lt;

	tri = (t_tri *)intersects->closest->object;
	lt_color = color(0, 0, 0);
	comps = set_tricomps(tri, intersects, r, trace);
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

static inline t_comps	set_trianglecomps(t_triangle *tri, t_intersects *intersects, t_ray r, t_trace *trace)
{
	t_comps	comps;
	//t_point obj_pnt;
	
	(void)trace;
	comps.t = intersects->closest->t;
	comps.ray = r;
	comps.point = add_vec(r.origin, scale_vec(comps.t, r.dir));
	/* obj_pnt = mat_vec_mult(tri->transform, comps.point);
	comps.normal = sp_normal_at(obj_pnt, tri->transform); */
	comps.normal = tri->norm;
	comps.eyev = neg(r.dir);
	//comps.mat = tri->mat;
	comps.mat = trace->mesh->mat;
	
	set_indicies(intersects, &comps.n1, &comps.n2);
	if (dot_product(comps.normal, comps.eyev) < 0)
	{
		comps.inside = true;
		comps.normal = neg(comps.normal);
	}
	else
		comps.inside = false;
	comps.under_pnt = subtract_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.over_pnt = add_vec(comps.point, scale_vec(1e-6, comps.normal));
		comps.color = trace->mesh->color;
	
		//comps.color = tri->color;
	return (comps);
}

t_norm_color color_triangle(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths)
{
	t_triangle		*tri;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*curr_lt;

	tri = (t_triangle *)intersects->closest->object;
	lt_color = color(0, 0, 0);
	comps = set_trianglecomps(tri, intersects, r, trace);
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