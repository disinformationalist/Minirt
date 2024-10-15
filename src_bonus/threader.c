#include "minirt.h"

int	get_num_cores(void)
{
	int	num_cores;

	num_cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (num_cores < 1)
	{
		perror("sysconf error\n");
		return (1);
	}
	return (num_cores);
}

void	thread_error(t_trace *trace, int i)
{
	while (--i >= 0)
	{
		if (trace->threads[i])
			pthread_join(trace->threads[i], NULL);
	}
	free(trace->w_colors);
	clear_all(trace);
}

t_intersects *create_ints(int total)
{
	t_intersects *ints;

	/* if (total == 0)
		total += 2; */
	ints = malloc(sizeof(t_intersects));
	if (!ints)
		return (NULL);
	ints->hits = malloc(total * sizeof(t_track_hits));
	if (!ints->hits)
	{
		free(ints);
		return (NULL);
	}
	ints->closest = (t_track_hits *)malloc(sizeof(t_track_hits));
	if (!ints->closest)
	{
		free(ints->hits);
		free(ints);
		return (NULL);
	}
	ints->count = 0;
	ints->size = total;
	return (ints);
}

//set pixel section limits for each thread and each gets a copy of trace struct

int	set_pieces(t_trace *trace, t_piece piece[][trace->num_cols], int i, int j)
{
	piece[i][j].x_s = j * (trace->width / trace->num_cols);
	piece[i][j].x_e = (j + 1) * (trace->width / trace->num_cols);
	piece[i][j].y_s = i * (trace->height / trace->num_rows);
	piece[i][j].y_e = (i + 1) * (trace->height / trace->num_rows);
	piece[i][j].trace = trace;
	/* piece[i][j].closest = (t_track_hits *)malloc(sizeof(t_track_hits));
	if (!piece[i][j].closest)
		return (1); */
	piece[i][j].intersects = create_ints(trace->total_ints);
	if (!piece[i][j].intersects)
	{
		//free(piece[i][j].closest);
		return (1);
	}
	return (0);
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

void	free_closests(t_trace *trace, t_piece piece[][trace->num_cols], int i, int j)
{
	while (--j >= 0)
	{
		if (piece[i][j].intersects->hits)
			free(piece[i][j].intersects->hits);
		if (piece[i][j].intersects->closest)
			free(piece[i][j].intersects->closest);
		if (piece[i][j].intersects)
			free(piece[i][j].intersects);
	}
	while (--i >= 0)
	{
		j = trace->num_cols;
		while (--j >= 0)
		{
			if (piece[i][j].intersects->hits)
				free(piece[i][j].intersects->hits);
			if (piece[i][j].intersects->closest)
				free(piece[i][j].intersects->closest);
			if (piece[i][j].intersects)
				free(piece[i][j].intersects);
		}
	}
}
