#include "minirt.h"

void	thread_error(t_trace *trace, int i)
{
	while (--i >= 0)
	{
		if (trace->threads[i])
			pthread_join(trace->threads[i], NULL);
	}
	//pthread_mutex_destroy(&trace->mutex);
	clear_all(trace);
}

void	set_pieces(t_trace *trace, t_piece piece[][trace->num_cols], int i, int j)
{
	piece[i][j].x_s = j * ((trace->width * trace->n) / trace->num_cols);// using n for other multiple rays per pixel method
	piece[i][j].x_e = (j + 1) * ((trace->width * trace->n) / trace->num_cols);
	piece[i][j].y_s = i * ((trace->height * trace->n) / trace->num_rows);
	piece[i][j].y_e = (i + 1) * ((trace->height * trace->n) / trace->num_rows);
	piece[i][j].trace = trace;
}

void	join_threads(t_trace *trace)
{
	int	i;
	int	j;

	i = -1;
	while (++i < trace->num_rows)
	{
		j = -1;
		while (++j < trace->num_cols)
			pthread_join(trace->threads[i * trace->num_cols + j], NULL);
	}
}

//render_scene starts by breaking up the screen into sections of pixels and assigns the limits of each section to a t_peice, each to be worked by a thread

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
			set_pieces(trace, piece, i, j);
			if (pthread_create(&trace->threads[i * trace->num_cols + j], NULL, ray_trace, (void *)&piece[i][j]) != 0)
				thread_error(trace, i * trace->num_cols + j);
		}
	}
	join_threads(trace);
}