#include "minirt.h"

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
