#include "minirt.h"

void	count_ids(t_obj_counts *counts, char ***rt_file, int *k)
{
	while (rt_file[++(*k)])
	{
		if (!ft_strcmp(*(rt_file[*k]), "A"))
			count_check(&counts->amb_count,
				"Error\n Only one ambient light allowed\n", rt_file);
		else if (!ft_strcmp(*(rt_file[*k]), "C"))
			count_check(&counts->cam_count,
				"Error\n Only one camera allowed\n", rt_file);
		else if (!ft_strcmp(*(rt_file[*k]), "L"))
			counts->light_count++;
		else if (!ft_strcmp(*(rt_file[*k]), "SL"))
			counts->light_count++;
		else if (!ft_strcmp(*(rt_file[*k]), "AL"))
			counts->light_count++;
		else if (!ft_strcmp(*(rt_file[*k]), "sp"))
			counts->sphere_count++;
		else if (!ft_strcmp(*(rt_file[*k]), "pl"))
			counts->plane_count++;
		else if (!ft_strcmp(*(rt_file[*k]), "cy"))
			counts->cyl_count++;
		else if (!ft_strcmp(*(rt_file[*k]), "hy"))
			counts->hyp_count++;
		else if (!ft_strcmp(*(rt_file[*k]), "le"))
			counts->lens_count++;
		else if (!ft_strcmp(*(rt_file[*k]), "cu"))
			counts->cube_count++;
		else if (!ft_strcmp(*(rt_file[*k]), "tri"))
			counts->tri_count++;
		else if (!ft_strcmp(*(rt_file[*k]), "tx"))
			counts->tx_count++;
		else
			return (free_3d_array_i(rt_file, ft_3darray_len(rt_file)),
				error_exit("Error\n Invalid type identifier\n"));
	}
}

void	check_ids(char ***rt_file)
{
	int	k;

	k = -1;
	while (rt_file[++k])
	{
		if (!ft_strcmp(*(rt_file[k]), "A"))
			check_amb(rt_file[k], rt_file);
		if (!ft_strcmp(*(rt_file[k]), "C"))
			check_cam(rt_file[k], rt_file);
		if (!ft_strcmp(*(rt_file[k]), "L"))
			check_light(rt_file[k], rt_file);
		if (!ft_strcmp(*(rt_file[k]), "sp"))
			check_sp(rt_file[k], rt_file);
		if (!ft_strcmp(*(rt_file[k]), "pl"))
			check_pl(rt_file[k], rt_file);
		if (!ft_strcmp(*(rt_file[k]), "cy"))
			check_cy(rt_file[k], rt_file);
		if (!ft_strcmp(*(rt_file[k]), "hy"))
			check_hy(rt_file[k], rt_file);
		if (!ft_strcmp(*(rt_file[k]), "le"))
			check_le(rt_file[k], rt_file);
		if (!ft_strcmp(*(rt_file[k]), "cu"))
			check_cu(rt_file[k], rt_file);
		if (!ft_strcmp(*(rt_file[k]), "tri"))
			check_tri(rt_file[k], rt_file);
		if (!ft_strcmp(*(rt_file[k]), "tx"))
			check_tx(rt_file[k], rt_file);
		if (!ft_strcmp(*(rt_file[k]), "SL"))
			check_sl(rt_file[k], rt_file);
		if (!ft_strcmp(*(rt_file[k]), "AL"))
			check_al(rt_file[k], rt_file);
	}
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
		else if (!ft_strcmp(*(rt_file[k]), "sp"))
			status = append_sp(&trace->spheres, rt_file[k]);
		else if (!ft_strcmp(*(rt_file[k]), "pl"))
			status = append_pl(&trace->planes, rt_file[k]);
		else if (!ft_strcmp(*(rt_file[k]), "cy"))
			status = append_cy(&trace->cylinders, rt_file[k]);
		else if (!ft_strcmp(*(rt_file[k]), "hy"))
			status = append_hy(&trace->hyperboloids, rt_file[k]);
		else if (!ft_strcmp(*(rt_file[k]), "le"))
			status = append_le(&trace->lenses, rt_file[k]);
		else if (!ft_strcmp(*(rt_file[k]), "cu"))
			status = append_cu(&trace->cubes, rt_file[k]);
		else if (!ft_strcmp(*(rt_file[k]), "tri"))
			status = append_tri(&trace->triangles, rt_file[k]);
		else if (!ft_strcmp(*(rt_file[k]), "tx"))
			status = append_tx(&trace->textures, rt_file[k]);
		else if (!ft_strcmp(*(rt_file[k]), "SL"))
			status = append_light(trace, &trace->lights, rt_file[k]);
		else if (!ft_strcmp(*(rt_file[k]), "AL"))
			status = append_light(trace, &trace->lights, rt_file[k]);
		if (status)
			break ;
	}
	return (status);
}

void	count_ints(t_trace *trace, t_obj_counts counts)
{
	int	total_intersects;

	total_intersects = 0;
	total_intersects += 2 * counts.sphere_count;
	total_intersects += 4 * counts.lens_count; 
	total_intersects += 4 * counts.cyl_count;
	total_intersects += 4 * counts.hyp_count;
	total_intersects += counts.plane_count;
	total_intersects += 2 * counts.cube_count;
	total_intersects += counts.tri_count;
	trace->total_ints = total_intersects;
}

void	parse_rt(t_trace *trace, char ***rt_file)
{
	t_obj_counts	counts;
	int				k;

	k = -1;
	init_counts(&counts);
	count_ids(&counts, rt_file, &k);
	if (counts.amb_count == 0)
		free_exit(rt_file,
			"Error\n Missing or invalid ambient lighting identifier\n", \
	" Ambient lighting line must begin with 'A'\n");
	if (counts.cam_count == 0)
		free_exit(rt_file, "Error\n Missing or invalid camera identifier\n", \
	" Camera line must begin with 'C'\n");
	count_ints(trace, counts);
	check_ids(rt_file);
	init_obs(trace);
	if (build_lists(trace, rt_file))
	{
		free_all_objects(trace);
		free_3d_array_i(rt_file, ft_3darray_len(rt_file));
		ft_putstr_color_fd(2, "Error\n a build_lists malloc failed\n", RED);
		exit (EXIT_FAILURE);
	}
	//print_all_objects(trace);
}
