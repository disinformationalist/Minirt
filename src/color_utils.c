#include "minirt.h"

int	round_int(double d)
{
	return (floor(d + .5));
}

uint8_t clamp_color(float color)
{
	if (color > 255)
		return (255);
	return (color);
}

unsigned int get_final_color(t_trace *trace, t_norm_color color, double light_intensity)
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	if (trace->amb)//check outside if no amb make and set = 0 or require ambient
	{
		r = clamp_color(color.r * (light_intensity + trace->amb->color.r));
		g = clamp_color(color.g * (light_intensity + trace->amb->color.g));
		b = clamp_color(color.b * (light_intensity + trace->amb->color.b));
	}
	else
	{
		r = clamp_color(color.r * light_intensity);
		g = clamp_color(color.g * light_intensity);
		b = clamp_color(color.b * light_intensity);	
	}

	return (r << 16 | g << 8 | b);
}
//full phong: total_light = ambient + diff + specular

	/* //original for clarity
	//					diffuse component		+		ambient component
	r = clamp_color((color.r * light_intensity + color.r * trace->amb->color.r) * 255.0);
	g = clamp_color((color.g * light_intensity + color.g * trace->amb->color.g) * 255.0);
	b = clamp_color((color.b * light_intensity + color.b * trace->amb->color.b) * 255.0); */
	
	//moved * 255.0 back into object color during parse for opti...