#include "image_processing.h"

//add to im process for colors.

//linear interpolate..
/* t_color	assign_color_rgb(unsigned int color)
{
	t_color	res;
	
	res.r =  (color >> 16) & 0xFF;
	res.g =  (color >> 8) & 0xFF;
	res.b =  (color) & 0xFF;

	return (res);
}

unsigned int	interpolate_two_colors(t_color color1, t_color color2, int value)
{
	unsigned int res;

	res = (color1.r + ((color2.r - color1.r) * value) / 255) << 16;
	res |= (color1.g + ((color2.g - color1.g) * value) / 255) << 8;
	res |= color1.b + ((color2.b - color1.b) * value) / 255;
	return (res); 
} */

void assign_color_rgb(int color, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = (color >> 16) & 0xFF;
    *g = (color >> 8) & 0xFF;
    *b = color & 0xFF;
}

unsigned int interpolate_two_colors(int color1, int color2, double t)
{
    uint8_t r1, g1, b1;
    uint8_t r2, g2, b2;

    assign_color_rgb(color1, &r1, &g1, &b1);
    assign_color_rgb(color2, &r2, &g2, &b2);

    uint8_t r = r1 + (r2 - r1) * t;
    uint8_t g = g1 + (g2 - g1) * t;
    uint8_t b = b1 + (b2 - b1) * t;

    return ((r << 16) | (g << 8) | b);
}