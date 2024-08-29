#include "image_processing.h"

void	*error_2(t_png_io *png_img, const char *msg)
{
	png_destroy_read_struct(&png_img->png_ptr, &png_img->info, NULL);
	fclose(png_img->fp);
	free(png_img);
	perror(msg);
	return (NULL);
}

int	error_3(t_png_io *png_img, const char *msg)
{
	png_destroy_read_struct(&png_img->png_ptr, &png_img->info, NULL);
	fclose(png_img->fp);
	free(png_img);
	perror(msg);
	return (1);
}