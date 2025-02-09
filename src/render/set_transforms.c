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
			curr_sp->rots = vec(0, 0, 0, 0);
			if (curr_sp == trace->spheres)
				break ;
		}
	}
}

void	set_transforms_curr(t_trace *trace, t_hyperboloid *curr_hy)
{
	t_mat4	inv_trans;
	t_mat4	inv_rot;

	inv_trans = translation(-curr_hy->center.x,
			-curr_hy->center.y, -curr_hy->center.z);
	inv_rot = rot_to(curr_hy->norm, vec(0, 1, 0, 0));
	curr_hy->curr_scale = inv_scaling(curr_hy->rad,
			curr_hy->half_h, curr_hy->rad);
	curr_hy->curr_rottran = mat_mult(inv_rot, inv_trans);
	curr_hy->transform = mat_mult(curr_hy->curr_scale,
			curr_hy->curr_rottran);
	curr_hy->i_transform = inverse(curr_hy->transform);
	curr_hy->t_transform = transpose(curr_hy->transform);
	curr_hy->pattern = uv_checker(18, 9 / M_PI, color(40, 40, 40), \
	color(255, 255, 255));
	curr_hy->texture = trace->textures;
	curr_hy->rots = extract_rot(inv_rot);
}

void	set_hy_transforms(t_trace *trace)
{
	t_hyperboloid	*curr_hy;

	if (trace->hyperboloids)
	{
		curr_hy = trace->hyperboloids;
		while (true)
		{
			set_transforms_curr(trace, curr_hy);
			curr_hy = curr_hy->next;
			if (curr_hy == trace->hyperboloids)
				break ;
		}
	}
}

void	set_cu_transforms2(t_trace *trace, t_cube *curr_cu)
{
	curr_cu->i_transform = inverse(curr_cu->transform);
	curr_cu->t_transform = transpose(curr_cu->transform);
	curr_cu->pattern = uv_checker(6, 6, color(30, 30, 30), \
	color(255, 255, 255));
	curr_cu->texture = trace->textures;
}

void	set_cu_transforms(t_trace *trace)
{
	t_cube			*curr_cu;
	t_mat4	inv_rot;
	t_mat4	inv_trans;

	if (trace->cubes)
	{
		curr_cu = trace->cubes;
		while (true)
		{
			curr_cu->curr_scale = inv_scaling(curr_cu->h_width, \
			curr_cu->h_height, curr_cu->h_depth);
			inv_trans = translation(-curr_cu->center.x, \
			-curr_cu->center.y, -curr_cu->center.z);
			inv_rot = rot_to(curr_cu->norm, vec(0, 1, 0, 0));
			curr_cu->curr_rottran = mat_mult(inv_rot, inv_trans);
			curr_cu->transform = (mat_mult(curr_cu->curr_scale, \
			curr_cu->curr_rottran));
			curr_cu->rots = extract_rot(inv_rot);
			set_cu_transforms2(trace, curr_cu);
			curr_cu = curr_cu->next;
			if (curr_cu == trace->cubes)
				break ;
		}
	}
}
