#include "image_processing.h"

static double gaussian_value(int x, int y, double sigma)
{
	return (exp(-((x * x + y * y) / (2 * sigma * sigma))) / (2 * M_PI * (sigma * sigma)));
}

static void	set_gauss_kernal(t_filter *f, int kern_size, double sigma)
{
	int i;
	int j;
	int center;

	if (kern_size == 3)
		f->factor = 16;
	else if (kern_size == 5)
		f->factor = 276;
	else if (kern_size == 7)
		f->factor = 1003;
	center = kern_size / 2;
	i = -1;
	f->kern_sum = 0;
	while (++i < kern_size)
	{
		j = -1;
		while (++j < kern_size)
		{			
			f->kernel[i][j] = round(gaussian_value(i - center, j - center, sigma) * f->factor);
				f->kern_sum += f->kernel[i][j];
		}
	}
}

int	make_gaussian(t_filter *f, int kern_size, double sigma)
{
	if (malloc_kernel(f, kern_size) == -1)
		return (-1);
	set_gauss_kernal(f, kern_size, sigma);
	print_matrix(f->kernel, kern_size, kern_size);
	printf("Sum of kernel values: %f\n", f->kern_sum);
	return (0);
}

//makes a gaussian with mirror pad..
int	make_gaussian_mirror(t_filter *f, int kern_size, int layers, double sigma)
{
	double **mirror_kernel;

	if (malloc_kernel(f, kern_size) == -1)
		return (-1);
	set_gauss_kernal(f, kern_size, sigma);
	mirror_kernel = pad_mirror_kernel(f, kern_size, kern_size, layers);
	free_kernel(f->kernel, kern_size);
	f->kernel = mirror_kernel;
	kern_size += layers * 2;
	f->kern_sum = get_matrix_sum(f->kernel, kern_size, kern_size);
	print_matrix(mirror_kernel, kern_size, kern_size);
	printf("Sum of kernel values: %f\n", f->kern_sum);
	return (0);
}
//CONSIDER REPLACING FREE KERNEL WITH FREE MATRIX

//RETURN VALUE: 0 on success, -1 on failure.

int	gaussian_convo_filter(void *mlx_ptr, t_img *img, int width, int height, int kern_size, double sigma)
{
	t_filter	f;
	t_img		*img_pad;

	if (kern_size != 3 && kern_size != 5 && kern_size != 7)
		return (0);
	if (make_gaussian(&f, kern_size, sigma) == -1)
		return (0);
	f.half_k = kern_size / 2;
//	img_pad = pad_zeros(mlx_ptr, img, width, height, f.half_k);
	img_pad = pad_mirror(mlx_ptr, img, width, height, f.half_k);
	if (img_pad == NULL)
	{
		free(f.kernel);
		write(2,"pad failure\n", 12);
		return (-1);
	}
	apply_convo_filter(&f, img_pad, img, width, height);
	mlx_destroy_image(mlx_ptr, img_pad->img_ptr);
	free_kernel(f.kernel, kern_size);
	free(img_pad);
	return (0);
}

int	gaussmirror_convo_filter(void *mlx_ptr, t_img *img, int width, int height, int kern_size, int layers, double sigma)
{
	t_filter	f;
	t_img		*img_pad;

	if (kern_size != 3 && kern_size != 5 && kern_size != 7)
		return (0);
	if (make_gaussian(&f, kern_size, sigma) == -1)
		return (0);
	if (make_gaussian_mirror(&f, kern_size, layers, sigma) == -1)//--------------
		return (0);
	kern_size += layers * 2;//-----------
	f.half_k = kern_size / 2;
//	img_pad = pad_zeros(mlx_ptr, img, width, height, f.half_k);
	img_pad = pad_mirror(mlx_ptr, img, width, height, f.half_k);
	if (img_pad == NULL)
	{
		free(f.kernel);
		write(2,"pad failure\n", 12);
		return (-1);
	}
	apply_convo_filter(&f, img_pad, img, width, height);
	mlx_destroy_image(mlx_ptr, img_pad->img_ptr);
	free_kernel(f.kernel, kern_size);
	free(img_pad);
	return (0);
}
