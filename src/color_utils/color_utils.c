#include "minirt.h"

uint8_t	clamp_color(double color)
{
	if (color >= 255)
		return (255);
	if (color < 0)
		return (0);
	else
		return ((uint8_t)(color));
}

t_norm_color	color(double r, double g, double b)
{
	t_norm_color	col;

	col.r = r;
	col.g = g;
	col.b = b;
	return (col);
}

t_norm_color	sum_rgbs(t_norm_color sum, t_norm_color to_add)
{
	sum.r += to_add.r;
	sum.g += to_add.g;
	sum.b += to_add.b;
	return (sum);
}

//averages samples components, packs and returns a pixel color

unsigned int	avg_samples(t_norm_color sum, double n)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	r = clamp_color(sum.r / n);
	g = clamp_color(sum.g / n);
	b = clamp_color(sum.b / n);
	return (r << 16 | g << 8 | b);
}

t_norm_color	mult_color(double scalar, t_norm_color color)
{
	t_norm_color	col;

	col.r = scalar * color.r;
	col.g = scalar * color.g;
	col.b = scalar * color.b;
	return (col);
}
