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
			inv_rot = rot_up(curr_pl->norm);
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
			inv_rot = rot_up(curr_cy->norm);
			curr_cy->curr_scale = inv_scaling(curr_cy->radius, 1.0, curr_cy->radius);
			curr_cy->curr_rottran = mat_mult(inv_rot, inv_trans);
			curr_cy->transform = mat_mult(curr_cy->curr_scale, curr_cy->curr_rottran);
			curr_cy = curr_cy->next;
			if (curr_cy == trace->cylinders)
				break;
		}
	}
}

void	set_le_transforms(t_trace *trace)
{
	t_lens	*curr_le;
	t_matrix_4x4	inv_trans;
	t_matrix_4x4	inv_rot;

	if (trace->lenses)
	{
		curr_le = trace->lenses;
		while (true)
		{
			curr_le->sphere_1.curr_scale = inv_scaling(curr_le->sphere_1.radius, curr_le->sphere_1.radius, curr_le->sphere_1.radius);
			curr_le->sphere_1.curr_rottran = translation(-curr_le->sphere_1.center.x, -curr_le->sphere_1.center.y, -curr_le->sphere_1.center.z);
			curr_le->sphere_1.transform = (mat_mult(curr_le->sphere_1.curr_scale, curr_le->sphere_1.curr_rottran));
			curr_le->sphere_2.curr_scale = inv_scaling(curr_le->sphere_2.radius, curr_le->sphere_2.radius, curr_le->sphere_2.radius);
			curr_le->sphere_2.curr_rottran = translation(-curr_le->sphere_2.center.x, -curr_le->sphere_2.center.y, -curr_le->sphere_2.center.z);
			curr_le->sphere_2.transform = (mat_mult(curr_le->sphere_2.curr_scale, curr_le->sphere_2.curr_rottran));
			inv_trans = translation(-curr_le->center.x, -curr_le->center.y, -curr_le->center.z);
			inv_rot = rot_up(curr_le->axis);
			curr_le->curr_scale = inv_scaling(curr_le->radius, 1.0, curr_le->radius);
			curr_le->curr_rottran = mat_mult(inv_rot, inv_trans);
			curr_le->transform = mat_mult(curr_le->curr_scale, curr_le->curr_rottran);
			curr_le = curr_le->next;
			if (curr_le == trace->lenses)
				break;
		}
	}
}

//not using view trans right now.

/* using from will inv_translate that much, the diff between from and to is 
the orienation.

	(up)^	O(to)
		|  /
		| /------orientation vec
		|/
		O---------cam location
 	 (from) 
*/
//t_matrix_4x4	view_transform(t_point from, t_point to, t_vec3 up)

t_matrix_4x4	view_transform(t_point from, t_vec3 ori_vec, t_vec3 up)
{
	t_matrix_4x4	orient;
	//t_vec3			forward;
	t_vec3			left;
	t_vec3			true_up;

//try to pass in the orientation for forward, and from as a translation
	//forward = norm_vec(subtract_vec(to, from));
	//forward = norm_vec(subtract_vec(to, from));
	left = cross_prod(ori_vec, norm_vec(up));
	true_up = cross_prod(left, ori_vec);
	tuple_to_row(&orient, vec(left.x, left.y, left.z, 0), 0);
	tuple_to_row(&orient, vec(true_up.x, true_up.y, true_up.z, 0), 1);
	tuple_to_row(&orient, vec(-ori_vec.x, -ori_vec.y, -ori_vec.z, 0), 2);
	tuple_to_row(&orient, vec(0, 0, 0, 1), 3);
	return (mat_mult(orient, translation(-from.x, -from.y, -from.z)));
}