#include "minirt.h"

//set all transforms to account for position and orientation.

void	set_sp_transforms(t_trace *trace)
{
	t_sphere		*curr_sp;
	t_matrix_4x4	inv_scale;
	t_matrix_4x4	inv_tran;

	if (trace->spheres)
	{
		curr_sp = trace->spheres;
		while (true)
		{
			inv_scale = inv_scaling(curr_sp->radius, curr_sp->radius, curr_sp->radius);
			inv_tran = translation(-curr_sp->center.x, -curr_sp->center.y, -curr_sp->center.z);
			curr_sp->transform = (mat_mult(inv_scale, inv_tran));
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
			curr_pl->transform = mat_mult(inv_rot, inv_trans);	
			curr_pl = curr_pl->next;
			if (curr_pl == trace->planes)
				break;
		}
	}
}

void	set_cy_transforms(t_trace *trace)
{
	t_cylinder		*curr_cy;
	t_matrix_4x4	inv_scale;
	t_matrix_4x4	inv_trans;
	t_matrix_4x4	inv_rot;

	if (trace->cylinders)
	{
		curr_cy = trace->cylinders;
		while (true)
		{
			inv_trans = translation(-curr_cy->center.x, -curr_cy->center.y, -curr_cy->center.z);
			inv_scale = inv_scaling(curr_cy->radius, curr_cy->radius, curr_cy->radius);
			inv_rot = rot_up(curr_cy->norm);
			curr_cy->transform = mat_mult(inv_scale, mat_mult(inv_rot, inv_trans));
			curr_cy = curr_cy->next;
			if (curr_cy == trace->cylinders)
				break;
		}
	}
}

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

t_matrix_4x4	view_transform(t_point from, t_point ori_vec, t_vec3 up)
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

void	render(t_trace *trace)
{
	//set all transforms here maybe move to outside render later... must adjust hooks
	set_sp_transforms(trace);
	set_pl_transforms(trace);
	set_cy_transforms(trace);
	/* t_vec3 cam_cen;
	t_vec3 cam_ori;
	cam_cen = trace->cam->center;
	cam_ori = trace->cam->orient;
	trace->cam->transform = view_transform(cam_cen, cam_ori, vec(0, 1, 0, 0));
 	//trace->cam->transform = view_transform(vec(0, 0, -6, 1), vec(0, 0, 1, 1), vec(0, 1, 0, 0)); 
	print_matrix_4(&trace->cam->transform);
 */
	//print_vec((trace->cam->orient));

	if (trace->supersample)
	{
		ft_putstr_color_fd(1, "SUPERSAMPLE IN PROGRESS...\n", BOLD_GREEN);
		trace->pixels_xl = malloc_ui_matrix(trace->width_orig * trace->s_kernel, trace->height_orig * trace->s_kernel);
		if (!trace->pixels_xl)
		{
			printf(RED"The super malloc has failed\n"RESET);
			close_win(trace);
		}
		zero_ui_matrix(trace->pixels_xl, trace->width, trace->height);
	}
	
	long start = get_time();//test
	render_scene(trace);
	
	if (trace->supersample)
	{
		downsample_xl(trace->width, trace->height, &trace->img, trace->pixels_xl, trace->s_kernel);
		zero_ui_matrix(trace->pixels_xl, trace->width, trace->height);
		free_ui_matrix(trace->pixels_xl, trace->height);
		ft_putstr_color_fd(1, "SUPERSAMPLE COMPLETE!\n", BOLD_BRIGHT_GREEN);
	}
	mlx_put_image_to_window(trace->mlx_connect,
		trace->mlx_win, trace->img.img_ptr, 0, 0);
	print_times(start, get_time(), "RENDER TIME: %f seconds\n");//test
}
