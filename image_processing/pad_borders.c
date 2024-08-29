#include "image_processing.h"

	/* else if (op = DUPLICATE)
		img_pad = pad_zeros(mlx_ptr, img, width, height, num_layers); */
	/* else if (op = COLOR)
		img_pad = pad_color(mlx_ptr, img, width, height, num_layers, color); */
	


t_img	*pad_img(void *mlx_ptr, t_img *img, int *height, int *width, int num_layers, t_op op)
{
	t_img *img_pad;
	
	img_pad = NULL;
	if (op == ZEROS)
		img_pad = pad_zeros(mlx_ptr, img, *width, *height, num_layers);
	else if (op == REFLECT)
		img_pad = pad_mirror(mlx_ptr, img, *width, *height, num_layers);
	else
		write(2,"option not available\n", 21);
	if (!img_pad)	
	{
		write(2,"pad_img failure\n", 17);
		return (NULL);
	}
	*width += 2 * num_layers;
	*height += 2 * num_layers;
	return (img_pad);
}

/* pad_zeros() creates a new image from the given one now with a specified
number of layers of zeros around the border, useful as a precursor to 
applying convolution filters of various sizes.

RETURN VALUE: a pntr to a malloced t_img struct on success, NULL on failure. */

t_img	*pad_zeros(void *mlx_ptr, t_img *img, int width, int height, int layers)
{
	t_img			*img_pad;

	img_pad = create_img(mlx_ptr, width + 2 * layers, height + 2 * layers);
	if (!img_pad)
		return (NULL);
	copy_center(img, img_pad, width, height, layers);
	return (img_pad);
}

/* set_rows_color sets  all values in a row of length width to a given
color starting at s_row_index vertically and s_col_index horizontally,
and continuing until creating num_rows of pixels with the color. 
Utility for pad_color()  */

void	set_rows_color(t_img *img, int width, int s_col_index, int s_row_index, int num_rows, unsigned int color)
{
	int	x;
	int	y;

	y = -1;
	while (++y < num_rows)
	{
		x = -1;
		while (++x < width)
			my_pixel_put(s_col_index + x, s_row_index + y, img, color);
	}
}

/* pad_color() works exactly like pad zeros but instead sets a
color border as with pad_zeros(), the returned image is of 
width = previous width + 2 * number of layers, and 
height = previous height + 2 * number of layers. */

t_img	*pad_color(void *mlx_ptr, t_img *img, int width, int height, int layers, unsigned int color)
{
	t_img			*img_pad;
	unsigned int	pixel_color;
	int				x;
	int				y;

	img_pad = create_img(mlx_ptr, width + 2 * layers, height + 2 * layers);
	if (!img_pad)
		return (NULL);
	set_rows_color(img_pad, width + 2 * layers, 0, 0, layers, color);
	y = layers - 1;
	while (++y < height + layers)
	{
		set_rows_color(img_pad, layers, 0, y, 1, color);
		x = layers - 1;
		while (++x < width + layers)
		{
			pixel_color = pixel_color_get(x - layers, y - layers, img);
			my_pixel_put(x, y, img_pad, pixel_color);
		}
		set_rows_color(img_pad, layers, x, y, 1, color);		
	}
	set_rows_color(img_pad, width + 2 * layers, 0, y, layers, color);
	return (img_pad);
}

//t_img	*pad_dupicate(void *mlx_ptr, t_img *img, int width, int height, int layers, unsigned int color)
