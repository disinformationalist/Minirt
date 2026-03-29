#include "minirt.h"

/* //not using handle types broken somehow, may revisit.
if using then only need to set img pix rgba
void	handle_types(t_png_io *png_img)
{
	if (png_img->depth == 16)
		png_set_strip_16(png_img->png_ptr);
	if (png_img->color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_img->png_ptr);
	if (png_img->color_type == PNG_COLOR_TYPE_GRAY && png_img->depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_img->png_ptr);
    if (png_img->color_type == PNG_COLOR_TYPE_GRAY \
		|| png_img->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_img->png_ptr);
    if (png_get_valid(png_img->png_ptr, png_img->info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_img->png_ptr);
    if (png_img->color_type == PNG_COLOR_TYPE_RGB \
		|| png_img->color_type == PNG_COLOR_TYPE_GRAY)
        png_set_filler(png_img->png_ptr, 0xFF, PNG_FILLER_AFTER);
} */

void	set_img_pixels_rgba(t_png_io *png_img, \
	t_img *image, int width, int height)
{
	png_bytep		row;
	png_bytep		px;
	unsigned int	color;

	png_img->y = -1;
	while (++png_img->y < height)
	{
		row = png_img->row_pointers[png_img->y];
		png_img->x = -1;
		while (++png_img->x < width)
		{
			px = &(row[png_img->x * 4]);
			color = ((unsigned int)px[0]) << 16 | ((unsigned int)px[1] << 8) \
			| (unsigned int)px[2] | ((unsigned int)px[3] << 24);
			*(unsigned int *)(image->pixels_ptr + (4 * width * png_img->y) \
			+ (4 * png_img->x)) = color;
		}
	}
}

void	set_img_pixels_rgb(t_png_io *png_img, \
	t_img *image, int width, int height)
{
	png_bytep		row;
	png_bytep		px;
	unsigned int	color;

	png_img->y = -1;
	while (++png_img->y < height)
	{
		row = png_img->row_pointers[png_img->y];
		png_img->x = -1;
		while (++png_img->x < width)
		{
			px = &(row[png_img->x * 3]);
			color = ((unsigned int)px[0]) << 16 | ((unsigned int)px[1] << 8) \
			| (unsigned int)px[2] | (0xFF << 24);
			*(unsigned int *)(image->pixels_ptr + (4 * width * png_img->y) \
			+ (4 * png_img->x)) = color;
		}
	}
}

void	set_img_pixels_gray_alpha(t_png_io *png_img, \
	t_img *image, int width, int height)
{
	png_bytep		row;
	png_bytep		px;
	unsigned int	color;
	unsigned char	gray_val;
	unsigned char	alpha;

	png_img->y = -1;
	while (++png_img->y < height)
	{
		row = png_img->row_pointers[png_img->y];
		png_img->x = -1;
		while (++png_img->x < width)
		{
			px = &(row[png_img->x * 2]);
			gray_val = px[0];
			alpha = px[1];
			color = ((unsigned int)gray_val) << 16 \
			| ((unsigned int)gray_val << 8) \
			| (unsigned int)gray_val | ((unsigned int)alpha << 24);
			*(unsigned int *)(image->pixels_ptr + (4 * width * png_img->y) \
			+ (4 * png_img->x)) = color;
		}
	}
}

void	set_img_pixels_gray(t_png_io *png_img, \
	t_img *image, int width, int height)
{
	png_bytep		row;
	png_bytep		px;
	unsigned int	color;
	unsigned char	gray_val;

	png_img->y = -1;
	while (++png_img->y < height)
	{
		row = png_img->row_pointers[png_img->y];
		png_img->x = -1;
		while (++png_img->x < width)
		{
			px = &(row[png_img->x]);
			gray_val = px[0];
			color = ((unsigned int)gray_val) << 16 \
			| ((unsigned int)gray_val << 8) \
			| (unsigned int)gray_val | (0xFF << 24);
			*(unsigned int *)(image->pixels_ptr + (4 * width * png_img->y) \
			+ (4 * png_img->x)) = color;
		}
	}
}

int	init_import_vars(t_png_io *png_img)
{
	if (png_img->color_type == PNG_COLOR_TYPE_RGBA)
		png_img->pixel_size = 4;
	else if (png_img->color_type == PNG_COLOR_TYPE_RGB)
		png_img->pixel_size = 3;
	else if (png_img->color_type == PNG_COLOR_TYPE_GRAY)
		png_img->pixel_size = 1;
	else if (png_img->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_img->pixel_size = 2;
	else
		return (1);
	png_img->depth = png_get_bit_depth(png_img->png_ptr, png_img->info);
	return (0);
}
