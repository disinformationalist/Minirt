#include "minirt.h"

int	ft_round(double num)
{
	return((int)floor(num + 0.5));
}

//playing with color reassign, do in keyhooks
//may need rework closer to book method, these are proof of concepts..

t_norm_color color(double r, double g, double b)
{
	t_norm_color col;

	col.r = r;
	col.g = g;
	col.b = b;
	return(col);
}

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
	
	if ((ft_round(obj_pnt.x) + ft_round(obj_pnt.y) + ft_round(obj_pnt.z)) % 2 == 0)
		col = color(40.0, 40.0, 40.0);
	else
		col = color(255.0, 255.0, 255.0);
	return (col);
}

//not really working this way... must follow book more...

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

