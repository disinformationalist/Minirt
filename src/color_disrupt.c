#include "minirt.h"

//playing with color reassign, do in keyhooks, point must move with translation, rotation.

t_norm_color color(double r, double g, double b)
{
	t_norm_color col;

	col.r = r;
	col.g = g;
	col.b = b;
	return(col);
}


//dummy disruptions for testing
//disrupt color using object space. working.

t_norm_color stripe_at(t_point point, t_matrix_4x4 transform)//, t_norm_color color1, t_norm_color color2)
{
	t_norm_color	col;
	t_point			obj_pnt;

	obj_pnt = mat_vec_mult(transform, point);
	
	/* if ((int)floor(sqrt(obj_pnt.x * obj_pnt.x + obj_pnt.z * obj_pnt.z) * 2) % 2 == 0)
		col = color(40, 40, 40); */
	if ((int)floor(obj_pnt.x * 2) % 2 == 0)
		col = color(40, 40, 40);
	else
		col = color(255, 255, 255);
	return (col);
}

t_norm_color checker_at(t_point point, t_matrix_4x4 transform)//, t_norm_color color1, t_norm_color color2)
{
	t_norm_color	col;
	t_point			obj_pnt;

	obj_pnt = mat_vec_mult(transform, point);
	
	if ((int)((floor(obj_pnt.x) + floor(obj_pnt.y) + floor(obj_pnt.z))) % 2 == 0)
		col = color(40, 40, 40);
	else
		col = color(255, 255, 255);
	return (col);
}

