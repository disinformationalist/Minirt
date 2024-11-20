
#include "minirt.h"

void	print_matrix_4(t_matrix_4x4 *matrix)
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

void	print_matrix_3(t_matrix_3x3 *matrix)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < 3)
	{
		while (j < 3)
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

void	print_matrix_2(t_matrix_2x2 *matrix)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < 2)
	{
		while (j < 2)
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

void	print_vec(t_vec3 vec)
{
	printf("vec: (%f, %f, %f)", vec.x, vec.y, vec.z);
	printf("\n");
}
