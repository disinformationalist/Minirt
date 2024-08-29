#include "image_processing.h"

//must parrellelize very resource intensive..

//use reflect pad before filtering

/* 1) for each pixel get an n x n(7x7)(make adjustable) patch centered at pixel
2) calculate distance(similarity) between its patch and patchs within a larger area centered at target pixel.
-> distance is just euclidean.
3)convert patches to weights using gaussian kernel.
4)norm weights to sum to 1.  */

			/* f->offset = (f->y + f->j) * img->line_len + \
			((f->x + f->i) * (img->bpp / 8));
			f->pixel = *(unsigned int *)(img->pixels_ptr + f->offset); */

void	get_patch_mean(t_img *img_pad, int patch_area, t_filter *f)
{
	double pix_r;
	double pix_g;
	double pix_b;
	
	pix_r = 0;//maybe switch to use t_color
	pix_g = 0;
	pix_b = 0;
	f->mean_r = 0;
	f->mean_g = 0;
	f->mean_b = 0;

	f->j = -f->half_k - 1;
	while (++f->j <= f->half_k)
	{
		f->i = -f->half_k - 1;
		while (++f->i <= f->half_k)
		{
			f->pixel = pixel_color_get(f->x_r + f->i, f->y_r + f->j, img_pad);
			pix_r += (f->pixel >> 16) & 0xFF;
			pix_g += (f->pixel >> 8) & 0xFF;
			pix_b += f->pixel & 0xFF;
		}
	}
	f->mean_r = pix_r / patch_area;
	f->mean_g = pix_g / patch_area;
	f->mean_b = pix_b / patch_area;
	//if (f->x == 486 && f->y == 303)// && f->i == 1)
	//printf("mean_r: %f mean g: %f mean b: %f\n", f->mean_r, f->mean_g, f->mean_b);
}

static void	compute_nlm_pixels(t_img *img_pad, t_img *img, int patch_area, t_filter *f)
{
	int j;
	int i;
	int m;
	int n;

	f->red = 0;
	f->green = 0;
	f->blue = 0;
	j = -f->half_k - 1;
	n = 0;
	while (++j <= f->half_k)//added + 1 to both loops
	{
		i = -f->half_k - 1;
		m = 0;
		while (++i <= f->half_k)
		{
			//weighted avg of means here..
			f->x_r = f->x + i;
			f->y_r = f->y + j;
			get_patch_mean(img_pad, patch_area, f);
			f->red += f->mean_r * f->kernel[m][n];//f->red, etc.. getting reset each time patch mean is called
			f->green += f->mean_g * f->kernel[m][n];
			f->blue += f->mean_b * f->kernel[m][n];

		m++;
		}
	n++;
	}
	f->avg_r = round(f->red / f->kern_sum);
	f->avg_g = round(f->green / f->kern_sum);
	f->avg_b = round(f->blue / f->kern_sum);
	//if (f->x == 486 && f->y == 303)// && f->i == 1)
	//	printf("_r: %d  g: %d  b: %d\n", f->avg_r, f->avg_g, f->avg_b);
	
	my_pixel_put(f->x - 2 * f->half_k, f->y - 2 * f->half_k, img, ((f->avg_r << 16) | (f->avg_g << 8) | f->avg_b));
}

int	non_local_means(void *mlx_ptr, t_img *img, int width, int height, int patch_size, double sigma)//ret -1 on failure
{
	int 		pad_layers;
	t_filter	f;
	t_img		*img_pad;
	int			patch_area;


	f.half_k = patch_size / 2;
	pad_layers = f.half_k * 2;
	patch_area = patch_size * patch_size;
	printf("here is the pad_layers: %d\n", pad_layers);
	if (make_gaussian(&f, patch_size, sigma) == -1)
		return (-1);
	img_pad = pad_mirror(mlx_ptr, img, width, height, pad_layers);
	if (img_pad == NULL)
	{
		write(2,"pad_img failure\n", 17);
		return (-1);
	}
	//multithread would start here, after pad.
	f.y = pad_layers - 1;
	while (++f.y < height + pad_layers)
	{
		f.x = pad_layers - 1;
		while (++f.x < width + pad_layers)
			compute_nlm_pixels(img_pad, img, patch_area, &f);//change here
	}
	mlx_destroy_image(mlx_ptr, img_pad->img_ptr);
	free_kernel(f.kernel, patch_size);
	free(img_pad);
	return (0);
}
//---------stuff to make a better one
/* 
double patch_dist(t_filter *f, t_img *img_pad, int patch_area)
{
	double dist = 0.0;
	double pix_r;
	double pix_g;
	double pix_b;
	
	pix_r = 0;//maybe switch to use t_color
	pix_g = 0;
	pix_b = 0;
	f->mean_r = 0;
	f->mean_g = 0;
	f->mean_b = 0;
//need to compute for each pixel in p and neighbor q, take average.
	f->j = -f->half_k - 1;
	while (++f->j <= f->half_k)
	{
		f->i = -f->half_k - 1;
		while (++f->i <= f->half_k)
		{
			f->pixel = pixel_color_get(f->x_r + f->i, f->y_r + f->j, img_pad);
			dist += delta(f->pixel >> 16) & 0xFF;
			dist += (f->pixel >> 8) & 0xFF;
			dist += f->pixel & 0xFF;
		}
	}
	return (dist / (3 * patch_area));
} */