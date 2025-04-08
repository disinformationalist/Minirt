#include "minirt.h"

static inline t_vec3	cyl_normal_at(t_point int_pnt, \
t_cylinder cyl, t_comps *comps)
{
	t_vec3	norm;
	double	dist;

	dist = int_pnt.x * int_pnt.x + int_pnt.z * int_pnt.z;
	if (dist < 1 && int_pnt.y >= 1 - 1e-6)
	{
		comps->is_top = true;
		comps->is_bot = false;
		norm = vec(0, 1, 0, 0);
	}
	else if (dist < 1 && int_pnt.y <= -1 + 1e-6)
	{
		norm = vec(0, -1, 0, 0);
		comps->is_top = false;
		comps->is_bot = true;
	}
	else
	{
		norm = vec(int_pnt.x, 0, int_pnt.z, 0);
		comps->is_top = false;
		comps->is_bot = false;
	}
	norm = mat_vec_mult(cyl.t_transform, norm);
	norm.w = 0;
	return (norm_vec(norm));
}

static inline t_norm_color	set_cyl_color(t_comps *comps, \
t_cylinder cylinder, t_point obj_pnt)
{
	t_norm_color	out;

	if (cylinder.option == 1)
	{
		out = texture_cy_at(obj_pnt, cylinder, comps);
		if (cylinder.bump)
		{
			if (cylinder.bump)
				bump_cy(obj_pnt, cylinder, comps);
		}
	}
	else if (cylinder.option == 2)
		out = pattern_at(cylinder.pattern, cylinder_map(obj_pnt, 1, \
		comps->is_top, comps->is_bot));
	else
		out = cylinder.color;
	return (out);
}

static inline t_comps	set_cycomps(t_cylinder *cylinder, \
t_intersects *intersects, t_ray r)
{
	t_comps	comps;
	t_point	obj_pnt;

	comps.t = intersects->closest->t;
	comps.ray = r;
	comps.point = add_vec(r.origin, scale_vec(comps.t, r.dir));
	obj_pnt = mat_vec_mult(cylinder->transform, comps.point);
	comps.normal = cyl_normal_at(obj_pnt, *cylinder, &comps);
	comps.eyev = neg(r.dir);
	comps.mat = cylinder->mat;
	if (comps.mat.transp)
		set_indicies(intersects, &comps.n1, &comps.n2);
	if (dot_product(comps.normal, comps.eyev) < 0)
	{
		comps.normal = neg(comps.normal);
		comps.inside = true;
	}
	else
		comps.inside = false;
	comps.over_pnt = add_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.under_pnt = subtract_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.color = set_cyl_color(&comps, *cylinder, obj_pnt);
	if (cylinder->w_frost)
		comps.normal = frost(comps.normal, cylinder->fuzz_lev);
	return (comps);
}

t_norm_color	color_cylinder(t_trace *trace, t_ray r, \
t_intersects *intersects, t_depths depths)
{
	t_cylinder		*cylinder;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*lt;

	cylinder = (t_cylinder *)intersects->closest->object;
	lt_color = color(0, 0, 0);
	comps = set_cycomps(cylinder, intersects, r);
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
