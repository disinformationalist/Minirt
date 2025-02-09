#include "minirt.h"

void	set_thread_colors(t_trace *trace, t_piece piece[][trace->num_cols])
{
	t_norm_color	*struct_cols;
	t_norm_color	struct_col;
	int				i;
	int				j;
	unsigned int	thread_col;

	struct_cols = set_color_wheel(trace->num_cols * trace->num_rows, 1.0, 0.5, 140);
	i = -1;
	while (++i < trace->num_rows)
	{
		j = -1;
		while (++j < trace->num_cols)
		{
			struct_col = *(struct_cols + i * trace->num_cols + j);
			thread_col = (int)(struct_col.r * 255) << 16 | (int)(struct_col.g * 255) << 8 | (int)(struct_col.b * 255);
			piece[i][j].thread_color = thread_col;
		}
	}
	free(struct_cols);
}

void	print_thread(t_piece piece)
{
	printf("----------peice---------\n");
	printf("xs: %d\n", piece.x_s);
	printf("xe: %d\n", piece.x_e);
	printf("ys: %d\n", piece.y_s);
	printf("ye: %d\n", piece.y_e);
	if (piece.bridge)
	{
		printf("xs2: %d\n", piece.x_s2);
		printf("xe2: %d\n", piece.x_e2);
		printf("ys2: %d\n", piece.y_s2);
		printf("ye2: %d\n", piece.y_e2);
	}
}

void	print_threads(t_trace *trace, t_piece piece[][trace->num_cols])
{
	int i = -1;
	while (++i < trace->num_rows)
	{
		int j = -1;
		while (++j < trace->num_cols)
		{
			int k = i * trace->num_rows + j;
			printf("-----peice:--%d---------\n\n", k);
			printf("xs: %d\n", piece[i][j].x_s);
			printf("xe: %d\n", piece[i][j].x_e);
			printf("ys: %d\n", piece[i][j].y_s);
			printf("ye: %d\n", piece[i][j].y_e);
			if (piece[i][j].bridge)
			{
				printf("xs2: %d\n", piece[i][j].x_s2);
				printf("xe2: %d\n", piece[i][j].x_e2);
				printf("ys2: %d\n", piece[i][j].y_s2);
				printf("ye2: %d\n", piece[i][j].y_e2);
			}
			printf("\n");
		}
	}
}
