#include "image_processing.h"

/* export_png takes the desired name for the image file(eg: my_img.png), a
t_img struct, and the width and height of the image to export and lastly a 
pointer to an array of png_text structs. If no text is desired to be placed 
in the image metadata, just write NULL this param. 
RETURN VALUE: 0 on success, and -1 on failure, 
freeing all internal memory resources always. */

/* set_png_text_metadata stores txt but doesnt display, as that is not 
supported by gnome veiwer. however, you can use a terminal tool and cmd 
to display text stored for the image here. will place txt in first row as 
<text[0].key> <text[0].text>, next row as <text[1].key> <text[1].text> etc...
adjust this function according to needs. EX: Greetings	 : Enjoy this image
this function can be used in place of passing a value to the last param.*/

/* TERMINAL TOOL VIEWING METADATA: sudo apt-get install libimage-exiftool-perl
Command line input: exiftool image_1.png */

/* void	set_png_text_metadata(png_structp png_ptr, png_infop info)
{
	png_text	text[1];

	text[0].compression = PNG_TEXT_COMPRESSION_NONE;
	text[0].key = "Greetings:";
	text[0].text = "Enjoy this image";
	png_set_text(png_ptr, info, text, 1);
} */

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
	int i;
	int len;

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

int	export_png(const char *filename, t_img *img, int width, int height, png_text *text)
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
	png_img->text = text;
	write_img(png_img);
	clean_memory(png_img, height, true);
	return (0);
}

//below is the old version that doesnt take an array of png_text structs

/* static inline void	write_img(t_png_io *png_img)
{
	set_png_text_metadata(png_img->png_ptr, png_img->info);
	png_init_io(png_img->png_ptr, png_img->fp);
	png_set_rows(png_img->png_ptr, png_img->info, png_img->row_pointers);
	png_write_png(png_img->png_ptr, png_img->info, \
	PNG_TRANSFORM_IDENTITY, NULL);
}

consider creating a check for t(a) channel and create rgb or rgba
will have to adjust png_set_ihdr, png color type

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
	write_img(png_img);
	clean_memory(png_img, height, true);
	return (0);
} */