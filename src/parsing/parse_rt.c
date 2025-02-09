#include "minirt.h"

bool	build_obj_lists(t_trace *trace, char ***rt_file, int k)
{
	bool	status;

	status = false;
	if (!ft_strcmp(*(rt_file[k]), "sp"))
		status = append_sp(&trace->spheres, rt_file[k]);
	else if (!ft_strcmp(*(rt_file[k]), "pl"))
		status = append_pl(&trace->planes, rt_file[k]);
	else if (!ft_strcmp(*(rt_file[k]), "cy"))
		status = append_cy(&trace->cylinders, rt_file[k]);
	else if (!ft_strcmp(*(rt_file[k]), "hy"))
		status = append_hy(&trace->hyperboloids, rt_file[k]);
	else if (!ft_strcmp(*(rt_file[k]), "cu"))
		status = append_cu(&trace->cubes, rt_file[k]);
	else if (!ft_strcmp(*(rt_file[k]), "tx"))
		status = append_tx(&trace->textures, rt_file[k]);
	return (status);
}

bool	build_lists(t_trace *trace, char ***rt_file)
{
	int		k;
	bool	status;

	k = -1;
	status = false;
	while (rt_file[++k])
	{
		if (!ft_strcmp(*(rt_file[k]), "A"))
			status = set_amb(&trace->amb, rt_file[k]);
		else if (!ft_strcmp(*(rt_file[k]), "C"))
			status = set_cam(&trace->cam, rt_file[k]);
		else if (!ft_strcmp(*(rt_file[k]), "L"))
			status = append_light(trace, &trace->lights, rt_file[k]);
		else if (!ft_strcmp(*(rt_file[k]), "SL"))
			status = append_light(trace, &trace->lights, rt_file[k]);
		else if (!ft_strcmp(*(rt_file[k]), "AL"))
			status = append_light(trace, &trace->lights, rt_file[k]);
		else
			status = build_obj_lists(trace, rt_file, k);
		if (status)
			break ;
	}
	if (!trace->amb)
		status = set_empty_amb(&trace->amb);
	return (status);
}

void	count_ints(t_trace *trace, t_obj_counts counts)
{
	int	total_intersects;

	total_intersects = 1;
	total_intersects += 2 * counts.sphere_count;
	total_intersects += 4 * counts.cyl_count;
	total_intersects += 4 * counts.hyp_count;
	total_intersects += counts.plane_count;
	total_intersects += 2 * counts.cube_count;
	total_intersects += counts.tri_count;
	trace->total_ints = total_intersects;
	trace->sl_count = counts.sl_count;
	trace->al_count = counts.al_count;
}

//parsing test: print_all_objects(trace);

void	parse_rt(t_trace *trace, char ***rt_file)
{
	t_obj_counts	counts;

	init_counts(&counts);
	count_ids(&counts, rt_file);
	counts.light_count += counts.sl_count + counts.al_count;
	if (counts.cam_count == 0)
		free_exit(rt_file, "Error\n Scene must contain a camera\n", \
	"format: <type id> <x,y,z> <x,y,z> <fov>\n");
	count_ints(trace, counts);
	if (counts.tx_count > 30)
		free_exit(rt_file, "Error\n Too many textures\n", \
		" Only 30 textures allowed or less\n");
	check_ids(rt_file);
	init_obs(trace);
	if (build_lists(trace, rt_file))
	{
		free_all_objects(trace);
		free_3d_array_i(rt_file, ft_3darray_len(rt_file));
		ft_putstr_color_fd(2, "Error\n a build_lists malloc failed\n", RED);
		exit (EXIT_FAILURE);
	}
}
