#include "minirt.h"

void	free_ui_matrix(unsigned int **matrix, int j)
{
	while (--j >= 0)
	{
		free (matrix[j]);
		matrix[j] = NULL;
	}
	free(matrix);
	matrix = NULL;
}

void	zero_ui_matrix(unsigned int **pixels_xl, int width, int height)
{
	int	j;
	int	i;
	
	j = -1;
	while (++j < height)
	{
		i = -1;
		while (++i < width)
			pixels_xl[j][i] = 0;
	}
}

unsigned int	**malloc_ui_matrix(int width, int height)
{
	unsigned int	**matrix;
	int				i;

	i = -1;
	matrix = (unsigned int **)malloc(height * sizeof(unsigned int *));
	if (!matrix)
		return (NULL);
	while (++i < height)
	{
		matrix[i] = (unsigned int *)malloc(width * sizeof(unsigned int));
		if (!matrix[i])
		{
			free_ui_matrix(matrix, i);
			return (NULL);
		}
	}
	return (matrix);
}

static void	filter_section_xl(t_img *img, unsigned int **pixels_xl, int kern_size, t_filter *f)
{
	f->red = 0;
	f->green = 0;
	f->blue = 0;
	f->j = -f->half_k - 1;
	while (++f->j <= f->half_k)
	{
		f->i = -f->half_k - 1;
		while (++f->i <= f->half_k)
		{	
			f->pixel = pixels_xl[f->y + f->j][f->x + f->i];
			f->red += (f->pixel >> 16) & 0xFF;
			f->green += (f->pixel >> 8) & 0xFF;
			f->blue += f->pixel & 0xFF;
		}
	}
	f->avg_r = f->red / (kern_size * kern_size);
	f->avg_g = f->green / (kern_size * kern_size);
	f->avg_b = f->blue / (kern_size * kern_size);
	my_pixel_put(f->pix_x, f->pix_y, img, \
	((f->avg_r << 16) | (f->avg_g << 8) | f->avg_b));
}

void	downsample_xl(int width, int height, t_img *img, unsigned int **pixels_xl, int kern_size)
{
	t_filter	f;
	
	f.pix_y = 0;
	f.half_k = kern_size / 2;
	f.y = f.half_k;
	while (f.y < height - f.half_k)
	{
		f.x = f.half_k;
		f.pix_x = 0;
		while (f.x < width - f.half_k)
		{
			filter_section_xl(img, pixels_xl, kern_size, &f);
			f.x += kern_size;
			f.pix_x++;
		}
		f.y += kern_size;
		f.pix_y++;
	}
}
