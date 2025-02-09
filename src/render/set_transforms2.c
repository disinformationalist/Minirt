#include "minirt.h"

void	set_pltrans(t_plane *curr_pl)
{
	t_mat4	inv_trans;
	t_mat4	inv_rot;

	inv_trans = translation(-curr_pl->point.x, \
	-curr_pl->point.y, -curr_pl->point.z);
	inv_rot = rot_to(curr_pl->norm, vec(0, 1, 0, 0));
	curr_pl->curr_scale = scaling(1.0, 1.0, 1.0);
	curr_pl->curr_rottran = mat_mult(inv_rot, inv_trans);
	curr_pl->transform = (mat_mult(curr_pl->curr_scale, \
	curr_pl->curr_rottran));
	curr_pl->t_transform = transpose(curr_pl->transform);
	curr_pl->i_transform = inverse(curr_pl->transform);
	curr_pl->rots = extract_rot(inv_rot);
}

void	set_pl_transforms(t_trace *trace)
{
	t_plane	*curr_pl;

	if (trace->planes)
	{
		curr_pl = trace->planes;
		while (true)
		{
			set_pltrans(curr_pl);
			curr_pl->norm = norm_vec(mat_vec_mult(curr_pl->t_transform, \
			vec(0, 1, 0, 0)));
			curr_pl->pattern = uv_checker(2, 2, color(30, 30, 30), \
			color(255, 255, 255));
			curr_pl->texture = trace->textures;
			curr_pl = curr_pl->next;
			if (curr_pl == trace->planes)
				break ;
		}
	}
}

void	set_cytrans(t_cylinder *curr_cy)
{
	t_mat4	inv_trans;
	t_mat4	inv_rot;

	inv_trans = translation(-curr_cy->center.x, \
	-curr_cy->center.y, -curr_cy->center.z);
	inv_rot = rot_to(curr_cy->norm, vec(0, 1, 0, 0));
	curr_cy->curr_scale = inv_scaling(curr_cy->radius, \
	curr_cy->height / 2, curr_cy->radius);
	curr_cy->curr_rottran = mat_mult(inv_rot, inv_trans);
	curr_cy->transform = mat_mult(curr_cy->curr_scale, curr_cy->curr_rottran);
	curr_cy->t_transform = transpose(curr_cy->transform);
	curr_cy->i_transform = inverse(curr_cy->transform);
	curr_cy->rots = extract_rot(inv_rot);
}

void	set_cy_transforms(t_trace *trace)
{
	t_cylinder	*curr_cy;

	if (trace->cylinders)
	{
		curr_cy = trace->cylinders;
		while (true)
		{
			set_cytrans(curr_cy);
			curr_cy->pattern = uv_checker(18, 9 / M_PI, color(40, 40, 40), \
			color(255, 255, 255));
			curr_cy->texture = trace->textures;
			curr_cy = curr_cy->next;
			if (curr_cy == trace->cylinders)
				break ;
		}
	}
}
