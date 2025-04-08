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

//destroy malloced img made with create img or imported

void	destroy_img(t_img *img, void *con)
{
	mlx_destroy_image(con, img->img_ptr);
	free(img);
}

void	free_control(void *con, t_control *controls)
{
	if (controls->menu)
		destroy_img(controls->menu, con);
	if (controls->lt_con)
		destroy_img(controls->lt_con, con);
	if (controls->r)
		destroy_img(controls->r, con);
	if (controls->g)
		destroy_img(controls->g, con);
	if (controls->b)
		destroy_img(controls->b, con);
	if (controls->bron)
		destroy_img(controls->bron, con);
	if (controls->sil)
		destroy_img(controls->sil, con);
	if (controls->ct)
		destroy_img(controls->ct, con);
	if (controls->pt)
		destroy_img(controls->pt, con);
	if (controls->rot)
		destroy_img(controls->rot, con);
	if (controls->rot_dials)
		destroy_img(controls->rot_dials, con);
	if (controls->dtx)
		destroy_img(controls->dtx, con);
	if (controls->dty)
		destroy_img(controls->dty, con);
	if (controls->dtz)
		destroy_img(controls->dtz, con);

	if (controls->pos)
		destroy_img(controls->pos, con);
	if (controls->pos_dials)
		destroy_img(controls->pos_dials, con);
	if (controls->dtpx)
		destroy_img(controls->dtpx, con);
	if (controls->dtpy)
		destroy_img(controls->dtpy, con);
	if (controls->dtpz)
		destroy_img(controls->dtpz, con);

	if (controls->sca)
		destroy_img(controls->sca, con);
	if (controls->sca_dials)
		destroy_img(controls->sca_dials, con);
	if (controls->sca_tracks)
		destroy_img(controls->sca_tracks, con);
	if (controls->waist)
		destroy_img(controls->waist, con);

	if (controls->flash)
		destroy_img(controls->flash, con);
	if (controls->fov)
		destroy_img(controls->fov, con);
	free(controls);
}

void	free_all_objects(t_trace *trace)
{
	if (trace->group)
		free_group(trace->group);
	if (trace->bvh)
		free_group(trace->bvh);
	if (trace->box)
		free(trace->box);
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
	if (trace->obj_control)
		free_control(trace->mlx_connect, trace->obj_control);
}
