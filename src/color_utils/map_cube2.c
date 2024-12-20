#include "minirt.h"

//mapping functions for each face bottom in map_cube.c

t_vec2	cu_uv_front(t_point pnt)
{
	t_vec2	uv;

	uv.x = fmod((pnt.x + 1), 2.0) / 2.0;
	uv.y = fmod((pnt.y + 1), 2.0) / 2.0;
	return (uv);
}

t_vec2	cu_uv_back(t_point pnt)
{
	t_vec2	uv;

	uv.x = fmod((1 - pnt.x), 2.0) / 2.0;
	uv.y = fmod((pnt.y + 1), 2.0) / 2.0;
	return (uv);
}

t_vec2	cu_uv_right(t_point pnt)
{
	t_vec2	uv;

	uv.x = fmod((1 - pnt.z), 2.0) / 2.0;
	uv.y = fmod((pnt.y + 1), 2.0) / 2.0;
	return (uv);
}

t_vec2	cu_uv_left(t_point pnt)
{
	t_vec2	uv;

	uv.x = fmod((pnt.z + 1), 2.0) / 2.0;
	uv.y = fmod((pnt.y + 1), 2.0) / 2.0;
	return (uv);
}

t_vec2	cu_uv_up(t_point pnt)
{
	t_vec2	uv;

	uv.x = fmod((1 - pnt.x), 2.0) / 2.0;
	uv.y = fmod((pnt.z + 1), 2.0) / 2.0;
	return (uv);
}
