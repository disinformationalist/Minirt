#include "minirt.h"

//rings

t_norm_color	ring_at(t_point point, t_mat4 transform)
{
	t_norm_color	col;
	t_point			obj_pnt;

	obj_pnt = mat_vec_mult(transform, point);
	if ((int)floor(sqrt(obj_pnt.x * obj_pnt.x \
		+ obj_pnt.z * obj_pnt.z) * 2) % 2 == 0)
		col = color(40, 40, 40);
	else
		col = color(255, 255, 255);
	return (col);
}

//not really working this way... must rework.. map

t_norm_color	gradient_at(t_point point, t_mat4 transform, \
	t_norm_color col1, t_norm_color col2)
{
	t_norm_color	col;
	t_point			obj_pnt;

	obj_pnt = mat_vec_mult(transform, point);
	col.r = col1.r + (col1.r - col2.r) * (obj_pnt.x - floor(obj_pnt.x));
	col.g = col1.g + (col1.g - col2.g) * (obj_pnt.x - floor(obj_pnt.x));
	col.b = col1.b + (col1.b - col2.b) * (obj_pnt.x - floor(obj_pnt.x));
	return (col);
}

//sets pat colors and dims

t_pattern	uv_checker(double width, double height, \
	t_norm_color color1, t_norm_color color2)
{
	t_pattern	pat;

	pat.width = width;
	pat.height = height;
	pat.color1 = color1;
	pat.color2 = color2;
	return (pat);
}

//returns color for given u, v 

	/* u2 = ft_round(u * pat.width);
	v2 = ft_round(v * pat.height); */
t_norm_color	uv_checker_at(t_pattern pat, double u, double v)
{
	int	u2;
	int	v2;

	u2 = (int)floor(u * pat.width);
	v2 = (int)floor(v * pat.height);
	if (!((u2 + v2) % 2))
		return (pat.color1);
	else
		return (pat.color2);
}

//this one really returns the color at the point in obj space. 
//takes the pattern, and the uv returned by the map function used.

t_norm_color	pattern_at(t_pattern pat, t_map map)
{
	return (uv_checker_at(pat, map.u, map.v));
}
