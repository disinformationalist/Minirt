#include "minirt.h"

static inline t_norm_color	pixel_color_get(int x, int y, t_img *img)
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

uint8_t	luminosity(t_norm_color color)
{
	return ((uint8_t)(0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b));
}

t_img	*create_img(void *mlx_ptr, int width, int height)
{
	t_img	*img;

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

//if no height map, build a luminosity based one.

t_img	*build_lumin_map(void *mlx_con, t_img *img, int width, int height)
{
	t_img			*bump_map;
	uint8_t			gray_val;
	unsigned int	gray_col;
	int				i;
	int				j;

	bump_map = create_img(mlx_con, width, height);
	if (!bump_map)
		return (NULL);
	j = -1;
	while (++j < height)
	{
		i = -1;
		while (++i < width)
		{
			gray_val = luminosity(pixel_color_get(i, j, img));
			gray_col = (gray_val << 16 | gray_val << 8 | gray_val);
			my_pixel_put(i, j, bump_map, gray_col);
		}
	}
	return (bump_map);
}
