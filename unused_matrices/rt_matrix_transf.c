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

#include "rt.h"

void	translation_matrix(t_matrix_4x4 *matrix, double x, double y, double z)
{
	init_identity_matrix(matrix);
	matrix->m[0][3] = x;
	matrix->m[1][3] = y;
	matrix->m[2][3] = z;
}

void	scaling_matrix(t_matrix_4x4 *matrix, double x, double y, double z)
{
	init_zero_matrix(matrix);
	matrix->m[0][0] = x;
	matrix->m[1][1] = y;
	matrix->m[2][2] = z;
	matrix->m[3][3] = 1;
}

void	inverse_scaling_matrix(t_matrix_4x4 *matrix, double x, double y, double z)
{
	init_zero_matrix(matrix);
	matrix->m[0][0] = 1.0 / x;
	matrix->m[1][1] = 1.0 / y;
	matrix->m[2][2] = 1.0 / z;
	matrix->m[3][3] = 1;
}

void	rotation_x_axis_matrix(t_matrix_4x4 *matrix, double radians)
{
	init_identity_matrix(matrix);
	matrix->m[1][1] = cos(radians);
	matrix->m[2][1] = sin(radians);
	matrix->m[1][2] = -sin(radians);
	matrix->m[2][2] = cos(radians);
}

void	rotation_y_axis_matrix(t_matrix_4x4 *matrix, double radians)
{
	init_identity_matrix(matrix);
	matrix->m[0][0] = cos(radians);
	matrix->m[0][2] = sin(radians);
	matrix->m[2][0] = -sin(radians);
	matrix->m[2][2] = cos(radians);
}

void	rotation_z_axis_matrix(t_matrix_4x4 *matrix, double radians)
{
	init_identity_matrix(matrix);
	matrix->m[0][0] = cos(radians);
	matrix->m[1][0] = -sin(radians);
	matrix->m[0][1] = sin(radians);
	matrix->m[1][1] = cos(radians);
}

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
