#include "minirt.h"

uint8_t	clamp_color(double color)
{
	if (color > 255)
		return (255);
	else
		return ((uint8_t)color);
}

t_norm_color	sum_sample_rgbs(t_norm_color sum, t_norm_color to_add)
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

//using for other supersample method
t_norm_color	get_final_color(t_trace *trace, t_norm_color color,
			double light_int)
{
	t_norm_color	color_out;

	color_out.r = color.r * (light_int + trace->amb->color.r);
	color_out.g = color.g * (light_int + trace->amb->color.g);
	color_out.b = color.b * (light_int + trace->amb->color.b);
	return (color_out);
}

//full phong: total_light = ambient + diff + specular

	/* //original for clarity
	//					diffuse component		+		ambient component
	r = clamp_color((color.r * light_int
		+ color.r * trace->amb->color.r) * 255.0);
	g = clamp_color((color.g * light_int
		+ color.g * trace->amb->color.g) * 255.0);
	b = clamp_color((color.b * light_int
		+ color.b * trace->amb->color.b) * 255.0); */
	//moved * 255.0 back into object color during parse for opti...
