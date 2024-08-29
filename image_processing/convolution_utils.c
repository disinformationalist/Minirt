#include "image_processing.h"

void	free_kernel(double **kernel, int j)
{
	while (--j >= 0)
	{
		free (kernel[j]);
		kernel[j] = NULL;
	}
	free(kernel);
	kernel = NULL;
}

int	malloc_kernel(t_filter *f, int kern_size)
{
	int i;

	i = -1;
	f->kernel = (double **)malloc(kern_size * sizeof(double *));
	if (!f->kernel)
		return (-1);
	while (++i < kern_size)
	{
		f->kernel[i] = (double *)malloc(kern_size * sizeof(double));
		if (!f->kernel[i])
		{
			free_kernel(f->kernel, i);
			return (-1);
		}
	}
	return (0);
}

void	filter_section(t_img *img, t_img *img_out, t_filter *f)
{
	int m;
	int n;

	f->red = 0;
	f->green = 0;
	f->blue = 0;
	f->j = -f->half_k - 1;
	n = 0;
	while (++f->j <= f->half_k)
	{
		f->i = -f->half_k - 1;
		m = 0;
		while (++f->i <= f->half_k)
		{
			f->offset = (f->y + f->j) * img->line_len + ((f->x + f->i) * (img->bpp / 8));
			f->pixel = *(unsigned int *)(img->pixels_ptr + f->offset);
				f->red += (((f->pixel >> 16) & 0xFF) * f->kernel[m][n]);
				f->green += ((f->pixel >> 8) & 0xFF) * f->kernel[m][n];
				f->blue += (f->pixel & 0xFF) * f->kernel[m][n];
		m++;
		}
	n++;
	}
	f->avg_r = round(f->red / f->kern_sum);
	f->avg_g = round(f->green / f->kern_sum);
	f->avg_b = round(f->blue / f->kern_sum);
	my_pixel_put(f->x - f->half_k, f->y - f->half_k, img_out, ((f->avg_r << 16) | (f->avg_g << 8) | f->avg_b));
}

void	apply_convo_filter(t_filter *f, t_img *img_pad, t_img *img_out, int width, int height)
{
	f->y = f->half_k - 1;
	while (++f->y < height + f->half_k)
	{
		f->x = f->half_k - 1;
		while (++f->x < width + f->half_k)
			filter_section(img_pad, img_out, f);
	}
}