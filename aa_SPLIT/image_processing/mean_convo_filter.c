#include "image_processing.h"

//USE A GET HEIGHT AND WIDTH, MAYBE WITH H/W STRUCT IN PROG FOR 2 LESS PARAMS


//filters the zero padded image Z_img and stores in in_out

//	offset = y * img->line_len + (x * (img->bpp / 8));
//	pixel = *(unsigned int *)(img->pixels_ptr + offset);

static void	mean_filter_section(t_img *img, t_img *img_out, int kern_size, t_filter *f)
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
			f->offset = (f->y + f->j) * img->line_len + \
			((f->x + f->i) * (img->bpp / 8));
			f->pixel = *(unsigned int *)(img->pixels_ptr + f->offset);
			f->red += (f->pixel >> 16) & 0xFF;
			f->green += (f->pixel >> 8) & 0xFF;
			f->blue += f->pixel & 0xFF;
		}
	}
	f->avg_r = f->red / (kern_size * kern_size);
	f->avg_g = f->green / (kern_size * kern_size);
	f->avg_b = f->blue / (kern_size * kern_size);
	my_pixel_put(f->x - f->half_k, f->y - f->half_k, img_out, \
	((f->avg_r << 16) | (f->avg_g << 8) | f->avg_b));
}

/* the mean convo filter takes a pointer to an image, and performs a smoothing
action via taking the mean of a kern_size x kern_size set of pixels and 
setting the center pixel to this value, this is done for all pixels in the
image. This action overwrites the original input img. This results in a 
smoothed or "blurred" image. Raising the kernel size increases the strength 
of smoothing.
RETURN VALUE: 0 on success, -1 on failure.  */
//using the below line instead using color border for filtering	
//img_pad = pad_color(mlx_ptr, img, width , height, f.half_k, <hex_color>);

int	mean_convo_filter(void *mlx_ptr, t_img *img, int width, int height, int kern_size)
{
	t_filter	f;
	t_img		*img_pad;

	f.half_k = kern_size / 2;
	//img_pad = pad_color(mlx_ptr, img, width , height, f.half_k, 0xFF0000);
	img_pad = pad_mirror(mlx_ptr, img, width, height, f.half_k);
	//img_pad = pad_zeros(mlx_ptr, img, width , height, f.half_k);
	if (img_pad == NULL)
	{
		write(2,"pad_img failure\n", 17);
		return (-1);
	}
	f.y = f.half_k - 1;
	while (++f.y < height + f.half_k)
	{
		f.x = f.half_k - 1;
		while (++f.x < width + f.half_k)
			mean_filter_section(img_pad, img, kern_size, &f);// generic function accept variadic on loops?
	}
	mlx_destroy_image(mlx_ptr, img_pad->img_ptr);
	free(img_pad);
	return (0);
}