#include "minirt.h"

double	det_2(t_matrix_2x2 mat)
{
	double	det;

	det = mat.m[0][0] * mat.m[1][1] - mat.m[0][1] * mat.m[1][0];
	return (det);
}

t_matrix_3x3	submat_4(t_matrix_4x4 matrix, int row, int column)
{
	t_matrix_3x3	res;
	int				i;
	int 			j;
	int 			k;
	int 			l;
	
	k = 0;
	i = -1;
	while (++i < 4)
	{
		if (i != row)
		{
			l = 0;
			j = -1;
			while (++j < 4)
			{
				if (j != column)
					res.m[k][l++] = matrix.m[i][j];
			}
			k++;
		}
	}
	return (res);
}

t_matrix_2x2	submat_3(t_matrix_3x3 mat, int row, int column)
{
	t_matrix_2x2	res;
	int				i;
	int				j;
	int				k;
	int				l;

	k = 0;
	i = -1;
	while (++i < 3)
	{
		if (i != row)
		{
			l = 0;
			j = -1;
			while (++j < 3)
			{
				if (j != column)
					res.m[k][l++] = mat.m[i][j];
			}
			k++;
		}
	}
	return (res);
}

double	min_3(t_matrix_3x3 mat, int row, int column)
{
	t_matrix_2x2	new;

	new = submat_3(mat, row, column);
	return (det_2(new));
}

void	tuple_to_col(t_matrix_4x4 *matrix, t_vec3 tuple, int col)
{
	matrix->m[0][col] = tuple.x;
	matrix->m[1][col] = tuple.y;
	matrix->m[2][col] = tuple.z;
	matrix->m[3][col] = tuple.w;
}