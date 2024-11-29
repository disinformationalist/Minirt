
#include "minirt.h"

static inline t_vec3 hyp_normal_at(t_point int_pnt, t_hyperboloid hyperboloid)
{
	t_vec3	norm;
	double	dist;

	int_pnt = mat_vec_mult(hyperboloid.transform, int_pnt);
	dist = int_pnt.x * int_pnt.x + int_pnt.z * int_pnt.z;
	if (dist < 2 && int_pnt.y >= 1 - 1e-6)//add plus val, dist < 2 + plus val
		norm = vec(0, 1, 0, 0);
	else if (dist < 2 && int_pnt.y <= -1 + 1e-6)//add plus val, dist < 2 + plus val
		norm = vec(0, -1, 0, 0);
	else
	{
		norm = vec(2 * int_pnt.x,
			-2 * int_pnt.y,
			2 * int_pnt.z, 0);
	}
	norm = mat_vec_mult(hyperboloid.t_transform, norm);
	norm.w = 0;
	return (norm_vec(norm));
}

static inline t_comps	set_hycomps(t_hyperboloid *hyperboloid, t_intersects *intersects, t_ray r)
{
	t_comps	comps;
	
	comps.t = intersects->closest->t;
	comps.ray = r;
	comps.point = add_vec(r.origin, scale_vec(comps.t, r.dir));
	comps.normal = hyp_normal_at(comps.point, *hyperboloid);
	comps.color = hyperboloid->color;
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
	if (hyperboloid->w_frost)
		comps.normal = frost(comps.normal);	
	return (comps);
}

t_norm_color color_hyperboloid(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths)
{
	t_hyperboloid	*hyperboloid;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*curr_lt;

	hyperboloid = (t_hyperboloid *)intersects->closest->object;
	lt_color = color(0, 0, 0);
	comps = set_hycomps(hyperboloid, intersects, r);
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
