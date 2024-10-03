#include "image_processing.h"

//sets the value of a pixel at coordinates x, y

void	my_pixel_put(int x, int y, t_img *img, unsigned int color)
{
	int	offset;

	offset = (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)(img->pixels_ptr + offset) = color;
}

/* my_pixel_put_plus is like my_pixel_put but adds the color passed as a
parameter to the current color pixel when adding nonoverlaping bits. An example use might 
be to add one color channel r, g, b at a time when working with a "false color" model .
take note that no overflow protection is provided. */

void	my_pixel_put_plus(int x, int y, t_img *img, unsigned int color)
{
	int				offset;
	unsigned int	current_color;

	offset = (y * img->line_len) + (x * (img->bpp / 8));
	current_color = *(unsigned int *)(img->pixels_ptr + offset);
	*(unsigned int *)(img->pixels_ptr + offset) = current_color | color;
}

// pixel_color_get() retreives color of pixel(x, y) of image

unsigned int pixel_color_get(int x, int y, t_img *img)
{
	unsigned int	pixel_color;
	int 			offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	pixel_color = *(unsigned int *)(img->pixels_ptr + offset);
	return (pixel_color);
}
