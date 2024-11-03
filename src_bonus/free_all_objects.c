#include "minirt.h"

void	free_sp_list(t_sphere **start)
{
	t_sphere	*curr;
	t_sphere	*temp;

	if (*start == NULL)
		return ;
	curr = *start;
	curr = curr->next;
	while (curr != *start)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
	free(curr);
	*start = NULL;
}

void	free_pl_list(t_plane **start)
{
	t_plane		*curr;
	t_plane		*temp;

	if (*start == NULL)
		return ;
	curr = *start;
	curr = curr->next;
	while (curr != *start)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
	free(curr);
	*start = NULL;
}

void	free_cy_list(t_cylinder **start)
{
	t_cylinder		*curr;
	t_cylinder		*temp;

	if (*start == NULL)
		return ;
	curr = *start;
	curr = curr->next;
	while (curr != *start)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
	free(curr);
	*start = NULL;
}

void	free_le_list(t_lens **start)
{
	t_lens		*curr;
	t_lens		*temp;

	if (*start == NULL)
		return ;
	curr = *start;
	curr = curr->next;
	while (curr != *start)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
	free(curr);
	*start = NULL;
}

void	free_cu_list(t_cube **start)
{
	t_cube	*curr;
	t_cube	*temp;

	if (*start == NULL)
		return ;
	curr = *start;
	curr = curr->next;
	while (curr != *start)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
	free(curr);
	*start = NULL;
}

void	free_lt_list(t_light **start)
{
	t_light		*curr;
	t_light		*temp;

	if (*start == NULL)
		return ;
	curr = *start;
	curr = curr->next;
	while (curr != *start)
	{
		temp = curr->next;
		if (curr->type == AREA)
			free(curr->emitter);
		free(curr);
		curr = temp;
	}
	if (curr->type == AREA)
		free(curr->emitter);
	free(curr);
	*start = NULL;
}

void	free_curr_tx(void *mlx_con, t_tx *curr)
{
	if (curr->i_name)
		free(curr->i_name);
	if (curr->m_name)
		free(curr->m_name);
	if (curr->image)
	{
		mlx_destroy_image(mlx_con, curr->image->img_ptr);
		free(curr->image);
	}	
	if (curr->bump_map)
	{
		mlx_destroy_image(mlx_con, curr->bump_map->img_ptr);
		free(curr->bump_map);
	}	
	free(curr);
}

void	free_tx_list(void *mlx_con, t_tx **start)
{
	t_tx	*curr;
	t_tx	*temp;

	if (*start == NULL)
		return ;
	curr = *start;
	curr = curr->next;
	while (curr != *start)
	{
		temp = curr->next;
		free_curr_tx(mlx_con, curr);
		curr = temp;
	}
	free_curr_tx(mlx_con, curr);
	*start = NULL;
}

void	free_tri_list(t_tri **start)
{
	t_tri	*curr;
	t_tri	*temp;

	if (*start == NULL)
		return ;
	curr = *start;
	curr = curr->next;
	while (curr != *start)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
	free(curr);
	*start = NULL;
}

void	free_all_objects(t_trace *trace)
{
	
	if (trace->group)
		free_group(trace->group);
	//free_mapping(trace);//
	free_sp_list(&trace->spheres);
	free_pl_list(&trace->planes);
	free_cy_list(&trace->cylinders);
	free_le_list(&trace->lenses);
	free_lt_list(&trace->lights);
	free_cu_list(&trace->cubes);
	free_tri_list(&trace->triangles);
	free_tx_list(trace->mlx_connect, &trace->textures);
	if (trace->amb)
		free(trace->amb);
	if (trace->cam)
		free(trace->cam);
	
}