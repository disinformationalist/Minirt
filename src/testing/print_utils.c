#include "minirt.h"

void	print_obj_nums(t_obj_counts *counts)
{
	printf("amb_count: %d\n", counts->amb_count);
	printf("cam_count: %d\n", counts->cam_count);
	printf("light_count: %d\n", counts->light_count);
	printf("sphere_count: %d\n", counts->sphere_count);
	printf("plane_count: %d\n", counts->plane_count);
	printf("cyl_count: %d\n", counts->cyl_count);
}

void	print_3d_array(char ***array)
{
	int	i;
	int	j;

	i = 0;
	while (array[i])
	{
		j = 0;
		while (array[i][j])
		{
			printf("%s\n", array[i][j]);
			j++;
		}
		i++;
	}
}
