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

double	cofact_3(t_matrix_3x3 mat, int row, int column)
{
	double	minor;
	double	cofactor;

	minor = min_3(mat, row, column);
	if ((row + column) % 2 == 1)
		cofactor = minor * (-1);
	else
		cofactor = minor;
	return (cofactor);
}

double	det_3(t_matrix_3x3 mat)
{
	double	det;

	det = mat.m[0][0] * cofact_3(mat, 0, 0)
		+ mat.m[0][1] * cofact_3(mat, 0, 1)
		+ mat.m[0][2] * cofact_3(mat, 0, 2);
	return (det);
}

double	cofact_4(t_matrix_4x4 matrix, int row, int column)
{
	t_matrix_3x3	new;
	double			minor;
	double			cofactor;

	new = submat_4(matrix, row, column);
	minor = det_3(new);
	if ((row + column) % 2 == 1)
		cofactor = minor * (-1);
	else
		cofactor = minor;
	return (cofactor);
}

//get determinate 4x4

double	det_4(t_matrix_4x4 matrix)
{
	double	det;
	int		i;
	double	cofactor;

	det = 0.0;
	i = -1;
	while (++i < 4)
	{
		cofactor = cofact_4(matrix, 0, i);
		det += matrix.m[0][i] * cofactor;
	}
	return (det);
}

t_matrix_4x4	inverse(t_matrix_4x4 matrix)
{
	int				i;
	int				j;
	double			cofactor;
	double			det;
	t_matrix_4x4 	inv;

	det = det_4(matrix);
	if (fabs(det) < 1e-9)//check for non invertable, limit for precision errors
		return (identity(&inv), inv);// should i do something else here..
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			cofactor = cofact_4(matrix, i, j);
			inv.m[j][i] = cofactor / det;
		}
	}
	return (inv);
}