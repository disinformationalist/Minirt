#include "minirt.h"
		
static inline void	scale_arealt(t_light *lt, t_vec3 v)
{
	lt->curr_scale = mat_mult(scaling(v.x, 1.0, v.z), lt->curr_scale);
	lt->transform = mat_mult(lt->curr_scale, lt->curr_rottran);
	lt->emitter->curr_scale = mat_mult(inv_scaling(v.x, 1.0, v.z), lt->emitter->curr_scale);
	lt->emitter->transform = mat_mult(lt->emitter->curr_scale, lt->emitter->curr_rottran);
	lt->emitter->color = mult_color(1.0 / lt->emitter->bright, lt->emitter->color);
	lt->emitter->color = mult_color(lt->brightness, lt->emitter->color);
	lt->emitter->bright = lt->brightness;
	set_arealt(lt);
}

static inline void scale_object3(t_trace *trace, t_on *on, t_vec3 vec1)
{
	if (on->type == CUBE)
	{
		trace->curr_cu->curr_scale = mat_mult(inv_scaling(vec1.x, vec1.y, \
			vec1.z), trace->curr_cu->curr_scale);
		trace->curr_cu->transform = mat_mult(trace->curr_cu->curr_scale, \
			trace->curr_cu->curr_rottran);
		trace->curr_cu->t_transform = transpose(trace->curr_cu->transform);
		trace->curr_cu->i_transform = inverse(trace->curr_cu->transform);
	}
	else if (on->type == LIGHT)
	{
		trace->curr_lt->brightness *= vec1.y;
		if (trace->curr_lt->brightness > 1.0)
			trace->curr_lt->brightness = 1.0;
		if (trace->curr_lt->brightness < 0.05)
			trace->curr_lt->brightness = 0.05;
		if (trace->curr_lt->type == AREA)
			scale_arealt(trace->curr_lt, vec1);
	}
	else
		return ;
}

static inline void scale_object2(t_trace *trace, t_on *on, t_vec3 vec1)
{
	if (on->type == LENS)
	{
		trace->curr_le->sphere_1.curr_scale = mat_mult(inv_scaling(vec1.x, vec1.y, vec1.z), trace->curr_le->sphere_1.curr_scale);
		trace->curr_le->sphere_1.transform = mat_mult(trace->curr_le->sphere_1.curr_scale, trace->curr_le->sphere_1.curr_rottran);
		trace->curr_le->sphere_2.curr_scale = mat_mult(inv_scaling(vec1.x, vec1.y, vec1.z), trace->curr_le->sphere_2.curr_scale);
		trace->curr_le->sphere_2.transform = mat_mult(trace->curr_le->sphere_2.curr_scale, trace->curr_le->sphere_2.curr_rottran);
	}
	else if (on->type == CYLINDER)
	{
		trace->curr_cy->curr_scale = mat_mult(inv_scaling(vec1.x, vec1.y, \
			vec1.z), trace->curr_cy->curr_scale);
		trace->curr_cy->transform = mat_mult(trace->curr_cy->curr_scale, \
			trace->curr_cy->curr_rottran);
		trace->curr_cy->t_transform = transpose(trace->curr_cy->transform);
		trace->curr_cy->i_transform = inverse(trace->curr_cy->transform);
	}
	else if (on->type == PLANE)
	{
		trace->curr_pl->curr_scale = mat_mult(inv_scaling(vec1.x, 1.0, \
			vec1.z), trace->curr_pl->curr_scale);
		trace->curr_pl->transform = mat_mult(trace->curr_pl->curr_scale, \
			trace->curr_pl->curr_rottran);
		trace->curr_pl->t_transform = transpose(trace->curr_pl->transform);
		trace->curr_pl->i_transform = inverse(trace->curr_pl->transform);
	}
	else
		scale_object3(trace, on, vec1);
}

//scales current object in xyz based on the vec1 passed in

void	scale_object(t_trace *trace, t_on *on, t_vec3 vec1)
{
	if (on->object == NULL && on->type != LIGHT)
		return ;															//maybe keep sl al counts in trace..adjust to insert properly into list..
	else if (trace->lights == NULL && on->type == LIGHT)//needs to work when on spots exist or no areas exist... right now only works when list is totally empty..
	{
		if (vec1.x + vec1.y + vec1.z > 3)
		{
			if (insert_ltcopy_after2(trace))
				close_win(trace);
		}
		else if (vec1.x + vec1.y + vec1.z > 2.6)
		{
			if (insert_ltcopy_after3(trace))
				close_win(trace);
		}
		return ;
	}
	if (on->type == SPHERE)
	{
		trace->curr_sp->curr_scale = mat_mult(inv_scaling(vec1.x, \
			vec1.y, vec1.z), trace->curr_sp->curr_scale);
		trace->curr_sp->transform = mat_mult(trace->curr_sp->curr_scale, \
			trace->curr_sp->curr_rottran);
		trace->curr_sp->t_transform = transpose(trace->curr_sp->transform);
		trace->curr_sp->i_transform = inverse(trace->curr_sp->transform);
	}
	else
		scale_object2(trace, on, vec1);
}
