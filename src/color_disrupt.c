#include "minirt.h"

//playing with color reassign, do in keyhooks, point must move with translation, rotation.
//or figure out the tranforms from jamis buck

t_norm_color color(float r, float g, float b)
{
	t_norm_color col;

	col.r = r;
	col.g = g;
	col.b = b;
	return(col);
}

t_norm_color stripe(t_point point)//, t_norm_color color1, t_norm_color color2)
{
	t_norm_color col;

	/* if ((int)floor(point.z * 2) % 2 == 0)
		col = color(40, 40, 40); */
	if ((int)floor(sqrt(point.x * point.x + point.z * point.z) * 2) % 2 == 0)
		col = color(40, 40, 40);
	else
		col = color(255, 255, 255);
	return (col);
}
