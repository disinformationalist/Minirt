#include "minirt.h"

//cubetest //add params to t_pattern

t_pattern uv_align_check(t_norm_color main, t_norm_color ul, \
	t_norm_color ur, t_norm_color bl, t_norm_color br)
{
	t_pattern	pat;

	pat.main = main;
	pat.ul = ul;
	pat.ur = ur;
	pat.bl = bl;
	pat.br = br;
	return (pat);
}

//bot face, rest in map_cube2.c

t_vec2	cu_uv_down(t_point pnt)
{
	t_vec2	uv;

	uv.x = fmod((pnt.x + 1), 2.0) / 2.0;
	uv.y = fmod((pnt.z + 1), 2.0) / 2.0;
	return (uv);
}

//test with checkers soon

t_map	cube_map(t_point obj_pnt, t_face face)
{
	t_vec2	uv;
	t_map	map;

	if (face == FRONT)
		uv = cu_uv_front(obj_pnt);
	else if (face == LEFT)
		uv = cu_uv_left(obj_pnt);
	else if (face == RIGHT)
		uv = cu_uv_right(obj_pnt);
	else if (face == UP)
		uv = cu_uv_up(obj_pnt);
	else if (face == DOWN)
		uv = cu_uv_down(obj_pnt);
	else
		uv = cu_uv_back(obj_pnt);
	map.u = uv.x;
	map.v = uv.y;
	return (map);
}
