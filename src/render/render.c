#include "minirt.h"

static inline void	free_all(t_trace *trace, \
	t_piece piece[][trace->num_cols], int i, int j)
{
	free_closests(trace, piece, i, j);
	clear_all(trace);
}

//combine this options pass selection parameter or pass function pointer

void	render_scene_s(t_trace *trace)
{
	t_piece	piece[trace->num_rows][trace->num_cols];
	int		i;
	int		j;

	if (trace->menu_open)
		set_pieces_menu(trace, piece);
	i = -1;
	while (++i < trace->num_rows)
	{
		j = -1;
		while (++j < trace->num_cols)
		{
			if (!trace->menu_open)
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
//t_piece	piece[trace->num_rows][trace->num_cols];


void	render_scene(t_trace *trace)
{
	t_piece	piece[trace->num_rows][trace->num_cols];
	int		i;
	int		j;

	if (trace->menu_open)
		set_pieces_menu(trace, piece);
		//	print_threads(piece[i][j]);
	i = -1;
	while (++i < trace->num_rows)
	{
		j = -1;
		while (++j < trace->num_cols)
		{
			if (!trace->menu_open)
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

void	render_low_res(t_trace *trace)
{
	t_piece	piece[trace->num_rows][trace->num_cols];
	int		i;
	int		j;

	//set the threads around the menu
	if (trace->menu_open)
		set_pieces_menu(trace, piece);
	//set_thread_colors(trace, piece);//testing
	i = -1;
	while (++i < trace->num_rows)
	{
		j = -1;
		while (++j < trace->num_cols)
		{
			if (!trace->menu_open)
				if (set_pieces(trace, piece, i, j))
					free_all(trace, piece, i, j);
			if (pthread_create(&trace->threads[i * trace->num_cols + j], \
				NULL, ray_trace_l, (void *)&piece[i][j]) != 0)
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
	long	end;

	//cast_single_ray(trace, vec(0, 2, 0, 1), vec(0, 0, 1, 0));//for testing
	
	start = get_time();
	if (start == -1)
		clear_all(trace);
	if (trace->low_res)
		render_low_res(trace);
	else if (trace->supersample)
	{
		ft_putstr_color_fd(1, "SUPERSAMPLE IN PROGRESS...\n", BOLD_GREEN);
		render_scene_s(trace);
		ft_putstr_color_fd(1, "SUPERSAMPLE COMPLETE\n", BOLD_BRIGHT_GREEN);
	}
	else
		render_scene(trace); 
	mlx_put_image_to_window(trace->mlx_connect,
		trace->mlx_win, trace->img.img_ptr, 0, 0);
	end = get_time();
	if (end == -1)
		clear_all(trace);
	if (!trace->low_res)
		print_times(start, end, "RENDER TIME: %f seconds\n");
}
