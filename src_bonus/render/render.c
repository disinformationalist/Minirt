#include "minirt.h"

static inline void	free_all(t_trace *trace, \
	t_piece piece[][trace->num_cols], int i, int j)
{
	free_closests(trace, piece, i, j);
	clear_all(trace);
}

void	render_scene_s(t_trace *trace)
{
	t_piece	piece[trace->num_rows][trace->num_cols];
	int		i;
	int		j;

	i = -1;
	while (++i < trace->num_rows)
	{
		j = -1;
		while (++j < trace->num_cols)
		{
			if (set_pieces(trace, piece, i, j))
				free_all(trace, piece, i, j);
			if (pthread_create(&trace->threads[i * trace->num_cols + j], \
				NULL, ray_trace_s, (void *)&piece[i][j]) != 0)
			{
				free_closests(trace, piece, i, j);
				thread_error(trace, i * trace->num_cols + j);
				return ;
			}
		}
	}
	join_threads(trace);
	free_closests(trace, piece, trace->num_rows - 1, trace->num_cols);
}

/* render_scene starts by breaking up the screen into groupss of pixels and 
assigns the limits of each part to a t_peice, each to be worked by a thread */

void	render_scene(t_trace *trace)
{
	t_piece	piece[trace->num_rows][trace->num_cols];
	int		i;
	int		j;

	i = -1;
	while (++i < trace->num_rows)
	{
		j = -1;
		while (++j < trace->num_cols)
		{
			if (set_pieces(trace, piece, i, j))
				free_all(trace, piece, i, j);
			if (pthread_create(&trace->threads[i * trace->num_cols + j], \
				NULL, ray_trace, (void *)&piece[i][j]) != 0)
			{
				free_closests(trace, piece, i, j);
				thread_error(trace, i * trace->num_cols + j);
				return ;
			}
		}
	}
	join_threads(trace);
	free_closests(trace, piece, trace->num_rows - 1, trace->num_cols);
}

void	render(t_trace *trace)
{
	long	start;

	start = get_time();
	if (trace->supersample)
	{
		ft_putstr_color_fd(1, "SUPERSAMPLE IN PROGRESS...\n", BOLD_GREEN);
		render_scene_s(trace);
		ft_putstr_color_fd(1, "SUPERSAMPLE COMPLETE\n", BOLD_BRIGHT_GREEN);
	}
	else
		render_scene(trace);
	mlx_put_image_to_window(trace->mlx_connect,
		trace->mlx_win, trace->img.img_ptr, 0, 0);
	print_times(start, get_time(), "RENDER TIME: %f seconds\n");
}
