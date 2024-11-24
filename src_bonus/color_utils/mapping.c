#include "minirt.h"

//maps the obj pnt to spherical, returns uv

t_map sphere_map(t_point obj_pnt)
{
	t_map	map;
	
	obj_pnt.w = 0;
	map.theta = atan2(obj_pnt.x, obj_pnt.z);
	map.phi = acos(obj_pnt.y);	
	map.u = 1 - (map.theta / (2 * M_PI) + 0.5);
	map.v = 1 - map.phi / M_PI;
	return (map);
}

//mapping point to plane

t_map planar_map(t_point obj_pnt)
{
	t_map	map;

	map.u = fmod(obj_pnt.x, 1);
	map.v = fmod(obj_pnt.z, 1);
	return (map);
}

//cylinder map

t_map cylinder_map(t_point obj_pnt)
{
	t_map	map;
	
	map.dist2 = obj_pnt.x * obj_pnt.x + obj_pnt.z * obj_pnt.z;
	map.theta = atan2(obj_pnt.x, obj_pnt.z);
	map.u = 1 - (map.theta / (2 * M_PI) + 0.5);
	if (map.dist2 < 1 && (obj_pnt.y >= 1 - 1e-6 || obj_pnt.y <= -1 + 1e-6))
		map.v = sqrt(map.dist2);
	else
		map.v = fmod(obj_pnt.y, 1.0);
	return (map);
}

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

t_norm_color uv_pattern_at(t_pattern check, t_vec2 uv)
{
	if (uv.y > .8)
	{
		if (uv.x < .2)
			return (check.ul);
		if (uv.x > .8)
			return (check.ur);
	}
	else if (uv.y < .2)
	{
		if (uv.x < .2)
			return (check.bl);
		if (uv.x > .8)
			return (check.br);
	}
	return (check.main);
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

//mapping functions

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
//check all.

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
	uv.y = fmod((1 - pnt.z), 2.0) / 2.0;
	return (uv);
}

t_vec2	cu_uv_down(t_point pnt)
{
	t_vec2	uv;

	uv.x = fmod((1 - pnt.x), 2.0) / 2.0;
	uv.y = fmod((pnt.z + 1), 2.0) / 2.0;
	return (uv);
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
