#include "minirt.h"

void	identity(t_matrix_4x4 *matrix)
{
	int	i;
	int	j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			if (i == j)
				matrix->m[i][j] = 1.0;
			else
				matrix->m[i][j] = 0.0;
		}
	}
}

t_matrix_4x4	transpose(t_matrix_4x4 matrix)
{
	t_matrix_4x4	tp;
	int				i;
	int				j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			tp.m[j][i] = matrix.m[i][j];
	}
	return (tp);
}

t_matrix_4x4	rot_x(double radians)
{
	t_matrix_4x4	matrix;
	
	identity(&matrix);
	matrix.m[1][1] = cos(radians);
	matrix.m[2][1] = sin(radians);
	matrix.m[1][2] = -sin(radians);
	matrix.m[2][2] = cos(radians);
	return (matrix);
}

t_matrix_4x4	rot_y(double radians)
{
	t_matrix_4x4	matrix;

	identity(&matrix);
	matrix.m[0][0] = cos(radians);
	matrix.m[0][2] = sin(radians);
	matrix.m[2][0] = -sin(radians);
	matrix.m[2][2] = cos(radians);
	return (matrix);
}

t_matrix_4x4	rot_z(double radians)
{
	t_matrix_4x4	matrix;

	identity(&matrix);
	matrix.m[0][0] = cos(radians);
	matrix.m[1][0] = -sin(radians);
	matrix.m[0][1] = sin(radians);
	matrix.m[1][1] = cos(radians);
	return (matrix);
}
