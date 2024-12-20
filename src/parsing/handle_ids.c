#include "minirt.h"

void	increment_id_count(t_obj_counts *counts, char ***rt_file, int k)
{
	if (!ft_strcmp(*(rt_file[k]), "L"))
		counts->light_count++;
	else if (!ft_strcmp(*(rt_file[k]), "SL"))
		counts->sl_count++;
	else if (!ft_strcmp(*(rt_file[k]), "AL"))
		counts->al_count++;
	else if (!ft_strcmp(*(rt_file[k]), "sp"))
		counts->sphere_count++;
	else if (!ft_strcmp(*(rt_file[k]), "pl"))
		counts->plane_count++;
	else if (!ft_strcmp(*(rt_file[k]), "cy"))
		counts->cyl_count++;
	else if (!ft_strcmp(*(rt_file[k]), "hy"))
		counts->hyp_count++;
	else if (!ft_strcmp(*(rt_file[k]), "cu"))
		counts->cube_count++;
	else if (!ft_strcmp(*(rt_file[k]), "tx"))
		counts->tx_count++;
	else
		return (free_3d_array_i(rt_file, ft_3darray_len(rt_file)),
			error_exit("Error\n Invalid type identifier\n"));
}

void	count_ids(t_obj_counts *counts, char ***rt_file)
{
	int				k;

	k = -1;
	while (rt_file[++k])
	{
		if (!ft_strcmp(*(rt_file[k]), "A"))
			count_check(&counts->amb_count,
				"Error\n Only one ambient light allowed\n", rt_file);
		else if (!ft_strcmp(*(rt_file[k]), "C"))
			count_check(&counts->cam_count,
				"Error\n Only one camera allowed\n", rt_file);
		else
			increment_id_count(counts, rt_file, k);
	}
}

void	check_obj_ids(char ***rt_file, int k)
{
	if (!ft_strcmp(*(rt_file[k]), "sp"))
		check_sp(rt_file[k], rt_file);
	else if (!ft_strcmp(*(rt_file[k]), "pl"))
		check_pl(rt_file[k], rt_file);
	else if (!ft_strcmp(*(rt_file[k]), "cy"))
		check_cy(rt_file[k], rt_file);
	else if (!ft_strcmp(*(rt_file[k]), "hy"))
		check_hy(rt_file[k], rt_file);
	else if (!ft_strcmp(*(rt_file[k]), "cu"))
		check_cu(rt_file[k], rt_file);
	else if (!ft_strcmp(*(rt_file[k]), "tx"))
		check_tx(rt_file[k], rt_file);
}

void	check_ids(char ***rt_file)
{
	int	k;

	k = -1;
	while (rt_file[++k])
	{
		check_str_len(rt_file[k], rt_file);
		if (!ft_strcmp(*(rt_file[k]), "A"))
			check_amb(rt_file[k], rt_file);
		else if (!ft_strcmp(*(rt_file[k]), "C"))
			check_cam(rt_file[k], rt_file);
		else if (!ft_strcmp(*(rt_file[k]), "L"))
			check_light(rt_file[k], rt_file);
		else if (!ft_strcmp(*(rt_file[k]), "SL"))
			check_sl(rt_file[k], rt_file);
		else if (!ft_strcmp(*(rt_file[k]), "AL"))
			check_al(rt_file[k], rt_file);
		else
			check_obj_ids(rt_file, k);
	}
}
