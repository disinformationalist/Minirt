#ifndef MAINLOOP_FTS_H
# define MAINLOOP_FTS_H

#include "types.h"
#include "math.h"


static inline void	my_pixel_put1(int x, int y, t_img *img, unsigned int color)
{
	int	offset;

	offset = (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)(img->pixels_ptr + offset) = color;
}

static inline t_vec3	add_vec1(t_vec3 vec1, t_vec3 vec2)
{
	t_vec3	res;

	res.x = vec1.x + vec2.x;
	res.y = vec1.y + vec2.y;
	res.z = vec1.z + vec2.z;
	res.w = vec1.w + vec2.w;
	return (res);
}

static inline t_vec3	scale_vec1(double scalar, t_vec3 vec)
{
	t_vec3	res;

	res.x = scalar * vec.x;
	res.y = scalar * vec.y;
	res.z = scalar * vec.z;
	res.w = 0;
	return (res);
}

static inline t_vec3	norm_vec1(t_vec3 vec)
{
	t_vec3	normed;
	double	length;

	length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (!length)
		return (vec);
	normed.x = vec.x / length;
	normed.y = vec.y / length;
	normed.z = vec.z / length;
	normed.w = 0;
	return (normed);
}


static inline uint8_t	clamp_color1(double color)
{
	
	color *= 255.0;
	if (color >= 255)
		return (255);
	if (color < 0)
		return (0);
	else
		return ((uint8_t)(color));
}

static inline unsigned int	clamped_col1(t_norm_color col)
{
	t_color	clamped;

	clamped.r = clamp_color1(col.r);
	clamped.g = clamp_color1(col.g);
	clamped.b = clamp_color1(col.b);
	return (clamped.r << 16 | clamped.g << 8 | clamped.b);
}

static inline t_vec3	subtract_vec1(t_vec3 vec1, t_vec3 vec2)
{
	t_vec3	res;

	res.x = vec1.x - vec2.x;
	res.y = vec1.y - vec2.y;
	res.z = vec1.z - vec2.z;
	res.w = vec1.w - vec2.w;
	return (res);
}

static inline t_norm_color	color1(double r, double g, double b)
{
	t_norm_color	col;

	col.r = r;
	col.g = g;
	col.b = b;
	return (col);
}

//used in super
static inline unsigned int	avg_samples1(t_norm_color sum, double n)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	r = clamp_color1(sum.r / n);
	g = clamp_color1(sum.g / n);
	b = clamp_color1(sum.b / n);
	return (r << 16 | g << 8 | b);
}

static inline t_norm_color	sum_rgbs1(t_norm_color sum, t_norm_color to_add)
{
	sum.r += to_add.r;
	sum.g += to_add.g;
	sum.b += to_add.b;
	return (sum);
}


#endif// MAINLOOP_FTS_H