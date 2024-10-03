#include "image_processing.h"

static inline void top_and_bottom(t_img *img, t_img *img_pad, int width, int height, int layers)
{
	unsigned int	pixel_color;
	int				x;
	int				y;

	y = -1;
	while (++y < layers)
	{
		x = -1;
		while (++x < width)
		{
			//top
			pixel_color = pixel_color_get(x, layers - y, img);
			my_pixel_put(x + layers, y, img_pad, pixel_color);
			//bottom
			pixel_color = pixel_color_get(x, height - (y + 2), img);
			my_pixel_put(x + layers, height + layers + y, img_pad, pixel_color);
		}
	}
}

static inline void right_and_left(t_img *img, t_img *img_pad, int width, int height, int layers)
{
	unsigned int	pixel_color;
	int				x;
	int				y;

	y = -1;
	while (++y < height)
	{
		x = -1;
		while (++x < layers)
		{
			//left s 
			pixel_color = pixel_color_get(layers - x, y, img);
			my_pixel_put(x, y + layers, img_pad, pixel_color);
			//right s
			pixel_color = pixel_color_get(width - (x + 2), y, img);
			my_pixel_put(width + layers + x, y + layers, img_pad, pixel_color);
		}
	}
}

static inline void corners(t_img *img, t_img *img_pad, int width, int height, int layers)
{
	unsigned int	pixel_color;
	int				x;
	int				y;

	y = -1;
	while (++y < layers)
	{
		x = -1;
		while (++x < layers)
		{
			// top left
			pixel_color = pixel_color_get(layers - x, layers - y, img);
			my_pixel_put(x, y, img_pad, pixel_color);
			//top right 
			pixel_color = pixel_color_get(width - (x + 2), layers - y, img);
			my_pixel_put(width + layers + x, y, img_pad, pixel_color);
			//bot left 
			pixel_color = pixel_color_get(layers - x, height - (y + 2), img);
			my_pixel_put(x, height + layers + y, img_pad, pixel_color);
			// bot right
			pixel_color = pixel_color_get(width - (x + 2), height - (y + 2), img);
			my_pixel_put(width + layers + x, height + layers + y, img_pad, pixel_color);

		}
	}
}

void copy_center(t_img *img, t_img *img_pad, int width, int height, int layers)
{
	unsigned int	pixel_color;
	int				x;
	int				y;

	y = layers - 1;
	while (++y < height + layers)
	{
		x = layers - 1;
		while (++x < width + layers)
		{
			pixel_color = pixel_color_get(x - layers, y - layers, img);
			my_pixel_put(x, y, img_pad, pixel_color);
		}
	}
}

t_img	*pad_mirror(void *mlx_ptr, t_img *img, int width, int height, int layers)
{
	t_img	*img_pad;

	img_pad = create_img(mlx_ptr, width + 2 * layers, height + 2 * layers);
	if (!img_pad)
		return (NULL);
	copy_center(img, img_pad, width, height, layers);
	top_and_bottom(img, img_pad, width, height, layers);
	right_and_left(img, img_pad, width, height, layers);
	corners(img, img_pad, width, height, layers);
	return (img_pad);
}

t_img	*loop_reflector(void *mlx_ptr, t_img *img, int *width, int *height, int layers, int iters)
{
	t_img	*img_pad;
	t_img	*img_nxt;
	int 	i;
	
	img_pad = pad_mirror(mlx_ptr, img, *width, *height, layers);
	i = 0;
	*height += 2 * layers;
	*width += 2 *layers;
	while (++i < iters)
	{
		img_nxt = pad_mirror(mlx_ptr, img_pad, *width, *height, layers);
		mlx_destroy_image(mlx_ptr, img_pad->img_ptr);
		free(img_pad);
		*height += 2 * layers;
		*width += 2 * layers;
		img_pad = img_dup(mlx_ptr, img_nxt, *width, *height);
		mlx_destroy_image(mlx_ptr, img_nxt->img_ptr);
		free(img_nxt);
	} 
	
	return (img_pad);
}
