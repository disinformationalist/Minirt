/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:22:24 by arybarsk          #+#    #+#             */
/*   Updated: 2024/07/23 16:22:27 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_OPS_H
# define MATRIX_OPS_H

# define MTRX_SIZE 4
# define SIZE_3X3 3
# define SIZE_2X2 2

# define MTRX_STACK_SIZE 10

//#include "tools.h"

typedef struct s_matrix_2x2
{
	double		m[SIZE_2X2][SIZE_2X2];
}				t_matrix_2x2;

typedef struct s_matrix_3x3
{
	double		m[SIZE_3X3][SIZE_3X3];
}				t_matrix_3x3;

typedef struct s_matrix_4x4
{
	double		m[MTRX_SIZE][MTRX_SIZE];
}				t_matrix_4x4;

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
	double	w;
}	t_vec3;

typedef t_vec3 t_point;

//apply an objects transform matrix to ray


//----------------------------new------------------------
//all but inverse in matrix_3x3_transf.c

//tools
double			dot(t_vec3 a, t_vec3 b);
void			zero(t_matrix_4x4 *matrix);
t_vec3			row_to_vec4(t_matrix_4x4 matrix, int row);

t_matrix_4x4	inverse(t_matrix_4x4 matrix);//all stuff for this in inverse.c
t_matrix_4x4	transpose(t_matrix_4x4 matrix);

t_vec3			mat_vec_mult(t_matrix_4x4 matrix, t_vec3 vec);
t_matrix_4x4	mat_mult(t_matrix_4x4 m_1, t_matrix_4x4 m_2);

//move and scale
t_matrix_4x4	translation(double x, double y, double z);

t_matrix_4x4	scaling(double x, double y, double z);
t_matrix_4x4	inv_scaling(double x, double y, double z);

//rot
t_matrix_4x4	rot_x(double radians);
t_matrix_4x4	rot_y(double radians);
t_matrix_4x4	rot_z(double radians);

void			zero(t_matrix_4x4 *matrix);
void			identity(t_matrix_4x4 *matrix);

void			tuple_to_row(t_matrix_4x4 *matrix, t_vec3 tuple, int row);


//----------------------------end new------------------------



/* print */
void			print_matrix_4(t_matrix_4x4 *matrix);
void			print_matrix_3(t_matrix_3x3 *matrix);
void			print_matrix_2(t_matrix_2x2 *matrix);
void			print_vec(t_vec3 vec);
/* matrix */
void			fill_matrix_4(t_matrix_4x4 *matrix, double num);
void			make_matrix_4(t_matrix_4x4 *matrix);
void			init_matrix_3(double matrix[3][3]);
void			init_matrix_2(double matrix[2][2]);
int				are_matrices_equal(t_matrix_4x4 *matrix_1, t_matrix_4x4 *matrix_2);
/* matrix_2 */
void			init_zero_matrix_2x2(t_matrix_2x2 *matrix);
/* matrix_3_init */
void			init_zero_matrix_3x3(t_matrix_3x3 *matrix);
void			fill_matrix_3x3(t_matrix_3x3 *matrix, int num);
void			init_identity_matrix_3x3(t_matrix_3x3 *matrix);
void			make_incremental_matrix_3x3(t_matrix_3x3 *matrix);
int				are_matrices_3x3_equal(t_matrix_3x3 *matrix_1, t_matrix_3x3 *matrix_2);
//
void			init_identity_matrix(t_matrix_4x4 *matrix);

/* matrix_3_ops */




t_matrix_3x3	multiply_3x3_matrices(t_matrix_3x3 *matrix_1, t_matrix_3x3 *matrix_2);
t_vec3			multiply_3x3_matrix_by_vec(t_matrix_3x3 *matrix, t_vec3 vec);
t_matrix_3x3	transpose_matrix_3x3(t_matrix_3x3 *matrix);

double			determinant_2(t_matrix_2x2 *matrix);
t_matrix_2x2	submatrix_3(t_matrix_3x3 *matrix, int row, int column);
double			minor_3(t_matrix_3x3 *matrix, int row, int column);
double			cofactor_3(t_matrix_3x3 *matrix, int row, int column);
double			determinant_3(t_matrix_3x3 *matrix);
t_matrix_3x3	inverse_matrix(t_matrix_3x3 *matrix);

//translate, scale, rotate

t_matrix_3x3	translation_matrix_3x3(double x, double y);
t_matrix_3x3	scaling_matrix_3x3(double x, double y);
t_matrix_3x3	inverse_scaling_matrix_3x3(double x, double y);

t_matrix_3x3	rotation_x_axis_matrix_3x3(double radians);
t_matrix_3x3	rotation_y_axis_matrix_3x3(double radians);
t_matrix_3x3	rotation_z_axis_matrix_3x3(double radians);


t_vec3			row_to_vec(t_matrix_3x3 *matrix, int row);



t_vec3			column_to_vec(t_matrix_3x3 *matrix, int column);
void			vec_to_row(t_matrix_3x3 *matrix, t_vec3 vec, int row);
void			vec_to_column(t_matrix_3x3 *matrix, t_vec3 vec, int column);
/* matrix_3_stack */
void			push_mtrx_on_stack(t_matrix_3x3 *matrix, t_matrix_3x3 *matrix_stack,
				int *end_of_stack);
void			pop_mtrx_from_stack(t_matrix_3x3 *matrix,  t_matrix_3x3 *matrix_stack,
				int *end_of_stack);

#endif
