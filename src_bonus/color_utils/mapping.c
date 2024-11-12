#include "minirt.h"

//maps the obj pnt to spherical, returns uv

t_vec2 sphere_map(t_point obj_pnt)
{
	t_vec2			uv;
	double			theta;
	double			phi;
	double			radius;
	
	obj_pnt.w = 0;
	radius = magnitude(obj_pnt);//pass rad?
	theta = atan2(obj_pnt.x, obj_pnt.z);
	phi = acos(obj_pnt.y / radius);
	uv.x = 1 - (theta / (2 * M_PI) + 0.5);
	uv.y = 1 - phi / M_PI;
	return (uv);
}

//mapping point to plane

t_vec2 planar_map(t_point obj_pnt)
{
	t_vec2	uv;

	uv.x = fmod(obj_pnt.x, 1);
	uv.y = fmod(obj_pnt.z, 1);
	return (uv);
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
