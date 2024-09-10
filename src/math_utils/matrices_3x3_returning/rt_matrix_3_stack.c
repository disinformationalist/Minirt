/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_matrix_3_stack.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:34:29 by arybarsk          #+#    #+#             */
/*   Updated: 2024/09/09 18:34:31 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	push_mtrx_on_stack(t_matrix_3x3 *matrix, t_matrix_3x3 *matrix_stack, int *end_of_stack)
{
	if (*end_of_stack < MTRX_STACK_SIZE - 1)
	{
		(*end_of_stack)++;
		ft_memcpy(&matrix_stack[*end_of_stack], matrix, sizeof(t_matrix_3x3));
	}
}

void	pop_mtrx_from_stack(t_matrix_3x3 *matrix, t_matrix_3x3 *matrix_stack, int *end_of_stack)
{
	if (*end_of_stack >= 0)
	{
		ft_memcpy(matrix, &matrix_stack[*end_of_stack], sizeof(t_matrix_3x3));
		(*end_of_stack)--;
	}
}
