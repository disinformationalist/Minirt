
#ifndef MATRIX_OPS_H
# define MATRIX_OPS_H

# define MTRX_SIZE 4
# define SIZE_3X3 3
# define SIZE_2X2 2

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

//matrix ops
void			identity(t_matrix_4x4 *matrix);
t_vec3			mat_vec_mult(t_matrix_4x4 matrix, t_vec3 vec);
t_matrix_4x4	mat_mult(t_matrix_4x4 m_1, t_matrix_4x4 m_2);
t_matrix_4x4	transpose(t_matrix_4x4 matrix);

//inverse and its utils
t_matrix_4x4	inverse(t_matrix_4x4 matrix);
t_matrix_3x3	submat_4(t_matrix_4x4 matrix, int row, int column);
t_matrix_2x2	submat_3(t_matrix_3x3 mat, int row, int column);
double			det_2(t_matrix_2x2 mat);
double			min_3(t_matrix_3x3 mat, int row, int column);

//move and scale

t_matrix_4x4	scaling(double x, double y, double z);
t_matrix_4x4	inv_scaling(double x, double y, double z);
t_matrix_4x4	translation(double x, double y, double z);

//rot
t_matrix_4x4	rot_x(double radians);
t_matrix_4x4	rot_y(double radians);
t_matrix_4x4	rot_z(double radians);

//other utils

t_vec3			row_to_vec(t_matrix_3x3 *matrix, int row);
t_vec3			column_to_vec(t_matrix_3x3 *matrix, int column);
void			vec_to_row(t_matrix_3x3 *matrix, t_vec3 vec, int row);
void			vec_to_column(t_matrix_3x3 *matrix, t_vec3 vec, int column);
void			tuple_to_row(t_matrix_4x4 *matrix, t_vec3 tuple, int row);
void			tuple_to_col(t_matrix_4x4 *matrix, t_vec3 tuple, int col);

//print utils
void			print_matrix_4(t_matrix_4x4 *matrix);
void			print_matrix_3(t_matrix_3x3 *matrix);
void			print_matrix_2(t_matrix_2x2 *matrix);
void			print_vec(t_vec3 vec);




/* stuff in unused matices folder */
//add/remove stack in rt_matrix_transf.c
void			init_matrix_3(double matrix[3][3]);
void			init_matrix_2(double matrix[2][2]);
int				are_matrices_equal(t_matrix_4x4 *matrix_1, t_matrix_4x4 *matrix_2);
void			init_identity_matrix(t_matrix_4x4 *matrix);

#endif
