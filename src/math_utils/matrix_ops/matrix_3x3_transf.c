/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_matrix_transf.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 18:58:22 by arybarsk          #+#    #+#             */
/*   Updated: 2024/08/08 18:58:28 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//--------------------new------------------------------------
double	dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
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

t_vec3	row_to_vec4(t_matrix_4x4 matrix, int row)
{
	t_vec3	vec;

	vec.x = matrix.m[row][0];
	vec.y = matrix.m[row][1];
	vec.z = matrix.m[row][2];
	vec.w = matrix.m[row][3];
	return (vec);
}

t_vec3	col_to_vec4(t_matrix_4x4 matrix, int column)
{
	t_vec3	vec;

	vec.x = matrix.m[0][column];
	vec.y = matrix.m[1][column];
	vec.z = matrix.m[2][column];
	vec.w = matrix.m[3][column];
	return (vec);
}

t_matrix_4x4	translation(double x, double y, double z)
{
	t_matrix_4x4	matrix;
	
	identity(&matrix);
	matrix.m[0][3] = x;
	matrix.m[1][3] = y;
	matrix.m[2][3] = z;
	return (matrix);
}

t_vec3	mat_vec_mult(t_matrix_4x4 matrix, t_vec3 vec)
{
	t_vec3		new_vec;

	new_vec.x = dot(row_to_vec4(matrix, 0), vec);
	new_vec.y = dot(row_to_vec4(matrix, 1), vec);
	new_vec.z = dot(row_to_vec4(matrix, 2), vec);
	new_vec.w = dot(row_to_vec4(matrix, 3), vec);
	return (new_vec);
}

t_matrix_4x4	mat_mult(t_matrix_4x4 m_1, t_matrix_4x4 m_2)
{
	t_matrix_4x4	res;
	t_vec3			row;
	t_vec3			column;
	int				i;
	int				j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			row = row_to_vec4(m_1, i);
			column = col_to_vec4(m_2, j);
			res.m[i][j] = dot(row, column);
		}
	}
	return (res);
}

void	zero(t_matrix_4x4 *matrix)
{
	int	i;
	int	j;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			matrix->m[i][j] = 0.0;
	}
}

t_matrix_4x4	scaling(double x, double y, double z)
{
	t_matrix_4x4 matrix;

	zero(&matrix);
	matrix.m[0][0] = x;
	matrix.m[1][1] = y;
	matrix.m[2][2] = z;
	matrix.m[3][3] = 1;
	return (matrix);
}

t_matrix_4x4	inv_scaling(double x, double y, double z)
{
	t_matrix_4x4 matrix;
	
	zero(&matrix);
	matrix.m[0][0] = 1.0 / x;
	matrix.m[1][1] = 1.0 / y;
	matrix.m[2][2] = 1.0 / z;
	matrix.m[3][3] = 1;
	return (matrix);
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

void	tuple_to_row(t_matrix_4x4 *matrix, t_vec3 tuple, int row)
{
	matrix->m[row][0] = tuple.x;
	matrix->m[row][1] = tuple.y;
	matrix->m[row][2] = tuple.z;
	matrix->m[row][3] = tuple.w;
}

//using right handed here

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

//-----------------------------------------end new----------------------


t_matrix_3x3	translation_matrix_3x3(double x, double y)
{
	t_matrix_3x3	matrix;

	init_identity_matrix_3x3(&matrix);
	matrix.m[0][2] = x;
	matrix.m[1][2] = y;
	return (matrix);
}


t_matrix_3x3	inverse_scaling_matrix_3x3(double x, double y)
{
	t_matrix_3x3	matrix;
	init_identity_matrix_3x3(&matrix);
	if (x == 0.0 || y == 0.0)
	{
		//TODO: complain, maybe exit
		return (matrix);
	}
	init_zero_matrix_3x3(&matrix);
	matrix.m[0][0] = 1.0 / x;
	matrix.m[1][1] = 1.0 / y;
	matrix.m[2][2] = 1;
	return (matrix);
}


t_matrix_3x3	scaling_matrix_3x3(double x, double y)
{
	t_matrix_3x3	matrix;
	init_zero_matrix_3x3(&matrix);
	matrix.m[0][0] = x;
	matrix.m[1][1] = y;
	matrix.m[2][2] = 1;
	return (matrix);
}

t_matrix_3x3	rotation_x_axis_matrix_3x3(double radians)
{
	t_matrix_3x3	matrix;
	init_identity_matrix_3x3(&matrix);
	matrix.m[1][1] = cos(radians);
	matrix.m[2][1] = sin(radians);
	matrix.m[1][2] = -sin(radians);
	matrix.m[2][2] = cos(radians);
	return (matrix);
}

t_matrix_3x3	rotation_y_axis_matrix_3x3(double radians)
{
	t_matrix_3x3	matrix;
	init_identity_matrix_3x3(&matrix);
	matrix.m[0][0] = cos(radians);
	matrix.m[0][2] = sin(radians);
	matrix.m[2][0] = -sin(radians);
	matrix.m[2][2] = cos(radians);
	return (matrix);
}

t_matrix_3x3	rotation_z_axis_matrix_3x3(double radians)
{
	t_matrix_3x3	matrix;
	init_identity_matrix_3x3(&matrix);
	matrix.m[0][0] = cos(radians);
	matrix.m[1][0] = -sin(radians);
	matrix.m[0][1] = sin(radians);
	matrix.m[1][1] = cos(radians);
	return (matrix);
}
