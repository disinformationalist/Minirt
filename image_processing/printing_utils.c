#include "image_processing.h"

//try printing with color, higher to lower changing.

//prints the kernel for viewing / testing
static int	ft_len(int n)
{
	int len;
	
	len = 1;
	while (n / 10)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static void	print_loops(double **matrix, int rows, int cols, int *col_widths)
{
	int row;
	int col;
	int spaces;
	
	printf("2D convo Kernel:\n\n");
	row = -1;
    while (++row < rows)
	{
        col = -1;
		while (++col < cols)
        {
			if (col_widths[col] < 0 && matrix[col][row] >= 0)
				printf(" ");
			printf("%.1f", matrix[col][row]);
			if (col != cols - 1)
			{
				if (col_widths[col] < 0)
					spaces = -col_widths[col] - ft_len(matrix[col][row]) + 1;
				else
					spaces = col_widths[col] - ft_len(matrix[col][row]) + 1;
				while (--spaces >= 0)
					printf(" ");
			}
		}
		printf("\n");
    }
	printf("\n");
}

void	print_matrix(double **matrix, int cols, int rows)
{
	int col;
	int row;
	int biggest;
	int len;
	int col_widths[cols];
	int flag;

	flag = 0;
	col = -1;
    while (++col < cols)
	{
        row = -1;
		biggest = 0;
		while (++row < rows)
        {
			len = ft_len(matrix[col][row]);
			if (len > biggest)
				biggest = len;
			if (matrix[col][row] < 0)
				flag = 1;
		}
		if (flag)
			biggest = -biggest;
		col_widths[col] = biggest;
	}
	print_loops(matrix, rows, cols, col_widths);
}