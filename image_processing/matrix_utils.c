#include "image_processing.h"

double	get_matrix_sum(double **matrix, int width, int height)
{
	int		i;
	int		j;
	double	sum;

	sum = 0;
	i = -1;
	while (++i < height)
	{
		j = -1;
		while (++j < width)
				sum += matrix[i][j];
	}
	return (sum);
}


double	**malloc_matrix(int width, int height)
{
	double	**matrix;
	int i;

	i = -1;
	matrix = (double **)malloc(height * sizeof(double *));
	if (!matrix)
		return (NULL);
	while (++i < height)
	{
		matrix[i] = (double *)malloc(width * sizeof(double));
		if (!matrix[i])
		{
			free_kernel(matrix, i);
			return (NULL);
		}
	}
	return (matrix);
}

unsigned char	**malloc_uc_matrix(int width, int height)
{
	unsigned char	**matrix;
	int i;

	i = -1;
	matrix = (unsigned char **)malloc(height * sizeof(unsigned char *));
	if (!matrix)
		return (NULL);
	while (++i < height)
	{
		matrix[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
		if (!matrix[i])
		{
			//free_kernel(matrix, i);
			return (NULL);
		}
	}
	return (matrix);
}

void	free_ui_matrix(unsigned int **matrix, int j)
{
	while (--j >= 0)
	{
		free (matrix[j]);
		matrix[j] = NULL;
	}
	free(matrix);
	matrix = NULL;
}

void	zero_ui_matrix(unsigned int **pixels_xl, int width, int height)
{
	int j;
	int i;
	
	j = -1;
	while (++j < height)
	{
		i = -1;
		while (++i < width)
			pixels_xl[j][i] = 0;
	}
}

unsigned int	**malloc_ui_matrix(int width, int height)
{
	unsigned int	**matrix;
	int i;

	i = -1;
	matrix = (unsigned int **)malloc(height * sizeof(unsigned int *));
	if (!matrix)
		return (NULL);
	while (++i < height)
	{
		matrix[i] = (unsigned int *)malloc(width * sizeof(unsigned int));
		if (!matrix[i])
		{
			free_ui_matrix(matrix, i);
			return (NULL);
		}
	}
	return (matrix);
}
