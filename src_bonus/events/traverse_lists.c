# include "minirt.h"

static inline void	next_list_ob2(t_trace *trace, t_on *on)
{
	t_cylinder		*curr_cy;
	t_lens			*curr_le;
	t_light			*curr_lt;

	if (on->type == CYLINDER)
	{
		curr_cy = (t_cylinder *)on->object;
		trace->curr_cy = curr_cy->next;
		on->object = trace->curr_cy;
	}
	else if (on->type == LENS)
	{
		curr_le = (t_lens *)on->object;
		trace->curr_le = curr_le->next;
		on->object = trace->curr_le;
	}
	else if (on->type == LIGHT)
	{
		curr_lt = (t_light *)on->object;
		trace->curr_lt = curr_lt->next;
		on->object = trace->curr_lt;
	}
	else
		return ;
}

// go the the next object on the current list, else is cam or non list object

void	next_list_ob(t_trace *trace, t_on *on)
{
	t_sphere		*curr_sp;
	t_plane 		*curr_pl;

	if (on->object == NULL)
		return ;
	if (on->type == SPHERE)
	{
		curr_sp = (t_sphere *)on->object;
		trace->curr_sp = curr_sp->next;
		on->object = trace->curr_sp;
	}
	else if (on->type == PLANE)
	{
		curr_pl = (t_plane *)on->object;
		trace->curr_pl = curr_pl->next;
		on->object = trace->curr_pl;
	}
	else
		next_list_ob2(trace, on);	
}

static inline void	prev_list_ob2(t_trace *trace, t_on *on)
{
	t_cylinder		*curr_cy;
	t_lens			*curr_le;
	t_light			*curr_lt;

	if (on->type == CYLINDER)
	{
		curr_cy = (t_cylinder *)on->object;
		trace->curr_cy = curr_cy->prev;
		on->object = trace->curr_cy;
	}
	else if (on->type == LENS)
	{
		curr_le = (t_lens *)on->object;
		trace->curr_le = curr_le->prev;
		on->object = trace->curr_le;
	}
	else if (on->type == LIGHT)
	{
		curr_lt = (t_light *)on->object;
		trace->curr_lt = curr_lt->prev;
		on->object = trace->curr_lt;
	}
	else
		return ;
}

//go to the previous object on each list

void	prev_list_ob(t_trace *trace, t_on *on)
{
	t_sphere		*curr_sp;
	t_plane 		*curr_pl;

	if (on->object == NULL)
		return ;
	if (on->type == SPHERE)
	{
		curr_sp = (t_sphere *)on->object;
		trace->curr_sp = curr_sp->prev;
		on->object = trace->curr_sp;
	}
	else if (on->type == PLANE)
	{
		curr_pl = (t_plane *)on->object;
		trace->curr_pl = curr_pl->prev;
		on->object = trace->curr_pl;
	}
	else
		prev_list_ob2(trace, on);
}