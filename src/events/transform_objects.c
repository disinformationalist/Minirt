#include "minirt.h"
//in progress reintit cam based on current orient and true up... must rotate with object space!
void	reset_topleft(t_trace *trace, t_vec3 view_center, double view_width, double view_height)
{
	t_point	view_topleft;
	t_vec3	horizontal_move;
	t_vec3	vertical_move;
	t_vec3	right;
	t_vec3	true_up;
	
	right = norm_vec(cross_prod(trace->cam->true_up, trace->cam->orient));
	true_up = trace->cam->true_up;

	horizontal_move = scale_vec(view_width / 2.0, right);
	vertical_move = scale_vec(view_height / 2.0, true_up);
	view_topleft = add_vec(view_center, vertical_move);
	view_topleft = subtract_vec(view_topleft, horizontal_move);
	set_pixel00(trace, view_topleft, right, true_up);
}

void	reinit_viewing(t_trace *trace)
{
	t_point	view_center;
	double	focal_len;
	double	view_width;
	double	view_height;
	
	trace->cam->orient = norm_vec(trace->cam->orient);
	focal_len = 1.0;
	view_center = add_vec(trace->cam->center, scale_vec(1.0 / focal_len, trace->cam->orient));
	view_width = 2.0 * tan((double)((trace->cam->fov) / 2.0) * DEG_TO_RAD);
	view_height = view_width / ASPECT;
	trace->pixel_width = view_width / (double)trace->width;
	trace->pixel_height = view_height / (double)trace->height;	
	reset_topleft(trace, view_center, view_width, view_height);
}

//moves current "on" object in x,y,z

void	translate_object(t_trace *trace, t_on *on, t_vec3 vec)
{
	if (on->object == NULL)
		return ;
	if (on->type == SPHERE)
	{
		trace->curr_sp->curr_rottran = mat_mult(trace->curr_sp->curr_rottran, translation(-vec.x, -vec.y, -vec.z));
		trace->curr_sp->transform = mat_mult(trace->curr_sp->curr_scale, trace->curr_sp->curr_rottran);
	}
	else if (on->type == PLANE)
	{
		trace->curr_pl->curr_rottran = mat_mult(trace->curr_pl->curr_rottran, translation(-vec.x, -vec.y, -vec.z));
		trace->curr_pl->transform = mat_mult(trace->curr_pl->curr_scale, trace->curr_pl->curr_rottran);
	}
	else if (on->type == CYLINDER)
	{
		trace->curr_cy->curr_rottran = mat_mult(trace->curr_cy->curr_rottran, translation(-vec.x, -vec.y, -vec.z));
		trace->curr_cy->transform = mat_mult(trace->curr_cy->curr_scale, trace->curr_cy->curr_rottran);
	}
	else if (on->type == LIGHT)
		trace->lights->center = add_vec(trace->lights->center, vec);
	else if (on->type == CAM)
	{
		trace->cam->center = add_vec(trace->cam->center, vec);
		reinit_viewing(trace);
	}
}

// rotates current "on" object

void	rotate_object(t_trace *trace, t_on *on, t_matrix_4x4 rot)
{
	if (on->object == NULL)
		return ;
	else if (on->type == SPHERE)
	{
		trace->curr_sp->curr_rottran = mat_mult(rot, trace->curr_sp->curr_rottran);
		trace->curr_sp->transform = mat_mult(trace->curr_sp->curr_scale, trace->curr_sp->curr_rottran);
	}
	else if (on->type == PLANE)
	{	
		trace->curr_pl->curr_rottran = mat_mult(rot, trace->curr_pl->curr_rottran);
		trace->curr_pl->transform = mat_mult(trace->curr_pl->curr_scale, trace->curr_pl->curr_rottran);
		trace->curr_pl->norm = norm_vec(mat_vec_mult(transpose(trace->curr_pl->transform), vec(0, 1, 0, 0)));
	}
	else if (on->type == CYLINDER)
	{
		trace->curr_cy->curr_rottran = mat_mult(rot, trace->curr_cy->curr_rottran);
		trace->curr_cy->transform = mat_mult(trace->curr_cy->curr_scale, trace->curr_cy->curr_rottran);
	}
	else if (on->type == CAM)
	{
		/* t_matrix_4x4	curr_rot;//huh?
		get_rotation(trace->cam->orient, double cos(M_PI / 6), double sin(M_PI / 6)); */

		//stack the matrix ops... to rotate the camera //its not working for object space rotation
		//print_matrix_4(&trace->cam->transform);
		trace->cam->transform = mat_mult(rot, trace->cam->transform);
		trace->cam->orient = norm_vec(mat_vec_mult(trace->cam->transform, vec(0, 1, 0, 0)));
		trace->cam->true_up = norm_vec(mat_vec_mult(trace->cam->transform, vec(0.0, 0.0, -1.0, 0.0)));

		/* trace->cam->orient = norm_vec(mat_vec_mult(trace->cam->transform, trace->cam->orient));
		trace->cam->true_up = norm_vec(mat_vec_mult(trace->cam->transform, trace->cam->true_up));		
 */

		/* trace->cam->orient = norm_vec(mat_vec_mult(rot, norm_vec(trace->cam->orient)));
		trace->cam->true_up = norm_vec(mat_vec_mult(rot, norm_vec(trace->cam->true_up))); */
		reinit_viewing(trace);
	}
	else
		return ;
}

//scales current object in xyz based on the vec passed in

void	scale_object(t_trace *trace, t_on *on, t_vec3 vec)
{
	if (on->object == NULL)
		return ;
	if (on->type == SPHERE)
	{
		trace->curr_sp->curr_scale = mat_mult(inv_scaling(vec.x, vec.y, vec.z), trace->curr_sp->curr_scale);
		trace->curr_sp->transform = mat_mult(trace->curr_sp->curr_scale, trace->curr_sp->curr_rottran);
	}
	else if (on->type == PLANE)
		trace->curr_pl->transform = mat_mult(inv_scaling(vec.x, vec.y, vec.z), trace->curr_pl->transform);
	else if (on->type == CYLINDER)
	{
		trace->curr_cy->curr_scale = mat_mult(inv_scaling(vec.x, vec.y, vec.z), trace->curr_cy->curr_scale);
		trace->curr_cy->transform = mat_mult(trace->curr_cy->curr_scale, trace->curr_cy->curr_rottran);
	}
	else
		return ;

/* 	else if (on->type == LIGHT)//maybe this can scale the spotlight radii?
		trace->lights->center = add_vec(trace->lights->center, vec);
	else if (on->type == CAM)// no use case yet?!
	{
		trace->cam->center = add_vec(trace->cam->center, vec);
		init_viewing(trace);
	} */
}

void	push_new_object(t_trace *trace, t_on *on)
{
	if (on->type == SPHERE)
	{
		if (insert_spcopy_after(trace, &trace->curr_sp))
			close_win(trace);
	}
	else if (on->type == PLANE)
	{
		if (insert_plcopy_after(trace, &trace->curr_pl))
			close_win(trace);	
	}
	else if (on->type == CYLINDER)
	{
		if (insert_cycopy_after(trace, &trace->curr_cy))
			close_win(trace);	
	}
	else
		return ;
	/* else if (on->type == LIGHT)////---------------
	{
		if (insert_ltcopy_after(trace, &trace->lights))
			close_win(trace);	
	} */
	next_list_ob(trace, trace->on);
}

void	pop_object(t_trace *trace, t_on *on)
{
	if (on->type == SPHERE)
		pop_sp(trace, &trace->curr_sp);
	else if (on->type == PLANE)
		pop_pl(trace, &trace->curr_pl);
	else if (on->type == CYLINDER)
		pop_cy(trace, &trace->curr_cy);
	else
		return ;
}
