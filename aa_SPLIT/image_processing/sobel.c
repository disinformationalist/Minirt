#include "image_processing.h"

//doesnt work yet must do further researching.

void	set_sobel_kernel(t_filter *f, int kern_size)
{
	int i;
	int j;
	int center;
	//int weight;
	int x;
	int y;

	i = -1;
	center = kern_size / 2;
	while (++i < kern_size)
	{
		//weight = center - abs(i - center);
		//weight = abs(center - i);
		j = -1;
		while (++j < kern_size)
		{
			x = j - center;
			y = i - center;
			//f->kernel[i][j] = (j - center) * weight;//i -center for other dir.
			//f->kernel[i][j] *= weight;
			f->kernel[i][j] = x * (center - abs(y));
			}
	}
	f->kernel[0][0] = -1;
	f->kernel[0][1] = 0;
	f->kernel[0][2] = 1;
	f->kernel[1][0] = -2;
	f->kernel[1][1] = -10;
	f->kernel[1][2] = 2;
	f->kernel[2][0] = -1;
	f->kernel[2][1] = 10;
	f->kernel[2][2] = 1;
}		

int	make_sobel(t_filter *f, int kern_size)//generic make filter here. take function for set or not maybe param diffs...
{
	if (malloc_kernel(f, kern_size) == -1)
		return (-1);
	set_sobel_kernel(f, kern_size);
	print_matrix(f->kernel, kern_size, kern_size);
	printf("Sum of kernel values: %f\n", f->kern_sum);
	return (0);
}

t_img *sobel(void *mlx_ptr, t_img *img, int kern_size, int width, int height)
{
	t_filter	f;
	t_img		*img_pad;
	t_img		*img_out;

	//if (kern_size != 3 && kern_size != 5 && kern_size != 7)
	//	return (NULL);
	if (make_sobel(&f, kern_size) == -1)//only diff line from gauss? and params, make below general func
		return (NULL);
	f.half_k = kern_size / 2;
	img_pad = pad_zeros(mlx_ptr, img, width, height, f.half_k);//or pad color with option reusable function? or reflect padding? make other pads..
	if (img_pad == NULL)
	{
		free(f.kernel);
		write(2,"pad_zeros failure\n", 18);
		return (NULL);
	}
	img_out = create_img(mlx_ptr, width, height);
	if (img_out == NULL)
	{
		free(f.kernel);
		mlx_destroy_image(mlx_ptr, img_pad->img_ptr);
		free(img_pad);
		return (NULL);
	}
	apply_convo_filter(&f, img_pad, img_out, width, height);
	mlx_destroy_image(mlx_ptr, img_pad->img_ptr);
	free_kernel(f.kernel, kern_size);
	free(img_pad);
	return (img_out);
}
