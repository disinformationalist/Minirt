/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_matrix_tuple_conv.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 18:56:37 by arybarsk          #+#    #+#             */
/*   Updated: 2024/08/08 18:56:39 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_tuple	row_to_tuple(t_matrix_4x4 *matrix, int row)
{
	t_tuple	tuple;

	tuple.x = matrix->m[row][0];
	tuple.y = matrix->m[row][1];
	tuple.z = matrix->m[row][2];
	tuple.w = matrix->m[row][3];
	return (tuple);
}

t_tuple	column_to_tuple(t_matrix_4x4 *matrix, int column)
{
	t_tuple	tuple;

	tuple.x = matrix->m[0][column];
	tuple.y = matrix->m[1][column];
	tuple.z = matrix->m[2][column];
	tuple.w = matrix->m[3][column];
	return (tuple);
}

void	tuple_to_row(t_matrix_4x4 *matrix, t_tuple tuple, int row)
{
	matrix->m[row][0] = tuple.x;
	matrix->m[row][1] = tuple.y;
	matrix->m[row][2] = tuple.z;
	matrix->m[row][3] = tuple.w;
}

void	tuple_to_column(t_matrix_4x4 *matrix, t_tuple tuple, int column)
{
	matrix->m[0][column] = tuple.x;
	matrix->m[1][column] = tuple.y;
	matrix->m[2][column] = tuple.z;
	matrix->m[3][column] = tuple.w;
}
