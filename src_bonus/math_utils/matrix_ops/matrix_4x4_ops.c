/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_4x4_ops.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:15:23 by arybarsk          #+#    #+#             */
/*   Updated: 2024/12/16 13:15:25 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline double	dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

static inline t_vec3	row_to_vec4(t_matrix_4x4 matrix, int row)
{
	t_vec3	vec;

	vec.x = matrix.m[row][0];
	vec.y = matrix.m[row][1];
	vec.z = matrix.m[row][2];
	vec.w = matrix.m[row][3];
	return (vec);
}

static inline t_vec3	col_to_vec4(t_matrix_4x4 matrix, int column)
{
	t_vec3	vec;

	vec.x = matrix.m[0][column];
	vec.y = matrix.m[1][column];
	vec.z = matrix.m[2][column];
	vec.w = matrix.m[3][column];
	return (vec);
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
