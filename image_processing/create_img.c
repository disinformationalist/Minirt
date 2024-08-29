#include "image_processing.h"

int	new_img_init(void *mlx_connection, t_img *img, int width, int height)
{
	img->img_ptr = mlx_new_image(mlx_connection,
			width, height);
	if (img->img_ptr == NULL)
		return (-1);
	img->pixels_ptr = mlx_get_data_addr(img->img_ptr,
			&img->bpp, &img->line_len,
			&img->endian);
	return (0);
}

t_img 	*create_img(void *mlx_ptr, int width, int height)
{
	t_img *img;
	
	img = (t_img *)malloc(sizeof(t_img));
	if (!img)
		return (NULL);
	if (new_img_init(mlx_ptr, img, width, height) == -1)
	{
		free(img);
		return (NULL);
	}
	return (img);
}

t_img	*img_cpy(t_img *dest, t_img *src, int width, int height)//works but hard to use..
{
	unsigned int	curr_pix;
	int 			x;
	int				y;
	
	y = -1;
	while (++y < height)
	{
		x = -1;
		while (++x < width)
		{
			curr_pix = pixel_color_get(x, y, src);
			my_pixel_put(x, y, dest, curr_pix);
		}
	} 
	return (dest);
}

t_img	*img_dup(void *mlx_ptr, t_img *img, int width, int height)
{
	t_img			*img_copy;
	unsigned int	curr_pix;
	int 			x;
	int				y;

	img_copy = create_img(mlx_ptr, width, height);
	if (!img_copy)
		return (NULL);
	y = -1;
	while (++y < height)
	{
		x = -1;
		while (++x < width)
		{
			curr_pix = pixel_color_get(x, y, img);
			my_pixel_put(x, y, img_copy, curr_pix);
		}
	} 
	return (img_copy);
}
