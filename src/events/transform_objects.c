#include "minirt.h"

//moves current "on" object in x,y,z

void	translate_object(t_trace *trace, t_on *on, t_vec3 vec)
{
	if (on->object == NULL)
		return ;
	if (on->type == SPHERE)
		trace->curr_sp->transform = mat_mult(trace->curr_sp->transform, translation(-vec.x, -vec.y, -vec.z));
	else if (on->type == PLANE)
		trace->curr_pl->transform = mat_mult(trace->curr_pl->transform, translation(-vec.x, -vec.y, -vec.z));
	else if (on->type == CYLINDER)
		trace->curr_cy->transform = mat_mult(trace->curr_cy->transform, translation(-vec.x, -vec.y, -vec.z));
	else if (on->type == LIGHT)
		trace->lights->center = add_vec(trace->lights->center, vec);
	else if (on->type == CAM)
	{
		trace->cam->center = add_vec(trace->cam->center, vec);
		init_viewing(trace);
	}
}

// rotates current "on" object

void	rotate_object(t_trace *trace, t_on *on, t_matrix_4x4 rot)
{
	if (on->object == NULL)
		return ;
	else if (on->type == SPHERE)
		trace->curr_sp->transform = mat_mult(rot, trace->curr_sp->transform);
	else if (on->type == PLANE)
	{	
		trace->curr_pl->transform = mat_mult(rot, trace->curr_pl->transform);
		trace->curr_pl->norm = norm_vec(mat_vec_mult(transpose(trace->curr_pl->transform), vec(0, 1, 0, 0)));
	}
	else if (on->type == CYLINDER)
		trace->curr_cy->transform = mat_mult(rot, trace->curr_cy->transform);
	else if (on->type == CAM)
	{
		trace->cam->orient = norm_vec(mat_vec_mult(rot, norm_vec(trace->cam->orient)));
		init_viewing(trace);
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
		trace->curr_sp->transform = mat_mult(inv_scaling(vec.x, vec.y, vec.z), trace->curr_sp->transform);
	else if (on->type == PLANE)
		trace->curr_pl->transform = mat_mult(inv_scaling(vec.x, vec.y, vec.z), trace->curr_pl->transform);
	else if (on->type == CYLINDER)
		trace->curr_cy->transform = mat_mult(inv_scaling(vec.x, vec.y, vec.z), trace->curr_cy->transform);

/* 	else if (on->type == LIGHT)//maybe this can scale the spotlight radii?
		trace->lights->center = add_vec(trace->lights->center, vec);
	else if (on->type == CAM)
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
