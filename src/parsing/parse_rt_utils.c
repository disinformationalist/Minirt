#include "minirt.h"

void	init_obs(t_trace *trace)
{
	trace->amb = NULL;
	trace->cam = NULL;
	trace->lights = NULL;
	trace->spheres = NULL;
	trace->planes = NULL;
	trace->cylinders = NULL;
}

void	init_counts(t_trace *trace)
{
	trace->amb_count = 0;
	trace->cam_count = 0;
	trace->light_count = 0;
	trace->sphere_count = 0;
	trace->plane_count = 0;
	trace->cyl_count = 0;
}

void	count_check(int *item_count, char *msg, char ***rt_file)
{
	(*item_count)++;
	if (*item_count > 1)
	{
		free_3d_array_i(rt_file, ft_3darray_len(rt_file));
		error_exit(msg);
	}
}