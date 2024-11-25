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
