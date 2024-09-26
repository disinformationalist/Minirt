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
			curr_sp->curr_scale = inv_scaling(curr_sp->radius, curr_sp->radius, curr_sp->radius);
			curr_sp->curr_rottran = translation(-curr_sp->center.x, -curr_sp->center.y, -curr_sp->center.z);
			curr_sp->transform = (mat_mult(curr_sp->curr_scale, curr_sp->curr_rottran));
			curr_sp = curr_sp->next;
			if (curr_sp == trace->spheres)
				break;
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
			curr_pl->transform = curr_pl->curr_rottran;			
			curr_pl->norm = norm_vec(mat_vec_mult(transpose(curr_pl->transform), vec(0, 1, 0, 0)));
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
			curr_cy->curr_scale = inv_scaling(curr_cy->radius, 1.0, curr_cy->radius);
			curr_cy->curr_rottran = mat_mult(inv_rot, inv_trans);
			curr_cy->transform = mat_mult(curr_cy->curr_scale, curr_cy->curr_rottran);
			curr_cy = curr_cy->next;
			if (curr_cy == trace->cylinders)
				break;
		}
	}
}
