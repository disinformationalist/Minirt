/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 10:13:46 by jhotchki          #+#    #+#             */
/*   Updated: 2024/02/20 10:13:48 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image_processing.h"

static void	filter_section_d(t_img *img, t_img *img_big, int kern_size, t_filter *f)
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
			f->offset = (f->y + f->j) * img_big->line_len + \
			((f->x + f->i) * (img_big->bpp / 8));
			f->pixel = *(unsigned int *)(img_big->pixels_ptr + f->offset);
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

void	downsample(long width, long height, t_img *img, t_img *img_big, int kern_size) //maybe switch img order, so in, then out. intiuitive//change to ints on inputs
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
			filter_section_d(img, img_big, kern_size, &f);
			f.x += kern_size;
			f.pix_x++;
		}
		f.y += kern_size;
		f.pix_y++;
	}
}

void	convo_down_filter_section(t_img *img, t_img *img_big, t_filter *f)
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
			f->offset = (f->y + f->j) * img_big->line_len + \
			((f->x + f->i) * (img_big->bpp / 8));
			f->pixel = *(unsigned int *)(img_big->pixels_ptr + f->offset);
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
	my_pixel_put(f->pix_x, f->pix_y, img, ((f->avg_r << 16) | (f->avg_g << 8) | f->avg_b));
}

int	gaussian_downsample(long width, long height, t_img *img, t_img *img_big, int kern_size, double sigma)
{
	t_filter	f;

	if (kern_size != 3 && kern_size != 5 && kern_size != 7)
		return (-1);
	if (make_gaussian(&f, kern_size, sigma) == -1)
		return (-1);
	f.pix_y = 0;
	f.half_k = kern_size / 2;
	f.y = f.half_k;
	while (f.y < height - f.half_k)
	{
		f.x = f.half_k;
		f.pix_x = 0;
		while (f.x < width - f.half_k)
		{
			convo_down_filter_section(img, img_big, &f);
			f.x += kern_size;
			f.pix_x++;
		}
		f.y += kern_size;
		f.pix_y++;
	}
	free_kernel(f.kernel, kern_size);
	return (0);
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