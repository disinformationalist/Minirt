#ifndef MATRIX_H
# define MATRIX_H

#include <immintrin.h> // For AVX SIMD intrinsics

//using single dimension representation for opimization. column major order

//4x4
typedef struct s_mat4
{
	double	mat[16];
}	t_mat4;

//3x3
typedef struct s_mat3
{
	double	mat[9];
}	t_mat3;

//2x2
typedef struct s_mat2
{
	double	mat[4];
}	t_mat2;

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
	double	w;
}	t_vec3;

typedef t_vec3	t_point;
typedef	t_vec3	t_quat;

typedef struct s_rots
{
	t_quat	xrots;
	t_quat	yrots;
	t_quat	zrots;
}	t_rots;

//some quaternion stuff for tracking in gui

t_quat	quat_mult(t_quat q1, t_quat q2);
t_quat	angle_to_quat(double angle, double x, double y, double z);
t_vec3	norm_quat(t_vec3 quat);
t_quat	qrot_to(t_vec3 from, t_vec3 to);


//matrix operations

t_mat4	identity(void);
t_mat4	transpose(t_mat4 m);
t_mat4	translation(double x, double y, double z);
t_mat4	scaling(double x, double y, double z);
t_mat4	inv_scaling(double x, double y, double z);
t_mat4	shearing(double xy, double xz, double yx, double yz, double zx, double zy);
void	tuple_to_col(t_mat4 *m, t_vec3 tuple, int col);

//rotate

t_mat4	rot_x(double rad);
t_mat4	rot_y(double rad);
t_mat4	rot_z(double rad);

//multiply

t_vec3	mat_vec_mult(t_mat4 m, t_vec3 v);
t_mat4	mat_mult(t_mat4 m1, t_mat4 m2);

//inverse

t_mat4	inverse(t_mat4 m);
double	cofact3(t_mat3 m ,int row, int col);
t_mat3	submat4(t_mat4 m, int row, int col);

//testing

void print_vec(t_vec3 v);
void print_mat4(t_mat4 m);

#endif