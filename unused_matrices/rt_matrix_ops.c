
#include "rt.h"

void	multiply_matrices(t_matrix_4x4 *matrix_1, t_matrix_4x4 *matrix_2,
	t_matrix_4x4 *new_matrix)
{
	int		i;
	int		j;
	t_tuple	row;
	t_tuple	column;

	i = 0;
	j = 0;
	while (i < 4)
	{
		while (j < 4)
		{
			row = row_to_tuple(matrix_1, i);
			column = column_to_tuple(matrix_2, j);
			new_matrix->m[i][j] = dot(row, column);
			j++;
		}
		j = 0;
		i++;
	}
}

void	multiply_matrix_by_tuple(t_matrix_4x4 *matrix, t_tuple tuple,
	t_tuple *new_tuple)
{
	new_tuple->x = dot(row_to_tuple(matrix, 0), tuple);
	new_tuple->y = dot(row_to_tuple(matrix, 1), tuple);
	new_tuple->z = dot(row_to_tuple(matrix, 2), tuple);
	new_tuple->w = dot(row_to_tuple(matrix, 3), tuple);
}

void	transform_matrix(t_matrix_4x4 *matrix, t_matrix_4x4 *new_matrix)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < 4)
	{
		while (j < 4)
		{
			new_matrix->m[j][i] = matrix->m[i][j];
			j++;
		}
		j = 0;
		i++;
	}
}

double	determinant_2(double matrix[2][2])
{
	double	determinant;

	determinant = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
	return (determinant);
}

//This is done with a tuple instead of 4 variables because of the f*ing Norm
void	submatrix_4(t_matrix_4x4 *matrix, double new_matrix[3][3],
	int row, int column)
{
	t_tuple	tuple;

	tuple.x = 0;
	tuple.z = 0;
	while (tuple.x < 4)
	{
		if (tuple.x != row)
		{
			tuple.y = 0;
			tuple.w = 0;
			while (tuple.y < 4)
			{
				if (tuple.y != column)
				{
					new_matrix[(int) tuple.z][(int) tuple.w]
						= matrix->m[(int) tuple.x][(int) tuple.y];
					tuple.w++;
				}
				tuple.y++;
			}
			tuple.z++;
		}
		tuple.x++;
	}
}

void	submatrix_3(double matrix[3][3], double new_matrix[2][2],
	int row, int column)
{
	t_tuple	tuple;

	tuple.x = 0;
	tuple.z = 0;
	while (tuple.x < 3)
	{
		if (tuple.x != row)
		{
			tuple.y = 0;
			tuple.w = 0;
			while (tuple.y < 3)
			{
				if (tuple.y != column)
				{
					new_matrix[(int) tuple.z][(int) tuple.w]
						= matrix[(int) tuple.x][(int) tuple.y];
					tuple.w++;
				}
				tuple.y++;
			}
			tuple.z++;
		}
		tuple.x++;
	}
}

double	minor_3(double matrix[3][3], int row, int column)
{
	double	new_matrix[2][2];

	submatrix_3(matrix, new_matrix, row, column);
	return (determinant_2(new_matrix));
}

double	cofactor_3(double matrix[3][3], int row, int column)
{
	double	minor;
	double	cofactor;

	minor = minor_3(matrix, row, column);
	if ((row + column) % 2 == 1)
		cofactor = minor * (-1);
	else
		cofactor = minor;
	return (cofactor);
}

double	determinant_3(double matrix[3][3])
{
	double	determinant;

	determinant = matrix[0][0] * cofactor_3(matrix, 0, 0)
		+ matrix[0][1] * cofactor_3(matrix, 0, 1)
		+ matrix[0][2] * cofactor_3(matrix, 0, 2);
	return (determinant);
}

double	cofactor_4(t_matrix_4x4 *matrix, int row, int column)
{
	double	new_matrix[3][3];
	double	minor;
	double	cofactor;

	submatrix_4(matrix, new_matrix, row, column);
	minor = determinant_3(new_matrix);
	if ((row + column) % 2 == 1)
		cofactor = minor * (-1);
	else
		cofactor = minor;
	return (cofactor);
}

double	determinant_4(t_matrix_4x4 *matrix)
{
	double	determinant;
	int	i;
	double 	new_matrix[3][3];
	double	cofactor;

	determinant = 0.0;
	i = 0;
	while (i < 4)
	{
		submatrix_4(matrix, new_matrix, 0, i);
		cofactor = cofactor_3(new_matrix, 0, i);
		determinant += matrix->m[0][i] * cofactor;
		i++;
	}
	return (determinant);
}

void	inverse_matrix(t_matrix_4x4 *matrix, t_matrix_4x4 *new_matrix)
{
	int	i;
	int	j;
	double	cofactor;
	double	determinant;

	determinant = determinant_4(matrix);
	if (determinant == 0)
		return ;
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			cofactor = cofactor_4(matrix, i, j);
			new_matrix->m[j][i] = cofactor / determinant;
			j++;
		}
		i++;
	}
}
