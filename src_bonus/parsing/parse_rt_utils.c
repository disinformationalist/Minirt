#include "minirt.h"

void	init_obs(t_trace *trace)
{
	trace->amb = NULL;
	trace->cam = NULL;
	trace->lights = NULL;
	trace->spheres = NULL;
	trace->planes = NULL;
	trace->cylinders = NULL;
	trace->lenses = NULL;
	trace->cubes = NULL;
	trace->triangles = NULL;
	trace->textures = NULL;
	trace->group = NULL;
	trace->mesh = NULL;
}

void	init_counts(t_obj_counts *counts)
{
	counts->amb_count = 0;
	counts->cam_count = 0;
	counts->light_count = 0;
	counts->sphere_count = 0;
	counts->plane_count = 0;
	counts->cyl_count = 0;
	counts->lens_count = 0;
	counts->cube_count = 0;
	counts->tx_count = 0;
	counts->tri_count = 0;
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
