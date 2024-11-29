#include "minirt.h"

//set all transforms to account for position and orientation.

void	set_sp_transforms(t_trace *trace)
{
	t_sphere	*curr_sp;

	if (trace->spheres)
	{
		curr_sp = trace->spheres;
		while (true)
		{
			curr_sp->curr_scale = inv_scaling(curr_sp->radius, \
			curr_sp->radius, curr_sp->radius);
			curr_sp->curr_rottran = translation(-curr_sp->center.x, \
			-curr_sp->center.y, -curr_sp->center.z);
			curr_sp->transform = (mat_mult(curr_sp->curr_scale, \
			curr_sp->curr_rottran));
			curr_sp->t_transform = transpose(curr_sp->transform);
			curr_sp->i_transform = inverse(curr_sp->transform);
			curr_sp->pattern = uv_checker(20, 10, color(40, 40, 40), \
			color(255, 255, 255));
			curr_sp->texture = trace->textures;
			curr_sp = curr_sp->next;
			if (curr_sp == trace->spheres)
				break ;
		}
	}
}

void	set_pl_transforms(t_trace *trace)
{
	t_plane			*curr_pl;
	t_matrix_4x4	inv_trans;
	t_matrix_4x4	inv_rot;

	if (trace->planes)
	{
		curr_pl = trace->planes;
		while (true)
		{
			inv_trans = translation(-curr_pl->point.x, -curr_pl->point.y, -curr_pl->point.z);
			inv_rot = rot_to(curr_pl->norm,  vec(0, 1, 0, 0));
			curr_pl->curr_scale = scaling(1.0, 1.0, 1.0);
			curr_pl->curr_rottran = mat_mult(inv_rot, inv_trans);
			curr_pl->transform = (mat_mult(curr_pl->curr_scale, curr_pl->curr_rottran));
			curr_pl->t_transform = transpose(curr_pl->transform);
			curr_pl->i_transform = inverse(curr_pl->transform);		
			curr_pl->norm = norm_vec(mat_vec_mult(curr_pl->t_transform, vec(0, 1, 0, 0)));
			curr_pl->pattern = uv_checker(2, 2, color(30, 30, 30), color(255, 255, 255));
			curr_pl->texture = trace->textures;
			curr_pl = curr_pl->next;
			if (curr_pl == trace->planes)
				break;
		}
	}
}

void	set_cy_transforms(t_trace *trace)
{
	t_cylinder		*curr_cy;
	t_matrix_4x4	inv_trans;
	t_matrix_4x4	inv_rot;

	if (trace->cylinders)
	{
		curr_cy = trace->cylinders;
		while (true)
		{
			inv_trans = translation(-curr_cy->center.x, -curr_cy->center.y, -curr_cy->center.z);
			inv_rot = rot_to(curr_cy->norm,  vec(0, 1, 0, 0));
			curr_cy->curr_scale = inv_scaling(curr_cy->radius, curr_cy->half_h, curr_cy->radius);
			curr_cy->curr_rottran = mat_mult(inv_rot, inv_trans);
			curr_cy->transform = mat_mult(curr_cy->curr_scale, curr_cy->curr_rottran);
			curr_cy->t_transform = transpose(curr_cy->transform);
			curr_cy->i_transform = inverse(curr_cy->transform);
			curr_cy->pattern = uv_checker(18, 9 / M_PI, color(40, 40, 40), color(255, 255, 255));
			curr_cy->texture = trace->textures;
			curr_cy = curr_cy->next;
			if (curr_cy == trace->cylinders)
				break;
		}
	}
}

void	set_hy_transforms(t_trace *trace)
{
	t_hyperboloid	*curr_hy;
	t_matrix_4x4	inv_trans;
	t_matrix_4x4	inv_rot;

	if (trace->hyperboloids)
	{
		curr_hy = trace->hyperboloids;
		while (true)
		{
			inv_trans = translation(-curr_hy->center.x, -curr_hy->center.y, -curr_hy->center.z);
			inv_rot = rot_to(curr_hy->norm,  vec(0, 1, 0, 0));
			curr_hy->curr_scale = inv_scaling(curr_hy->rad1, curr_hy->half_h, curr_hy->rad2);//check what to do with radiuses
			curr_hy->curr_rottran = mat_mult(inv_rot, inv_trans);
			curr_hy->transform = mat_mult(curr_hy->curr_scale, curr_hy->curr_rottran);
			curr_hy->i_transform = inverse(curr_hy->transform);
			curr_hy->t_transform = transpose(curr_hy->transform);
			curr_hy = curr_hy->next;
			if (curr_hy == trace->hyperboloids)
				break;
		}
	}
}

void	set_cu_transforms(t_trace *trace)
{
	t_cube			*curr_cu;
	t_matrix_4x4	inv_rot;
	t_matrix_4x4	inv_trans;

	if (trace->cubes)
	{
		curr_cu = trace->cubes;
		while (true)
		{
			curr_cu->curr_scale = inv_scaling(curr_cu->h_width, \
			curr_cu->h_height, curr_cu->h_depth);
			inv_trans = translation(-curr_cu->center.x, \
			-curr_cu->center.y, -curr_cu->center.z);
			inv_rot = rot_to(curr_cu->norm,  vec(0, 1, 0, 0));
			curr_cu->curr_rottran = mat_mult(inv_rot, inv_trans);
			curr_cu->transform = (mat_mult(curr_cu->curr_scale, \
			curr_cu->curr_rottran));
			curr_cu->i_transform = inverse(curr_cu->transform);
			curr_cu->t_transform = transpose(curr_cu->transform);
			curr_cu = curr_cu->next;
			if (curr_cu == trace->cubes)
				break;
		}
	}
}
