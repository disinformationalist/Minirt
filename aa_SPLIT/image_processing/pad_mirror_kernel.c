#include "image_processing.h"

static inline void top_and_bottom_k(double **kernel, double ***mirror, int width, int height, int layers)
{
	int				x;
	int				y;

	y = -1;
	while (++y < layers)
	{
		x = -1;
		while (++x < width)
		{
			//top correct
			(*mirror)[x + layers][y] = kernel[x][layers - y];
			//bottom
			(*mirror)[x + layers][height + layers + y] = kernel[x][height - (y + 2)];
		}
	}
}

static inline void right_and_left_k(double **kernel, double ***mirror, int width, int height, int layers)
{
	int				x;
	int				y;

	y = -1;
	while (++y < height)
	{
		x = -1;
		while (++x < layers)
		{
			//left s
			(*mirror)[x][y + layers] = kernel[layers - x][y];
			//right s
			(*mirror)[width + layers + x][y + layers] = kernel[width - (x + 2)][y];
		}
	}
}

static inline void corners_k(double **kernel, double ***mirror, int width, int height, int layers)
{
	int				x;
	int				y;

	y = -1;
	while (++y < layers)
	{
		x = -1;
		while (++x < layers)
		{
			// top left
			(*mirror)[x][y] = kernel[layers - x][layers - y];
			//top right 
			(*mirror)[width + layers + x][y] = kernel[width - (x + 2)][layers - y];
			//bot left
			(*mirror)[x][height + layers + y] = kernel[layers - x][height - (y + 2)];
			// bot right
			(*mirror)[width + layers + x][height + layers + y] = kernel[width - (x + 2)][height - (y + 2)];
		}
	}
}

void copy_center_k(double **kernel, double ***mirror, int width, int height, int layers)
{
	int				x;
	int				y;

	y = layers - 1;
	while (++y < height + layers)
	{
		x = layers - 1;
		while (++x < width + layers)
			(*mirror)[x][y] = kernel[x - layers][ y - layers];
	}
}

double	**pad_mirror_kernel(t_filter *f, int width, int height, int layers)
{
	double		**mirror_kernel;

	/* width += 2 * layers;
	height += 2 * layers; */
	
	mirror_kernel = malloc_matrix(height + layers * 2, width + layers * 2);
	if (!mirror_kernel)
		return (NULL);
	copy_center_k(f->kernel, &mirror_kernel, width, height, layers);
	top_and_bottom_k(f->kernel, &mirror_kernel, width, height, layers);
	right_and_left_k(f->kernel, &mirror_kernel, width, height, layers);
	corners_k(f->kernel, &mirror_kernel, width, height, layers);
	return (mirror_kernel);
}
