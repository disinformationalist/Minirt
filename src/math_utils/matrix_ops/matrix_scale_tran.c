/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_scale_tran.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:12:28 by arybarsk          #+#    #+#             */
/*   Updated: 2024/12/16 13:12:31 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline void	identity2(t_matrix_4x4 *matrix)
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

static inline void	zero(t_matrix_4x4 *matrix)
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

t_matrix_4x4	translation(double x, double y, double z)
{
	t_matrix_4x4	matrix;

	identity2(&matrix);
	matrix.m[0][3] = x;
	matrix.m[1][3] = y;
	matrix.m[2][3] = z;
	return (matrix);
}

t_matrix_4x4	scaling(double x, double y, double z)
{
	t_matrix_4x4	matrix;

	zero(&matrix);
	matrix.m[0][0] = x;
	matrix.m[1][1] = y;
	matrix.m[2][2] = z;
	matrix.m[3][3] = 1;
	return (matrix);
}

t_matrix_4x4	inv_scaling(double x, double y, double z)
{
	t_matrix_4x4	matrix;

	zero(&matrix);
	matrix.m[0][0] = 1.0 / x;
	matrix.m[1][1] = 1.0 / y;
	matrix.m[2][2] = 1.0 / z;
	matrix.m[3][3] = 1;
	return (matrix);
}
