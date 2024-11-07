#include "minirt.h"

static inline t_vec3	cu_normal_at(t_point int_pnt, t_matrix_4x4 transform)
{
	t_vec3	norm;
	double	max;
	double	absx;
	double	absy;
	double	absz;

	int_pnt = mat_vec_mult(transform, int_pnt);
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
	norm = mat_vec_mult(transpose(transform), norm);
	norm.w = 0;
	return (norm_vec(norm));
}

static inline t_comps	set_cucomps(t_cube *cube, t_intersects *intersects, t_ray r, t_trace *trace)
{
	t_comps	comps;
	
	(void)trace;
	comps.t = intersects->closest->t;
	comps.ray = r;
	comps.point = add_vec(r.origin, scale_vec(comps.t, r.dir));
	comps.normal = cu_normal_at(comps.point, cube->transform);
	//comps.color = texture cu_at(trace, comps.point, cube->transform, &comps.normal);//if texturing sets texture and bumps//TODO
	comps.color = cube->color;//no texture, build out color options with patterns or texture
	comps.eyev = neg(r.dir);
	comps.mat = cube->mat;
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
	//comps.irrad = irradiance_at(trace, comps.point, trace->gl_tree);
	//comps.irrad = irradiance_at(trace, comps.point, trace->c_tree);


	return (comps);
}

t_norm_color color_cube(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths)
{
	t_cube			*cube;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*curr_lt;
	/* t_norm_color	refl_col;
	t_norm_color	refr_col; */

	cube = (t_cube *)intersects->closest->object;
	if (cube->emitter)
		return (cube->color);
	lt_color = color(0, 0, 0);
	comps = set_cucomps(cube, intersects, r, trace);
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
	/* refl_col = get_reflected(trace, comps, intersects, depths);
	refr_col = get_refracted(trace, comps, intersects, depths);
	return (get_final_color3(trace, comps, lt_color, refl_col, refr_col)); */
}