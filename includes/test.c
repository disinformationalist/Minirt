#include "minirt.h"

void	init(t_matrix_4x4 *matrix)
{
	int	i;
	int	j;

	i = 0;
	while (i < MTRX_SIZE)
	{
		j = 0;
		while (j < MTRX_SIZE)
		{
			if (i == j)
				matrix->m[i][j] = 1.0;
			else
				matrix->m[i][j] = 0.0;
			j++;
		}
		i++;
	}
}

void	print_mat(t_matrix_4x4 *matrix)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < 4)
	{
		while (j < 4)
		{
			printf("%f, ", matrix->m[i][j]);
			j++;
		}
		printf("\n");
		j = 0;
		i++;
	}
	printf("\n");
}

t_matrix_4x4	tran(double x, double y, double z)
{
	t_matrix_4x4	matrix;
	
	init(&matrix);
	matrix.m[0][3] = x;
	matrix.m[1][3] = y;
	matrix.m[2][3] = z;
	return (matrix);
}

double	dt(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

t_vec3	row_vec4(t_matrix_4x4 matrix, int row)
{
	t_vec3	vec;

	vec.x = matrix.m[row][0];
	vec.y = matrix.m[row][1];
	vec.z = matrix.m[row][2];
	vec.w = matrix.m[row][3];
	return (vec);
}

t_vec3	mat_v_mult(t_matrix_4x4 matrix, t_vec3 vec)
{
	t_vec3		new_vec;

	new_vec.x = dt(row_vec4(matrix, 0), vec);
	new_vec.y = dt(row_vec4(matrix, 1), vec);
	new_vec.z = dt(row_vec4(matrix, 2), vec);
	new_vec.w = dt(row_vec4(matrix, 3), vec);
	return (new_vec);
}

void	print_vec(t_vec3 vec)
{
	printf("vec: (%f, %f, %f, %f)", vec.x, vec.y, vec.z, vec.w);
	printf("\n");
}

int main()
{
	t_matrix_4x4 mat;
	t_vec3 v;
	v.x = 1;
	v.y = 1;
	v.z = 1;
	v.w = 1;

	mat = tran(3, 3, 3);
	print_mat(&mat);
	v = mat_v_mult(mat, v);

	print_vec(v);


	return (0);
}