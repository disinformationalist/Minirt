#include "image_processing.h"
//unfinished mess....
void get_pixel(t_pixel *pix_t, t_img *img, int x, int y)
{
	int				offset;
	unsigned int	pixel;

	offset = y * img->line_len + (x * (img->bpp / 8));
	pixel = *(unsigned int *)(img->pixels_ptr + offset);
	pix_t->alpha = (pixel >> 24) & 0xFF;
	pix_t->red = (pixel >> 16) & 0xFF;
	pix_t->green = (pixel >> 8) & 0xFF;
	pix_t->blue = (pixel & 0xFF);
}

void	gamma_correction(t_pixel *pixel, double gamma)
{
    pixel->red = (uint8_t)(pow(pixel->red / 255.0, gamma) * 255.0);
    pixel->green = (uint8_t)(pow(pixel->green / 255.0, gamma) * 255.0);
    pixel->blue = (uint8_t)(pow(pixel->blue / 255.0, gamma) * 255.0);
}

void	pixel_op(t_img *img, int height, int width, void (*operation)(t_pixel *))
{
	int		x;
	int		y;
	t_pixel	temp_pixel;

	y = -1;
	while (++y < height)
	{
		x = -1;
		while (++x < width)
		{
			get_pixel(&temp_pixel, img, x, y);
			operation(&temp_pixel);
		}
	}
}


void	brighten_pixels(t_img *img, t_position pos, double gamma, uint8_t dim_threshold)
{
	t_pixel	pixel;
	if (red > 255)
		red = 255;
	else if (red < 0)
		red = 0;
	if (green > 255)
		green = 255;
	else if (green < 0)
		green = 0;
	if (blue > 255)
		blue = 255;
	else if (blue < 0)
		blue = 0;
	my_pixel_put(pos.x, pos.y, img, (red << 16 | green << 8 | blue));
}
   
   
   
    i = -1;
	while(++i < num_pix)
    {
        if (pixels[i].red > dim_threshold || pixels[i].green > dim_threshold || pixels[i].blue > dim_threshold)
        {
            gamma_correction(&pixels[i], gamma);
        }
    }
}