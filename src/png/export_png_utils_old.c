#include "minirt.h"

int	error_1(t_png_io *png_img, const char *msg)
{
	perror(msg);
	fclose(png_img->fp);
	free(png_img);
	return (-1);
}

void	init_vars(t_png_io *png_img)
{
	png_img->pixel_size = 4;
	png_img->depth = 8;
	png_img->png_ptr = NULL;
	png_img->info = NULL;
	png_img->row_pointers = NULL;
}

void	free_png_rows(png_structp png_ptr, png_byte **row_pointers, int j)
{
	int	y;

	y = -1;
	while (++y < j)
		png_free(png_ptr, row_pointers[y]);
	png_free(png_ptr, row_pointers);
}

void	clean_memory(t_png_io *png_img, int j, bool export)
{
	free_png_rows(png_img->png_ptr, png_img->row_pointers, j);
	if (export)
		png_destroy_write_struct(&png_img->png_ptr, &png_img->info);
	else
		png_destroy_read_struct(&png_img->png_ptr, &png_img->info, NULL);
	fclose(png_img->fp);
	free(png_img);
}

// for using the alpha channel , row 4...*row++ = png_img->temp_pixel.alpha;

void	get_pixel(t_pixel *pix_t, t_img *img, int x, int y)
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
