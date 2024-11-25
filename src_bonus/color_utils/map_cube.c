#include "minirt.h"

//cubetest //add params to t_pattern

t_pattern uv_align_check(t_norm_color main, t_norm_color ul, t_norm_color ur, t_norm_color bl, t_norm_color br)
{
	t_pattern pat;

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

	uv.x = fmod((1 - pnt.x), 2.0) / 2.0;
	uv.y = fmod((pnt.z + 1), 2.0) / 2.0;
	return (uv);
}

//get correct cube face for a given point

t_face face_of_pnt(t_point pnt)
{
	double coord;

	coord = fmax(fmax(fabs(pnt.x), fabs(pnt.y)), fabs(pnt.z));
	if (coord == pnt.x)
		return (RIGHT);
	if (coord == -pnt.x)
		return (LEFT);
	if (coord == pnt.y)
		return (UP);
	if (coord == -pnt.y)
		return (DOWN);
	if (coord == pnt.z)
		return (RIGHT);
	return (BACK);
}

//test with checkers soon

t_vec2 cube_map(t_point obj_pnt)
{
	t_face	face;
	t_vec2	uv;

	face = face_of_pnt(obj_pnt);
	if (face == FRONT)
		uv = cu_uv_front(obj_pnt);
	if (face == LEFT)
		uv = cu_uv_left(obj_pnt);
	if (face == RIGHT)
		uv = cu_uv_right(obj_pnt);
	if (face == UP)
		uv = cu_uv_up(obj_pnt);
	if (face == DOWN)
		uv = cu_uv_down(obj_pnt);
	else
		uv = cu_uv_back(obj_pnt);
	return (uv);
}
