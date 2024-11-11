#include "minirt.h"


int	ft_round(double num)
{
	return((int)floor(num + 0.5));
}

//playing with color reassign, do in keyhooks
//may need rework closer to book method, these are proof of concepts..

//places alternating stripes on object

t_norm_color stripe_at(t_point point, t_matrix_4x4 transform)//, t_norm_color color1, t_norm_color color2)
{
	t_norm_color	col;
	t_point			obj_pnt;

	obj_pnt = mat_vec_mult(transform, point);
	
	if ((int)floor(obj_pnt.x * 2) % 2 == 0)
		col = color(40, 40, 40);
	else
		col = color(255, 255, 255);
	return (col);
}

//rings

t_norm_color ring_at(t_point point, t_matrix_4x4 transform)//, t_norm_color color1, t_norm_color color2)
{
	t_norm_color	col;
	t_point			obj_pnt;

	obj_pnt = mat_vec_mult(transform, point);
	
	if ((int)floor(sqrt(obj_pnt.x * obj_pnt.x + obj_pnt.z * obj_pnt.z) * 2) % 2 == 0)
		col = color(40, 40, 40);
	else
		col = color(255, 255, 255);
	return (col);
}

//checkerboard

t_norm_color checker_at(t_point point, t_matrix_4x4 transform)//, t_norm_color color1, t_norm_color color2)
{
	t_norm_color	col;
	t_point			obj_pnt;

	obj_pnt = mat_vec_mult(transform, point);
	
//	if (((int)floor(obj_pnt.x) + (int)floor(obj_pnt.y) + (int)floor(obj_pnt.z)) % 2 == 0)
	if ((ft_round(obj_pnt.x) + ft_round(obj_pnt.y) + ft_round(obj_pnt.z)) % 2 == 0)
		col = color(40.0, 40.0, 40.0);
	else
		col = color(255.0, 255.0, 255.0);
	return (col);
}

//not really working this way... must rework.. map

t_norm_color gradient_at(t_point point, t_matrix_4x4 transform, t_norm_color col1, t_norm_color col2)
{
	t_norm_color	col;
	t_point			obj_pnt;

	obj_pnt = mat_vec_mult(transform, point);
	
	col.r = col1.r + (col1.r - col2.r) * (obj_pnt.x - floor(obj_pnt.x));
	col.g = col1.g + (col1.g - col2.g) * (obj_pnt.x - floor(obj_pnt.x));
	col.b = col1.b + (col1.b - col2.b) * (obj_pnt.x - floor(obj_pnt.x));
	
	return (col);
}
//must assign to a sep tnormcolor to pass into function.

//------------mapping stuff-----------

//sets pat colors and dims

t_pattern	uv_checker(double width, double height, t_norm_color color1, t_norm_color color2)
{
	t_pattern pat;
	
	pat.width = width;
	pat.height = height;
	pat.color1 = color1;
	pat.color2 = color2;
	return (pat);
}

//returns color for given u, v 

t_norm_color uv_checker_at(t_pattern pat, double u, double v)
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

t_norm_color pattern_at(t_pattern pat, t_vec2 uv)//t_pattern map in place of pat
{
	/* t_vec2 uv;
	t_point obj_pnt; */

	//obj_pnt = mat_vec_mult(transform, pnt);
	//uv = sphere_map(obj_pnt);
	return (uv_checker_at(pat, uv.x, uv.y));
}

