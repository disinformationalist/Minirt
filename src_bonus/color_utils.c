#include "minirt.h"

uint8_t clamp_color(double color)
{
	if (color > 255)
		return (255);
	else
		return ((uint8_t)color);
}

t_norm_color color(double r, double g, double b)
{
	t_norm_color col;

	col.r = r;
	col.g = g;
	col.b = b;
	return(col);
}

t_norm_color sum_rgbs(t_norm_color sum, t_norm_color to_add)
{
	sum.r += to_add.r;
	sum.g += to_add.g;
	sum.b += to_add.b;
	return (sum);
}

//averages samples components, packs and returns a pixel color

unsigned int avg_samples(t_norm_color sum, double n)
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	r = clamp_color(sum.r / n);
	g = clamp_color(sum.g / n);
	b = clamp_color(sum.b / n);
	return (r << 16 | g << 8 | b);
}

//currently only used in lens
t_norm_color get_final_color(t_trace *trace, t_norm_color color, double light_int)
{
	t_norm_color color_out;

	color_out.r = color.r * (light_int + trace->amb->color.r);
	color_out.g = color.g * (light_int + trace->amb->color.g);
	color_out.b = color.b * (light_int + trace->amb->color.b);

	return (color_out);
}

//send in material here? mult mat.amb * ambcomponent....

t_norm_color get_final_color1(t_trace *trace, t_norm_color color, t_norm_color light_color, double mat_amb)
{
	t_norm_color color_out;

	color_out.r = color.r * (light_color.r + mat_amb * trace->amb->color.r);// 0 - 255 object color, 0 - 1 light colors
	color_out.g = color.g * (light_color.g + mat_amb * trace->amb->color.g);
	color_out.b = color.b * (light_color.b + mat_amb * trace->amb->color.b);

	return (color_out);
}

t_norm_color mult_color(double scalar, t_norm_color color)
{
	t_norm_color col;

	col.r = scalar * color.r;
	col.g = scalar * color.g;
	col.b = scalar * color.b;
	return (col);
}
