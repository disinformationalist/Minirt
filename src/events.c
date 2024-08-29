#include "minirt.h"

int	close_win(t_trace *trace)//valgrind error when using the x to close window, escape key gives no such error...
{	
	pthread_mutex_destroy(&trace->mutex);
	free_all_objects(trace);
	mlx_destroy_image(trace->mlx_connect, trace->img.img_ptr);
	mlx_destroy_window(trace->mlx_connect, trace->mlx_win);
	mlx_destroy_display(trace->mlx_connect);
	free(trace->mlx_connect);
	free(trace->threads);
	exit(EXIT_SUCCESS);
	return (0);
}

int	supersample_handle(int keycode, t_trace *trace)
{
	if (keycode == SPACE)
	{
		if (trace->supersample)
		{
			trace->width = trace->width_orig ;
			trace->height = trace->height_orig;
			trace->move_x = trace->move_x * trace->s_kernel;
			trace->move_y = trace->move_y * trace->s_kernel;
			trace->move_z = trace->move_y * trace->s_kernel;

			init_viewing(trace);
		}
		else
		{
			trace->width = trace->width * trace->s_kernel;
			trace->height = trace->height * trace->s_kernel;
			trace->move_x = trace->move_x / trace->s_kernel;
			trace->move_y = trace->move_y / trace->s_kernel;
			trace->move_z = trace->move_y * trace->s_kernel;
			
			init_viewing(trace);
		}
		trace->supersample = !trace->supersample;
	}
	return (0);
}

int	key_press_2(int keycode, t_trace *trace)
{
	if (keycode == XK_Left)
		trace->move_x = -1;
	else if (keycode == XK_Right)
		trace->move_x = 1;
	else if (keycode == XK_Up)
		trace->move_y = -1;
	else if (keycode == XK_Down)
		trace->move_y = 1;
	else
		supersample_handle(keycode, trace);
	render(trace);
	return (0);
}

int	key_press(int keycode, t_trace *trace)
{
	if (keycode == XK_Escape)
		close_win(trace);
	else if (keycode == UP_CARET)
		trace->layer = !trace->layer;
	else if (keycode == F3)
	{
		char *name;

		name = get_nxt_name("scene_");
		if (!name)
			clear_all(trace);
		if (export_png(name, &trace->img, trace->width_orig, trace->height_orig, NULL) == -1)
			close_win(trace);
		ft_putstr_color_fd(1, "EXPORT COMPLETE\n", BOLD_BRIGHT_BLUE);
		if (name)
			free(name);
	}
	else
		key_press_2(keycode, trace);
	return (0);
}