/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:50:26 by arybarsk          #+#    #+#             */
/*   Updated: 2023/12/07 15:50:28 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	main(int argc, char **argv)
{
	int				end_of_stack;
	t_matrix_3x3	matrix_stack[MTRX_STACK_SIZE];
	t_matrix_3x3	matrix;
	t_matrix_3x3	matrix_b;
	t_matrix_3x3	matrix_c;
	t_matrix_3x3	new_matrix;
	t_matrix_2x2	matrix_2;

	end_of_stack = -1;
	init_zero_matrix_2x2(&matrix_2);
	init_zero_matrix_3x3(&matrix);
	init_zero_matrix_3x3(&matrix_b);
	init_zero_matrix_3x3(&matrix_c);
	init_zero_matrix_3x3(&new_matrix);
	
	matrix = translation_matrix_3x3(5.0, -3.0);
	push_mtrx_on_stack(&matrix, matrix_stack, &end_of_stack);
	matrix = scaling_matrix_3x3(2.0, 2.0);
	push_mtrx_on_stack(&matrix, matrix_stack, &end_of_stack);
	matrix = rotation_z_axis_matrix_3x3(M_PI / 4);
	push_mtrx_on_stack(&matrix, matrix_stack, &end_of_stack);
	
	pop_mtrx_from_stack(&matrix_b, matrix_stack, &end_of_stack);
	print_matrix_3(&matrix_b);
	pop_mtrx_from_stack(&matrix_c, matrix_stack, &end_of_stack);
	print_matrix_3(&matrix_c);
	new_matrix = multiply_3x3_matrices(&matrix_b, &matrix_c);
	print_matrix_3(&new_matrix);
}
