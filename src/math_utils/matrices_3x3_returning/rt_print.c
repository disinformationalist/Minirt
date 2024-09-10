/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 18:06:21 by arybarsk          #+#    #+#             */
/*   Updated: 2024/08/08 18:06:23 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	print_matrix_4(t_matrix_4x4 *matrix)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < 4)
	{
		while (j < 4)
		{
			printf("%f, ", matrix->m[i][j]);
			j++;
		}
		printf("\n");
		j = 0;
		i++;
	}
	printf("\n");
}

void	print_matrix_3(t_matrix_3x3 *matrix)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < 3)
	{
		while (j < 3)
		{
			printf("%f, ", matrix->m[i][j]);
			j++;
		}
		printf("\n");
		j = 0;
		i++;
	}
	printf("\n");
}

void	print_matrix_2(t_matrix_2x2 *matrix)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < 2)
	{
		while (j < 2)
		{
			printf("%f, ", matrix->m[i][j]);
			j++;
		}
		printf("\n");
		j = 0;
		i++;
	}
	printf("\n");
}

void	print_tuple(t_tuple tuple)
{
	printf("Tuple: (%f, %f, %f, %f)", tuple.x, tuple.y, tuple.z, tuple.w);
	printf("\n");
}

void	print_vec(t_vec3 vec)
{
	printf("vec: (%f, %f, %f)", vec.x, vec.y, vec.z);
	printf("\n");
}
