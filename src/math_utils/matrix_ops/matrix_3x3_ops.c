/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_matrix_ops.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:32:56 by arybarsk          #+#    #+#             */
/*   Updated: 2024/08/27 18:32:59 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_matrix_3x3	multiply_3x3_matrices(t_matrix_3x3 *matrix_1, t_matrix_3x3 *matrix_2)
{
	t_matrix_3x3	new_matrix;
	int		i;
	int		j;
	t_vec3	row;
	t_vec3	column;

	i = 0;
	j = 0;
	while (i < 3)
	{
		while (j < 3)
		{
			row = row_to_vec(matrix_1, i);
			column = column_to_vec(matrix_2, j);
			new_matrix.m[i][j] = dot_product(row, column);
			j++;
		}
		j = 0;
		i++;
	}
	return (new_matrix);
}

t_vec3	multiply_3x3_matrix_by_vec(t_matrix_3x3 *matrix, t_vec3 vec)
{
	t_vec3	new_vec;
	new_vec.x = dot_product(row_to_vec(matrix, 0), vec);
	new_vec.y = dot_product(row_to_vec(matrix, 1), vec);
	new_vec.z = dot_product(row_to_vec(matrix, 2), vec);
	return (new_vec);
}

t_matrix_3x3	transpose_matrix_3x3(t_matrix_3x3 *matrix)
{
	t_matrix_3x3	new_matrix;
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < 3)
	{
		while (j < 3)
		{
			new_matrix.m[j][i] = matrix->m[i][j];
			j++;
		}
		j = 0;
		i++;
	}
	return (new_matrix);
}

double	determinant_2(t_matrix_2x2 *matrix)
{
	double	determinant;

	determinant = matrix->m[0][0] * matrix->m[1][1] - matrix->m[0][1] * matrix->m[1][0];
	return (determinant);
}

t_matrix_2x2	submatrix_3(t_matrix_3x3 *matrix,
	int row, int column)
{
	t_matrix_2x2	new_matrix;
	int i;
	int j;
	int k;
	int l;

	i = 0;
	k = 0;
	while (i < 3)
	{
		if (i != row)
		{
			j = 0;
			l = 0;
			while (j < 3)
			{
				if (j != column)
				{
					new_matrix.m[k][l]
						= matrix->m[i][j];
					l++;
				}
				j++;
			}
			k++;
		}
		i++;
	}
	return (new_matrix);
}

double	minor_3(t_matrix_3x3 *matrix, int row, int column)
{
	t_matrix_2x2 new_matrix;

	new_matrix = submatrix_3(matrix, row, column);
	return (determinant_2(&new_matrix));
}

double	cofactor_3(t_matrix_3x3 *matrix, int row, int column)
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

double	determinant_3(t_matrix_3x3 *matrix)
{
	double	determinant;

	determinant = matrix->m[0][0] * cofactor_3(matrix, 0, 0)
		+ matrix->m[0][1] * cofactor_3(matrix, 0, 1)
		+ matrix->m[0][2] * cofactor_3(matrix, 0, 2);
	return (determinant);
}

t_matrix_3x3	inverse_matrix(t_matrix_3x3 *matrix)
{
	t_matrix_3x3	new_matrix;
	int	i;
	int	j;
	double	cofactor;
	double	determinant;

	init_zero_matrix_3x3(&new_matrix);
	determinant = determinant_3(matrix);
	if (determinant == 0)
		return (new_matrix);
	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 3)
		{
			cofactor = cofactor_3(matrix, i, j);
			new_matrix.m[j][i] = cofactor / determinant;
			j++;
		}
		i++;
	}
	return (new_matrix);
}
