
#include "minirt.h"

t_vec3	row_to_vec(t_matrix_3x3 *matrix, int row)
{
	t_vec3	vec;

	vec.x = matrix->m[row][0];
	vec.y = matrix->m[row][1];
	vec.z = matrix->m[row][2];
	return (vec);
}

t_vec3	column_to_vec(t_matrix_3x3 *matrix, int column)
{
	t_vec3	vec;

	vec.x = matrix->m[0][column];
	vec.y = matrix->m[1][column];
	vec.z = matrix->m[2][column];
	return (vec);
}

void	vec_to_row(t_matrix_3x3 *matrix, t_vec3 vec, int row)
{
	matrix->m[row][0] = vec.x;
	matrix->m[row][1] = vec.y;
	matrix->m[row][2] = vec.z;
}

void	vec_to_column(t_matrix_3x3 *matrix, t_vec3 vec, int column)
{
	matrix->m[0][column] = vec.x;
	matrix->m[1][column] = vec.y;
	matrix->m[2][column] = vec.z;
}

void	tuple_to_row(t_matrix_4x4 *matrix, t_vec3 tuple, int row)
{
	matrix->m[row][0] = tuple.x;
	matrix->m[row][1] = tuple.y;
	matrix->m[row][2] = tuple.z;
	matrix->m[row][3] = tuple.w;
}
