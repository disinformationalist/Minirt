#include "minirt.h"

//get correct cube face for a given point

t_face	face_of_pnt(t_point pnt)
{
	double	coord;
	double	abs_x;
	double	abs_y;
	double	abs_z;

	abs_x = fabs(pnt.x);
	abs_y = fabs(pnt.y);
	abs_z = fabs(pnt.z);
	coord = fmax(fmax(abs_z, abs_y), abs_x);
	if (coord == pnt.x)
		return (RIGHT);
	if (coord == -pnt.x)
		return (LEFT);
	if (coord == pnt.y)
		return (UP);
	if (coord == -pnt.y)
		return (DOWN);
	if (coord == -pnt.z)
		return (FRONT);
	return (BACK);
}

//normal determined by face of cube

static inline t_vec3	cu_normal_at(t_point int_pnt, t_cube cube, t_face face)
{
	t_vec3	norm;

	if (face == UP || face == DOWN)
		norm = vec(0, int_pnt.y, 0, 0);
	else if (face == FRONT || face == BACK)
		norm = vec(0, 0, int_pnt.z, 0);
	else if (face == LEFT || face == RIGHT)
		norm = vec(int_pnt.x, 0, 0, 0);
	else
		norm = vec(0, 1, 0, 0);
	norm = mat_vec_mult(cube.t_transform, norm);
	norm.w = 0;
	return (norm_vec(norm));
}

t_norm_color	set_cu_color(t_comps *comps, t_cube cube, t_point obj_pnt)
{
	t_norm_color	out;

	if (cube.option == 1)
	{
		out = texture_cube_at(obj_pnt, cube, comps, comps->face);
		if (cube.bump && !cube.sine)
			bump_cu(obj_pnt, cube, comps, comps->face);
	}
	else if (cube.option == 2)
		out = pattern_at(cube.pattern, cube_map(obj_pnt, comps->face));
	else
		out = cube.color;
	if (cube.sine)
		sine_ring_norm_cu(obj_pnt, comps, cube.t_transform, cube.i_transform, cube.amp);
	return (out);
}

static inline t_comps	set_cucomps(t_cube *cube, \
t_intersects *intersects, t_ray r)
{
	t_comps		comps;
	t_point		obj_pnt;

	comps.t = intersects->closest->t;
	comps.ray = r;
	comps.point = add_vec(r.origin, scale_vec(comps.t, r.dir));
	obj_pnt = mat_vec_mult(cube->transform, comps.point);
	comps.face = face_of_pnt(obj_pnt);
	comps.normal = cu_normal_at(obj_pnt, *cube, comps.face);
	comps.eyev = neg(r.dir);
	comps.mat = cube->mat;
	if (comps.mat.transp)
		set_indicies(intersects, &comps.n1, &comps.n2);
	if (dot_product(comps.normal, comps.eyev) < 0)
		comps.normal = neg(comps.normal);
	comps.over_pnt = add_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.under_pnt = subtract_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.color = set_cu_color(&comps, *cube, obj_pnt);
	if (cube->w_frost)
		comps.normal = frost(comps.normal, cube->fuzz_lev);
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
