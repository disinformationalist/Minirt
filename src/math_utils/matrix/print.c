#include "minirt.h"

//printing utilities for troubleshooting

void print_mat4(t_mat4 m)
{
	int	i;
	int	j;
	
	j = -1;
	while (++j < 4)
	{
		i = -1;
		while (++i < 4)
			printf("%f ", m.mat[i * 4 + j]);
		printf("\n");
	}
	printf("\n");
}

void print_vec(t_vec3 v)
{
	printf("vec: (%f, %f, %f, %f)\n\n", v.x, v.y, v.z, v.w);
}
