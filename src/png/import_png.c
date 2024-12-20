#include "minirt.h"

static int	init_png_structs_in(t_png_io *png_img, const char *file)
{
	png_img->fp = fopen(file, "rb");
	if (!png_img->fp)
	{
		perror("Error: import_png could not open file\n");
		free(png_img);
		return (-1);
	}
	png_img->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, \
	NULL, NULL);
	if (!png_img->png_ptr)
		return (error_1(png_img, \
		"Error: export_png could not create png read struct\n"));
	png_img->info = png_create_info_struct(png_img->png_ptr);
	if (!png_img->info)
	{
		png_destroy_read_struct(&png_img->png_ptr, NULL, NULL);
		return (error_1(png_img, \
		"Error: export _png could not create png info struct\n"));
	}
	return (0);
}

static int	malloc_rowptrs(t_png_io *png_img, int height)
{
	png_img->row_pointers = png_malloc(png_img->png_ptr, height \
	* sizeof(png_byte *));
	if (!png_img->row_pointers)
	{
		return (error_3(png_img, "Error: row ptrs malloc failure.\n"));
	}
	png_img->y = -1;
	while (++png_img->y < height)
	{
		png_img->row_pointers[png_img->y] = \
		png_malloc(png_img->png_ptr, \
		png_get_rowbytes(png_img->png_ptr, png_img->info));
		if (!png_img->row_pointers[png_img->y])
		{
			free_png_rows(png_img->png_ptr, png_img->row_pointers, png_img->y);
			return (error_3(png_img, "Error: row ptrs malloc failure.\n"));
		}
	}
	return (0);
}

static int	init_import(t_png_io **png_img, t_img **image, const char *file)
{
	*png_img = (t_png_io *)malloc(sizeof(t_png_io));
	if (!(*png_img))
		return (1);
	(*png_img)->png_ptr = NULL;
	(*png_img)->info = NULL;
	(*png_img)->row_pointers = NULL;
	if (init_png_structs_in(*png_img, file) == -1)
		return (1);
	if (setjmp(png_jmpbuf((*png_img)->png_ptr)))
		return (error_3(*png_img, "Error: Failed to setjmp.\n"));
	png_init_io((*png_img)->png_ptr, (*png_img)->fp);
	png_read_info((*png_img)->png_ptr, (*png_img)->info);
	(*png_img)->color_type = \
		png_get_color_type((*png_img)->png_ptr, (*png_img)->info);
	if (init_import_vars(*png_img))
		return (error_3(*png_img, "Unsupported color type.\n"));
	*image = (t_img *)malloc(sizeof(t_img));
	if (!*image)
		return (error_3(*png_img, "Failed to allocate memory for image.\n"));
	return (0);
}

void	set_pixels(t_png_io *png_img, t_img *image, int width, int height)
{
	if (png_img->color_type == PNG_COLOR_TYPE_RGBA)
		set_img_pixels_rgba(png_img, image, width, height);
	else if (png_img->color_type == PNG_COLOR_TYPE_RGB)
		set_img_pixels_rgb(png_img, image, width, height);
	else if (png_img->color_type == PNG_COLOR_TYPE_GRAY)
		set_img_pixels_gray(png_img, image, width, height);
	else if (png_img->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		set_img_pixels_gray_alpha(png_img, image, width, height);
	else
		return ;
}

/* import_img() accepts an mlx_init() pointer, the path of a png img, and 
addresses of the width and height variables. It returns a pointer to a 
malloced t_img struct (defined in image_processing.h), automatically 
setting values of width and height variables declared in the using 
program and passed by reference into import_png() */

t_img	*import_png(void *mlx_ptr, const char *file, int *width, int *height)
{
	t_png_io	*png_img;
	t_img		*image;

	image = NULL;
	png_img = NULL;
	if (init_import(&png_img, &image, file) == 1)
		return (NULL);
	*width = png_get_image_width(png_img->png_ptr, png_img->info);
	*height = png_get_image_height(png_img->png_ptr, png_img->info);
	if (new_img_init(mlx_ptr, image, *width, *height) == -1)
		return (free(image), error_2(png_img, "Error: img_init failure.\n"));
	if (malloc_rowptrs(png_img, *height))
	{
		mlx_destroy_image(mlx_ptr, image->img_ptr);
		free(image);
		return (NULL);
	}
	png_read_image(png_img->png_ptr, png_img->row_pointers);
	set_pixels(png_img, image, *width, *height);
	clean_memory(png_img, *height, false);
	return (image);
}
