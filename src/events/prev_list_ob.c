#include "minirt.h"

static inline void	prev_list_ob3(t_trace *trace, t_on *on)
{
	t_cube			*curr_cu;
	t_hyperboloid	*curr_hy;

	if (on->type == HYPERBOLOID)
	{
		curr_hy = (t_hyperboloid *)on->object;
		trace->curr_hy = curr_hy->prev;
		on->object = trace->curr_hy;
	}
	else if (on->type == CUBE)
	{
		curr_cu = (t_cube *)on->object;
		trace->curr_cu = curr_cu->prev;
		on->object = trace->curr_cu;
	}
	else
		return ;
}

static inline void	prev_list_ob2(t_trace *trace, t_on *on)
{
	t_cylinder		*curr_cy;
	t_light			*curr_lt;

	if (on->type == CYLINDER)
	{
		curr_cy = (t_cylinder *)on->object;
		trace->curr_cy = curr_cy->prev;
		on->object = trace->curr_cy;
	}
	else if (on->type == LIGHT)
	{
		curr_lt = (t_light *)on->object;
		trace->curr_lt = curr_lt->prev;
		on->object = trace->curr_lt;
	}
	else
		prev_list_ob3(trace, on);
}

//go to the previous object on each list

void	prev_list_ob(t_trace *trace, t_on *on)
{
	t_sphere		*curr_sp;
	t_plane			*curr_pl;

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
	if (trace->menu_open)
		reset_con_non(trace);
}
