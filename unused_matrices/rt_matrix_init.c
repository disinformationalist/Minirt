/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_matrix.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 15:32:47 by arybarsk          #+#    #+#             */
/*   Updated: 2024/08/08 15:32:49 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_zero_matrix(t_matrix_4x4 *matrix)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < MTRX_SIZE)
	{
		while (j < MTRX_SIZE)
		{
			matrix->m[i][j] = 0.0;
			j++;
		}
		j = 0;
		i++;
	}
}

void	fill_matrix(t_matrix_4x4 *matrix, int num)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < MTRX_SIZE)
	{
		while (j < MTRX_SIZE)
		{
			matrix->m[i][j] = num;
			j++;
		}
		j = 0;
		i++;
	}
}

void	init_identity_matrix(t_matrix_4x4 *matrix)
{
	int	i;
	int	j;

	i = 0;
	while (i < MTRX_SIZE)
	{
		j = 0;
		while (j < MTRX_SIZE)
		{
			if (i == j)
				matrix->m[i][j] = 1.0;
			else
				matrix->m[i][j] = 0.0;
			j++;
		}
		i++;
	}
}

void	make_matrix(t_matrix_4x4 *matrix)
{
	int	i;
	int	j;
	double	num;

	i = 0;
	j = 0;
	num = 0;
	while (i < MTRX_SIZE)
	{
		while (j < MTRX_SIZE)
		{
			matrix->m[i][j] = num++;
			j++;
		}
		j = 0;
		i++;
	}
}

int	are_matrices_equal(t_matrix_4x4 *matrix_1, t_matrix_4x4 *matrix_2)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < MTRX_SIZE)
	{
		while (j < MTRX_SIZE)
		{
			if (matrix_1->m[i][j] != matrix_2->m[i][j])
				return (0);
			j++;
		}
		j = 0;
		i++;
	}
	return (1);
}
