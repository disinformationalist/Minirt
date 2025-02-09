#include "minirt.h"

void	thread_error(t_trace *trace, int i)
{
	while (--i >= 0)
	{
		if (trace->threads[i])
			pthread_join(trace->threads[i], NULL);
	}
	clear_all(trace);
}

t_intersects	*create_ints(int total)
{
	t_intersects	*ints;

	if (total == 0)
		total += 2;
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

//menu 400 by 570, set threads to render around the menu and in low res mode

int	set_pieces_menu(t_trace *trace, t_piece piece[][trace->num_cols])
{
	int total_pix;
	int ppt;
	int small_len;
	int small_height;
	int short_sets;
	int top_bridge_h;
	int bot_bridge_h;
	int i;
	int h_left;
	int heights;

	total_pix = trace->height * trace->width - 228000;
	ppt = total_pix / trace->num_rows;
	small_len = trace->width - 400;
	small_height = ppt / small_len;
	short_sets = 570 / small_height;
	i = -1;
	while (++i < short_sets)
	{
		piece[i][0].x_s = 400;
		piece[i][0].x_e = trace->width;
		piece[i][0].y_s = small_height * i;
		piece[i][0].y_e = small_height * (i + 1);
		piece[i][0].trace = trace;
		piece[i][0].intersects = create_ints(trace->total_ints);
		if (!piece[i][0].intersects)
			return (1);	
		piece[i][0].bridge = false;
	}
	top_bridge_h = 570 % small_height;
	bot_bridge_h = (ppt - top_bridge_h * small_len) / trace->width;
	piece[i][0].x_s = 400;
	piece[i][0].x_e = trace->width;
	piece[i][0].y_s = small_height * i;
	piece[i][0].y_e = 570;
	piece[i][0].x_s2 = 0;
	piece[i][0].x_e2 = trace->width;
	piece[i][0].y_s2 = 570;
	if (i == trace->num_rows - 1)
		piece[i][0].y_e2 = trace->height;
	else
		piece[i][0].y_e2 = 570 + bot_bridge_h;
	piece[i][0].trace = trace;
	piece[i][0].intersects = create_ints(trace->total_ints);
	if (!piece[i][0].intersects)
		return (1);	
	piece[i][0].bridge = true;
	h_left = trace->height - (short_sets * small_height + top_bridge_h + bot_bridge_h);
	if (trace->num_rows - short_sets - 1 <= 0)
		return (0);
	heights = h_left / (trace->num_rows - short_sets - 1);
	while (++i < trace->num_rows)
	{
		piece[i][0].x_s = 0;
		piece[i][0].x_e = trace->width;
		if (piece[i - 1][0].bridge)
			piece[i][0].y_s = piece[i - 1][0].y_e2;
		else
			piece[i][0].y_s = piece[i - 1][0].y_e;
		if (i == trace->num_rows - 1)
			piece[i][0].y_e = trace->height;
		else
			piece[i][0].y_e = piece[i][0].y_s + heights;
		piece[i][0].trace = trace;
		piece[i][0].intersects = create_ints(trace->total_ints);
		if (!piece[i][0].intersects)
			return (1);	
		piece[i][0].bridge = false;
	}
	return (0);
}

//set pixel section limits for each thread and each gets a copy of trace struct

int	set_pieces(t_trace *trace, t_piece piece[][trace->num_cols], int i, int j)
{
	piece[i][j].x_s = j * (trace->width / trace->num_cols);
	if (j == trace->num_cols - 1)
		piece[i][j].x_e = trace->width;
	else
		piece[i][j].x_e = (j + 1) * (trace->width / trace->num_cols);
	piece[i][j].y_s = i * (trace->height / trace->num_rows);
	if (i == trace->num_rows - 1)
		piece[i][j].y_e = trace->height;
	else
		piece[i][j].y_e = (i + 1) * (trace->height / trace->num_rows);
	piece[i][j].trace = trace;
	piece[i][j].intersects = create_ints(trace->total_ints);
	if (!piece[i][j].intersects)
		return (1);	
	piece[i][j].bridge = false;
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

void	free_closests(t_trace *trace, \
		t_piece piece[][trace->num_cols], int i, int j)
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
