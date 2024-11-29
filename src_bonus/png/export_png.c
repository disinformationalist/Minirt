#include "minirt.h"

int	set_png_pixels(t_png_io *png_img, t_img *img, int height, int width)
{
	png_byte	*row;

	png_img->y = -1;
	while (++png_img->y < height)
	{
		row = png_malloc(png_img->png_ptr, sizeof(uint8_t) * \
		width * png_img->pixel_size);
		if (!row)
		{
			perror("Error: export_png row malloc failure\n");
			clean_memory(png_img, png_img->y, true);
			return (-1);
		}
		png_img->row_pointers[png_img->y] = row;
		png_img->x = -1;
		while (++png_img->x < width)
		{
			get_pixel(&png_img->temp_pixel, img, png_img->x, png_img->y);
			*row++ = png_img->temp_pixel.red;
			*row++ = png_img->temp_pixel.green;
			*row++ = png_img->temp_pixel.blue;
			*row++ = 255;
		}
	}
	return (0);
}

int	init_png_structs(t_png_io *png_img, const char *filename)
{
	png_img->fp = fopen(filename, "wb");
	if (!png_img->fp)
	{
		perror("Error: export_png could not open file\n");
		free(png_img);
		return (-1);
	}
	png_img->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, \
	NULL, NULL);
	if (!png_img->png_ptr)
		return (error_1(png_img, \
		"Error: export_png could not create png write struct\n"));
	png_img->info = png_create_info_struct(png_img->png_ptr);
	if (!png_img->info)
	{
		png_destroy_write_struct(&png_img->png_ptr, NULL);
		return (error_1(png_img, \
		"Error: export _png could not create png info struct\n"));
	}
	return (0);
}

static inline void	write_img(t_png_io *png_img)
{
	int	i;
	int	len;

	if (png_img->text != NULL)
	{
		len = 0;
		while (png_img->text[len].text != NULL)
			len++;
		png_set_text(png_img->png_ptr, png_img->info, png_img->text, len);
		i = -1;
		while (++i < len)
		{
			free(png_img->text[i].text);
			free(png_img->text[i].key);
		}
		free(png_img->text[i].key);
		free(png_img->text);
	}
	png_init_io(png_img->png_ptr, png_img->fp);
	png_set_rows(png_img->png_ptr, png_img->info, png_img->row_pointers);
	png_write_png(png_img->png_ptr, png_img->info, \
	PNG_TRANSFORM_IDENTITY, NULL);
}

//-----consider creating a check for t(a) channel and create rgb or rgba
//will have to adjust png_set_ihdr, png color type

int	export_png(const char *filename, t_img *img, int width, int height)
{
	t_png_io	*png_img;

	png_img = (t_png_io *)malloc(sizeof(t_png_io));
	if (!png_img)
		return (-1);
	init_vars(png_img);
	if (init_png_structs(png_img, filename) == -1)
		return (-1);
	png_set_IHDR(png_img->png_ptr, png_img->info, width, height, \
	png_img->depth, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, \
	PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_img->row_pointers = png_malloc(png_img->png_ptr, height \
	* sizeof(png_byte *));
	if (!png_img->row_pointers)
	{
		png_destroy_write_struct(&png_img->png_ptr, &png_img->info);
		return (error_1(png_img, "Error: row ptrs malloc failure\n"));
	}
	if (set_png_pixels(png_img, img, height, width) == -1)
		return (-1);
	png_img->text = NULL;
	write_img(png_img);
	clean_memory(png_img, height, true);
	return (0);
}
