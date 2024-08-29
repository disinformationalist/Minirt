#include "minirt.h"

void	clear_all(t_trace *trace)
{
	free_all_objects(trace);
	if (trace->closest)
		free(trace->closest);
	if (trace->img.img_ptr)
		mlx_destroy_image(trace->mlx_connect, trace->img.img_ptr);
	if (trace->mlx_win)
		mlx_destroy_window(trace->mlx_connect, trace->mlx_win);
	mlx_destroy_display(trace->mlx_connect);
	free(trace->mlx_connect);
	free(trace->threads);
	perror("Malloc or Thread error\n");
	exit(EXIT_FAILURE);
}

void	error_exit(char *msg)
{
	ft_putstr_color_fd(2, msg, RED);
	exit(EXIT_FAILURE);
}