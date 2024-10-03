#include "image_processing.h"
/* offset = pos.y * img->line_len + (pos.x * (img->bpp / 8));
	pixel = *(unsigned int *)(img->pixels_ptr + offset); */

int	adjust_channel(int channel, double times, double plus)
{
	int	adjusted_val;
	
	adjusted_val = (int)(channel * times + plus);
	if (adjusted_val > 255) 
			adjusted_val = 255;
	else if (adjusted_val < 0)
		adjusted_val = 0;
	return (adjusted_val);
}

void	apply_rgb_action(t_img *img, t_position pos, double times, double plus, t_channel c)
{
	unsigned int	pixel;
	int				red;
	int				green;
	int				blue;

	pixel = pixel_color_get(pos.x, pos.y, img);
	red = (int)((pixel >> 16) & 0xFF);
	green = (int)((pixel >> 8) & 0xFF);
	blue = (int)(pixel & 0xFF);
	if (c & R)
		red = adjust_channel(red, times, plus);
	if (c & G)
		green = adjust_channel(green, times, plus);
	if (c & B)
		blue = adjust_channel(blue, times, plus);
	my_pixel_put(pos.x, pos.y, img, (red << 16 | green << 8 | blue));
}

void	adjust_pixels_rgb(t_img *img, int width, int height, double times, double plus, t_channel c)
{
	t_position	pos;

	pos.y = -1;
	while (++pos.y < height)
	{
		pos.x = -1;
		while (++pos.x < width)
			apply_rgb_action(img, pos, times, plus, c);
	}
}


void	combine_pixels(t_img *img1, t_img *img2, t_img *img_out, t_position pos)//fix for different sizes
{
	unsigned int	pixel1;
	unsigned int	pixel2;

	int				offset;
	int				red1;
	int				green1;
	int				blue1;
	int				red2;
	int				green2;
	int				blue2;

	offset = pos.y * img1->line_len + (pos.x * (img1->bpp / 8));
	pixel1 = *(unsigned int *)(img1->pixels_ptr + offset);
	offset = pos.y * img2->line_len + (pos.x * (img1->bpp / 8));
	pixel2 = *(unsigned int *)(img2->pixels_ptr + offset);
	red1 = (int)(((pixel1 >> 16) & 0xFF));
	green1 = (int)(((pixel1 >> 8) & 0xFF));
	blue1 = (int)((pixel1 & 0xFF));
	red2 = (int)(((pixel2 >> 16) & 0xFF));
	green2 = (int)(((pixel2 >> 8) & 0xFF));
	blue2 = (int)((pixel2 & 0xFF));
	if (red1 < 40 && green1 < 40 && blue1 < 40)//may need to try different ways...
	{
		red1 = red2;
		green1 = green2;
		blue1 = blue2;
	}
	/* else if (red2 < 40 && green2 < 40 && blue2 < 40)
	{
		red1 = red1;
		green1 = green1;
		blue1 = blue1;
	} */
	else
	{
		red1 = (int)((double)(red1 + red2) / 2);
		green1 = (int)((double)(green1 + green2) / 2);
		blue1 = (int)((double)(blue1 + blue2) / 2);
	}
	my_pixel_put(pos.x, pos.y, img_out, (red1 << 16 | green1 << 8 | blue1));

}

// TRY USING TRANSPARENCY BLENDING WITH THE OTHER IMAGE, ask chat about

t_img	combine_images(t_img *img1, t_img *img2, int width, int height, void *mlx_connection)///combine two images into one. experiment with different ways. 
{
	t_position	pos;
	t_img 		img_out;

	new_img_init(mlx_connection, &img_out, width, height);
	
	pos.y = -1;
	while (++pos.y < height)
	{
		pos.x = -1;
		while (++pos.x < width)
			combine_pixels(img1, img2, &img_out, pos);
	}
	return (img_out);
}
	/* if (pixel1 == 0 || pixel2 == 0)
	{
		(pixel1 > pixel2)? (pixel1 = pixel1): (pixel1 = pixel2);
		red1 = (int)(((pixel1 >> 16) & 0xFF));
		green1 = (int)(((pixel1 >> 8) & 0xFF));
		blue1 = (int)((pixel1 & 0xFF));
	} */