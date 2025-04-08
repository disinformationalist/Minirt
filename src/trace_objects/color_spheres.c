#include "minirt.h"

static inline t_vec3	sp_normal_at(t_point obj_pnt, t_mat4 transform)
{
	t_vec3	norm;

	norm = mat_vec_mult(transform, obj_pnt);
	norm.w = 0;
	return (norm_vec(norm));
}

static inline t_norm_color	set_sp_color(t_comps *comps, \
t_sphere sphere, t_point obj_pnt)
{
	t_norm_color	out;

	if (sphere.option == 1)
	{
		out = texture_sp_at(obj_pnt, sphere, comps);
		if (sphere.bump)
			bump_sp(obj_pnt, sphere, comps);
	}
	else if (sphere.option == 2)
		out = pattern_at(sphere.pattern, sphere_map(obj_pnt, 1));
	else
		out = sphere.color;
	return (out);
}

static inline t_comps	set_spcomps(t_sphere *sphere, \
t_intersects *intersects, t_ray r)
{
	t_comps	comps;
	t_point	obj_pnt;

	comps.t = intersects->closest->t;
	comps.ray = r;
	comps.point = add_vec(r.origin, scale_vec(comps.t, r.dir));
	obj_pnt = mat_vec_mult(sphere->transform, comps.point);
	comps.normal = sp_normal_at(obj_pnt, sphere->t_transform);
	comps.eyev = neg(r.dir);
	comps.mat = sphere->mat;
	if (comps.mat.transp)
		set_indicies(intersects, &comps.n1, &comps.n2);
	if (dot_product(comps.normal, comps.eyev) < 0)
	{
		comps.normal = neg(comps.normal);
		comps.inside = true;
	}
	else
		comps.inside = false;
	comps.under_pnt = subtract_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.over_pnt = add_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.color = set_sp_color(&comps, *sphere, obj_pnt);
	if (sphere->w_frost)
		comps.normal = frost(comps.normal, sphere->fuzz_lev);
	return (comps);
}

t_norm_color	color_sphere(t_trace *trace, t_ray r, \
t_intersects *intersects, t_depths depths)
{
	t_sphere		*sphere;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*lt;

	sphere = (t_sphere *)intersects->closest->object;
	lt_color = color(0, 0, 0);
	comps = set_spcomps(sphere, intersects, r);
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
