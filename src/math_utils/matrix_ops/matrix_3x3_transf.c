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

t_matrix_3x3	translation_matrix_3x3(double x, double y)
{
	t_matrix_3x3	matrix;
	init_identity_matrix_3x3(&matrix);
	matrix.m[0][2] = x;
	matrix.m[1][2] = y;
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
