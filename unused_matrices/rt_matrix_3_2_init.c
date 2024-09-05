/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_matrix_3_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:33:56 by arybarsk          #+#    #+#             */
/*   Updated: 2024/08/08 17:33:58 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_matrix_3(double matrix[3][3])
{
	int	i;
	int	j;

	i = -0;
	j = 0;
	while (i < 3)
	{
		while (j < 3)
		{
			matrix[i][j] = 0.0;
			j++;
		}
		j = 0;
		i++;
	}
}

void	init_matrix_2(double matrix[2][2])
{
	int	i;
	int	j;

	i = -0;
	j = 0;
	while (i < 2)
	{
		while (j < 2)
		{
			matrix[i][j] = 0.0;
			j++;
		}
		j = 0;
		i++;
	}
}
