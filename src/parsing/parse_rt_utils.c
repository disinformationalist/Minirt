#include "minirt.h"

void	init_obs(t_trace *trace)
{
	trace->group = NULL;
	trace->bvh = NULL;
	trace->box = NULL;
	trace->amb = NULL;
	trace->cam = NULL;
	trace->lights = NULL;
	trace->spheres = NULL;
	trace->planes = NULL;
	trace->cylinders = NULL;
	trace->hyperboloids = NULL;
	trace->cubes = NULL;
	trace->textures = NULL;
	trace->obj_control = NULL;
}

void	init_counts(t_obj_counts *counts)
{
	counts->amb_count = 0;
	counts->cam_count = 0;
	counts->light_count = 0;
	counts->sphere_count = 0;
	counts->plane_count = 0;
	counts->cyl_count = 0;
	counts->hyp_count = 0;
	counts->cube_count = 0;
	counts->tx_count = 0;
	counts->tri_count = 0;
	counts->sl_count = 0;
	counts->al_count = 0;
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
