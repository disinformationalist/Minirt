#include "minirt.h"

//function to apply ambient , consider pre applying to all objects for efficiency.

t_color	apply_amb(t_amb *amb, t_color obj_color)
{
	t_color res;
	int 	color;

	color = (int)obj_color.r + (int)amb->amb_color.r;
	if (color > 255)//clamps to prevent overflow...
		color = 255;
	res.r = color;
	color = (int)obj_color.g + (int)amb->amb_color.g;
	if (color > 255)
		color = 255;
	res.g = color;
	color = (int)obj_color.b + (int)amb->amb_color.b;
	if (color > 255)
		color = 255;
	res.b = color;
	return (res);
}

unsigned int	get_diffuse_color(double light_intensity, t_color color)
{
	uint8_t r;
	uint8_t g;
	uint8_t b;


	
	r = round_c((double)(color.r) * light_intensity);
    g = round_c((double)(color.g) * light_intensity);
	b = round_c((double)(color.b) * light_intensity);
	return (r << 16 | g << 8 | b);
}

int	round_int(double d)
{
	return (floor(d + .5));
}

uint8_t clamp_color(int color)
{
	if (color > 255)
		return (255);
	return (color);
}

/* unsigned int get_final_color(t_trace *trace, t_color color, double light_intensity)
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	if (trace->amb && trace->lights)//diffuse and ambient
	{
		r = clamp_color(round_int((double)(color.r) * light_intensity + (double)(trace->amb->amb_color.r)));
		g = clamp_color(round_int((double)(color.g) * light_intensity + (double)(trace->amb->amb_color.g)));
		b = clamp_color(round_int((double)(color.b) * light_intensity + (double)(trace->amb->amb_color.b)));
	}
	else if (trace->amb)//ambient only
	{
		r = clamp_color(round_int((double)(color.r) * (double)(trace->amb->amb_color.r)));
		g = clamp_color(round_int((double)(color.g) * (double)(trace->amb->amb_color.g)));
		b = clamp_color(round_int((double)(color.b) * (double)(trace->amb->amb_color.b)));
	}
	else //diffuse only
	{
		r = clamp_color(round_c((double)(color.r) * light_intensity));
		g = clamp_color(round_c((double)(color.g) * light_intensity));
		b = clamp_color(round_c((double)(color.b) * light_intensity));
	}
	return (r << 16 | g << 8 | b);
} */