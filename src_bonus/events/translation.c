#include "minirt.h"

//camera translates in "cam space" for best use

static inline void	translate_cam(t_trace *trace, t_vec3 vec1)
{
	t_vec3	right;
	t_vec3	move;

	right = cross_prod(trace->cam->true_up, trace->cam->orient);
	move = scale_vec(vec1.x , right);
	move = add_vec(move, scale_vec(vec1.y , trace->cam->true_up));
	move = add_vec(move, scale_vec(vec1.z , trace->cam->orient));
	trace->cam->center = add_vec(trace->cam->center, move);
	reinit_viewing(trace);
}

void move_arealt(t_light *lt, t_vec3 v)
{
	lt->curr_rottran = mat_mult(lt->curr_rottran, translation(v.x, v.y, v.z));
	lt->transform = mat_mult(lt->curr_scale, lt->curr_rottran);
	lt->emitter->curr_rottran = mat_mult(lt->emitter->curr_rottran, translation(-v.x, -v.y, -v.z));
	lt->emitter->transform = mat_mult(lt->emitter->curr_scale, lt->emitter->curr_rottran);
	set_arealt(lt);
}


// just a continuation of the function below it

static inline void	translate_object2(t_trace *trace, t_on *on, t_vec3 vec1)
{
	if (on->type == HYPERBOLOID)
	{
		trace->curr_hy->curr_rottran = mat_mult(trace->curr_hy->curr_rottran, translation(-vec1.x, -vec1.y, -vec1.z));
		trace->curr_hy->transform = mat_mult(trace->curr_hy->curr_scale, trace->curr_hy->curr_rottran);
		trace->curr_hy->t_transform = transpose(trace->curr_hy->transform);
		trace->curr_hy->i_transform = inverse(trace->curr_hy->transform);
	}
	else if (on->type == CUBE)
	{
		trace->curr_cu->curr_rottran = mat_mult(trace->curr_cu->curr_rottran, translation(-vec1.x, -vec1.y, -vec1.z));
		trace->curr_cu->transform = mat_mult(trace->curr_cu->curr_scale, trace->curr_cu->curr_rottran);
		trace->curr_cu->t_transform = transpose(trace->curr_cu->transform);
		trace->curr_cu->i_transform = inverse(trace->curr_cu->transform);
	}
	else if (on->type == LIGHT)
	{
		trace->curr_lt->center = add_vec(trace->curr_lt->center, vec1);
		if (trace->curr_lt->type == AREA)
			move_arealt(trace->curr_lt, vec1);
	}
	else if (on->type == CAM)
		translate_cam(trace, vec1);
	else
		return ;
}

//moves current "on" object in x,y,z

void	translate_object(t_trace *trace, t_on *on, t_vec3 vec1)
{
	if (on->object == NULL)
		return ;
	if (on->type == SPHERE)
	{
		trace->curr_sp->curr_rottran = mat_mult(trace->curr_sp->curr_rottran, translation(-vec1.x, -vec1.y, -vec1.z));
		trace->curr_sp->transform = mat_mult(trace->curr_sp->curr_scale, trace->curr_sp->curr_rottran);
		trace->curr_sp->t_transform = transpose(trace->curr_sp->transform);
		trace->curr_sp->i_transform = inverse(trace->curr_sp->transform);
	}
	else if (on->type == PLANE)
	{
		trace->curr_pl->curr_rottran = mat_mult(trace->curr_pl->curr_rottran, translation(-vec1.x, -vec1.y, -vec1.z));
		trace->curr_pl->transform = mat_mult(trace->curr_pl->curr_scale, trace->curr_pl->curr_rottran);
		trace->curr_pl->t_transform = transpose(trace->curr_pl->transform);
		trace->curr_pl->i_transform = inverse(trace->curr_pl->transform);
	}
	else if (on->type == CYLINDER)
	{
		trace->curr_cy->curr_rottran = mat_mult(trace->curr_cy->curr_rottran, translation(-vec1.x, -vec1.y, -vec1.z));
		trace->curr_cy->transform = mat_mult(trace->curr_cy->curr_scale, trace->curr_cy->curr_rottran);
		trace->curr_cy->t_transform = transpose(trace->curr_cy->transform);
		trace->curr_cy->i_transform = inverse(trace->curr_cy->transform);
	}
	else
		translate_object2(trace, on, vec1);
}
