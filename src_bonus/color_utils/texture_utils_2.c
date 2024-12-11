#include "minirt.h"

// pixel_color_get() retreives color of pixel(x, y) of image

t_norm_color	pixel_color_get(int x, int y, t_img *img)
{
	unsigned int	pixcolor;
	t_norm_color	pixel_color;
	int				offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	pixcolor = *(unsigned int *)(img->pixels_ptr + offset);
	pixel_color.r = (pixcolor >> 16) & 0xFF;
	pixel_color.g = (pixcolor >> 8) & 0xFF;
	pixel_color.b = pixcolor & 0xFF;
	return (pixel_color);
}

/* the -5, 5 in this should be based on scene size(bounding box) in the future.
planes are using this to repeat and mirror texture infinitely. */

t_vec2	set_plane_uv(t_point obj_pnt, double img_iasp)
{
	double	xmin;
	double	xmax;
	double	zmin;
	double	zmax;
	t_vec2	uv;

	xmin = -5;
	xmax = 5;
	zmin = xmin * img_iasp;
	zmax = xmax * img_iasp;
	uv.x = (obj_pnt.x - xmin) / (xmax - xmin);
	uv.y = (obj_pnt.z - zmin) / (zmax - zmin);
	if (uv.x < 0)
		uv.x = -uv.x;
	if (uv.y < 0)
		uv.y = -uv.y;
	uv.x = 1 - fabs(fmod(uv.x, 2.0) - 1.0);
	uv.y = 1 - fabs(fmod(uv.y, 2.0) - 1.0);
	return (uv);
}
