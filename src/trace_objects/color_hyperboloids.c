#include "minirt.h"

static inline t_vec3	hyp_normal_at(t_point int_pnt,
			t_hyperboloid hyperboloid, t_comps *comps, double factor)
{
	t_vec3	norm;
	double	dist;

	dist = int_pnt.x * int_pnt.x + int_pnt.z * int_pnt.z;
	if (dist < 1 && int_pnt.y >= 1 - 1e-6)
	{
		norm = vec(0, 1, 0, 0);
		comps->is_top = true;
		comps->is_bot = false;
	}
	else if (dist < 1 && int_pnt.y <= -1 + 1e-6)
	{
		norm = vec(0, -1, 0, 0);
		comps->is_top = false;
		comps->is_bot = true;
	}
	else
	{
		norm = vec(2 * int_pnt.x * factor, -2 * int_pnt.y, 2 * int_pnt.z * factor, 0);
		comps->is_top = false;
		comps->is_bot = false;
	}
	norm = mat_vec_mult(hyperboloid.t_transform, norm);
	norm.w = 0;
	return (norm_vec(norm));
}

static inline t_norm_color	set_hy_color(t_comps *comps, \
t_hyperboloid hy, t_point obj_pnt)
{
	t_norm_color	out;

	if (hy.option == 1)
	{
		out = texture_hy_at(obj_pnt, hy, comps);
		if (hy.bump)
		{
			if (hy.bump)
				bump_hy(obj_pnt, hy, comps);
		}
	}
	else if (hy.option == 2)
		out = pattern_at(hy.pattern, hyperbolic_map(obj_pnt, 1, comps));
	else
		out = hy.color;
	if (hy.w_frost)
		comps->normal = frost(comps->normal, hy.fuzz_lev);
	return (out);
}

static inline t_comps	set_hycomps(t_hyperboloid *hyperboloid, \
	t_intersects *intersects, t_ray r)
{
	t_comps	comps;
	t_point	obj_pnt;

	comps.t = intersects->closest->t;
	comps.ray = r;
	comps.point = add_vec(r.origin, scale_vec(comps.t, r.dir));
	obj_pnt = mat_vec_mult(hyperboloid->transform, comps.point);
	comps.normal = hyp_normal_at(obj_pnt, *hyperboloid, &comps, \
	hyperboloid->waist3 + 1);
	comps.eyev = neg(r.dir);
	comps.mat = hyperboloid->mat;
	if (comps.mat.transp)
		set_indicies(intersects, &comps.n1, &comps.n2);
	if (dot_product(comps.normal, comps.eyev) < 0)
	{
		comps.inside = true;
		comps.normal = neg(comps.normal);
	}
	else
		comps.inside = false;
	comps.over_pnt = add_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.under_pnt = subtract_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.color = set_hy_color(&comps, *hyperboloid, obj_pnt);
	return (comps);
}

t_norm_color	color_hyperboloid(t_trace *trace, t_ray r, \
	t_intersects *intersects, t_depths depths)
{
	t_hyperboloid	*hyperboloid;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*lt;

	hyperboloid = (t_hyperboloid *)intersects->closest->object;
	lt_color = color(0, 0, 0);
	comps = set_hycomps(hyperboloid, intersects, r);
	if (trace->lights)
	{
		lt = trace->lights;
		while (true)
		{
			comps.light_dir = norm_vec(subtract_vec(lt->center, comps.point));
			handle_light(trace, &comps, &lt_color, lt);
			lt = lt->next;
			if (lt == trace->lights)
				break ;
		}
	}
	comps.refl_col = get_reflected(trace, comps, intersects, depths);
	comps.refr_col = get_refracted(trace, comps, intersects, depths);
	return (get_final_color4(trace, comps, lt_color));
}
