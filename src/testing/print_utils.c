#include "minirt.h"

void	print_obj_nums(t_trace *trace)
{
	printf("amb_count: %d\n", trace->amb_count);
	printf("cam_count: %d\n", trace->cam_count);
	printf("light_count: %d\n", trace->light_count);
	printf("sphere_count: %d\n", trace->sphere_count);
	printf("plane_count: %d\n", trace->plane_count);
	printf("cyl_count: %d\n", trace->cyl_count);
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