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

void	free_all_objects(t_trace *trace)
{
	free_sp_list(&trace->spheres);
	free_pl_list(&trace->planes);
	free_cy_list(&trace->cylinders);
	free_lt_list(&trace->lights);
	free_cu_list(&trace->cubes);
	free_hy_list(&trace->hyperboloids);
	free_tx_list(trace->mlx_connect, &trace->textures);
	if (trace->amb)
		free(trace->amb);
	if (trace->cam)
		free(trace->cam);
}
