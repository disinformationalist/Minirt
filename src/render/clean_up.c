#include "minirt.h"

int	new_img_init(void *mlx_con, t_img *img, int width, int height)
{
	img->img_ptr = mlx_new_image(mlx_con,
			width, height);
	if (img->img_ptr == NULL)
		return (-1);
	img->pixels_ptr = mlx_get_data_addr(img->img_ptr,
			&img->bpp, &img->line_len,
			&img->endian);
	return (0);
}

void	my_pixel_put(int x, int y, t_img *img, unsigned int color)
{
	int	offset;

	offset = (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)(img->pixels_ptr + offset) = color;
}

void	clear_few(t_trace *trace)
{
	free_all_objects(trace);
	free(trace->threads);
	perror("Mlx init() failure\n");
	exit(EXIT_FAILURE);
}

void	clear_some(t_trace *trace)
{
	free_all_objects(trace);
	free(trace->mlx_connect);
	free(trace->threads);
	perror("Malloc or Thread error\n");
	exit(EXIT_FAILURE);
}

void	clear_all(t_trace *trace)
{
	free_all_objects(trace);
	if (trace->on)
		free(trace->on);
	if (trace->w_colors)
		free(trace->w_colors);
	if (trace->m_colors)
		free(trace->m_colors);
	if (trace->img.img_ptr)
		mlx_destroy_image(trace->mlx_connect, trace->img.img_ptr);
	mlx_destroy_window(trace->mlx_connect, trace->mlx_win);
	mlx_destroy_display(trace->mlx_connect);
	free(trace->mlx_connect);
	free(trace->threads);
	exit(EXIT_FAILURE);
}
