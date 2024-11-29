#include "minirt.h"

static inline t_vec3	cu_normal_at(t_point int_pnt, t_cube cube)
{
	t_vec3	norm;
	double	max;
	double	absx;
	double	absy;
	double	absz;

	int_pnt = mat_vec_mult(cube.transform, int_pnt);
	absx = fabs(int_pnt.x);
	absy = fabs(int_pnt.y);
	absz = fabs(int_pnt.z);
	max = fmax(fmax(absx, absy), absz);
	if (max == absx)
		norm = vec(int_pnt.x, 0, 0, 0);
	else if (max == absy)
		norm = vec(0, int_pnt.y, 0, 0);
	else if (max == absz)
		norm = vec(0, 0, int_pnt.z, 0);
	else
		norm = vec(0, 1, 0, 0);
	norm = mat_vec_mult(cube.t_transform, norm);
	norm.w = 0;
	return (norm_vec(norm));
}

static inline t_comps	set_cucomps(t_cube *cube, \
t_intersects *intersects, t_ray r)
{
	t_comps	comps;

	comps.t = intersects->closest->t;
	comps.ray = r;
	comps.point = add_vec(r.origin, scale_vec(comps.t, r.dir));
	comps.normal = cu_normal_at(comps.point, *cube);
	comps.eyev = neg(r.dir);
	comps.mat = cube->mat;
	if (comps.mat.transp)
		set_indicies(intersects, &comps.n1, &comps.n2);
	if (dot_product(comps.normal, comps.eyev) < 0)
		comps.normal = neg(comps.normal);
	if (cube->w_frost)
		comps.normal = frost(comps.normal);
	comps.over_pnt = add_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.under_pnt = subtract_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.color = cube->color;
	return (comps);
}

t_norm_color	color_cube(t_trace *trace, t_ray r, \
t_intersects *intersects, t_depths depths)
{
	t_cube			*cube;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*lt;

	cube = (t_cube *)intersects->closest->object;
	if (cube->emitter)
		return (cube->color);
	lt_color = color(0, 0, 0);
	comps = set_cucomps(cube, intersects, r);
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
