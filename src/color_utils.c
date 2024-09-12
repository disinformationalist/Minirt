#include "minirt.h"

int	round_int(double d)
{
	return (floor(d + .5));
}

uint8_t clamp_color(double color)
{
	if (color > 255)
		return (255);
	return (color);
}

unsigned int get_final_color(t_trace *trace, t_norm_color color, double light_int)
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	if (trace->amb)//check outside if no amb make and set = 0 or require ambient
	{
		r = clamp_color(color.r * (light_int + trace->amb->color.r));// 0 - 255 object color, 0 - 1 light colors
		g = clamp_color(color.g * (light_int + trace->amb->color.g));//light int will become total light color in bonus.
		b = clamp_color(color.b * (light_int + trace->amb->color.b));
	}
	else
	{
		r = clamp_color(color.r * light_int);
		g = clamp_color(color.g * light_int);
		b = clamp_color(color.b * light_int);	
	}

	return (r << 16 | g << 8 | b);
}


//full phong: total_light = ambient + diff + specular

	/* //original for clarity
	//					diffuse component		+		ambient component
	r = clamp_color((color.r * light_int + color.r * trace->amb->color.r) * 255.0);
	g = clamp_color((color.g * light_int + color.g * trace->amb->color.g) * 255.0);
	b = clamp_color((color.b * light_int + color.b * trace->amb->color.b) * 255.0); */
	
	//moved * 255.0 back into object color during parse for opti...