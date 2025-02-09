#include "minirt.h"

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

void	free_hy_list(t_hyperboloid **start)
{
	t_hyperboloid	*curr;
	t_hyperboloid	*temp;

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
