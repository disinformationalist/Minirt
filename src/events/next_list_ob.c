#include "minirt.h"

static inline void	next_list_ob3(t_trace *trace, t_on *on)
{
	t_cube			*curr_cu;
	t_hyperboloid	*curr_hy;

	if (on->type == HYPERBOLOID)
	{
		curr_hy = (t_hyperboloid *)on->object;
		trace->curr_hy = curr_hy->next;
		on->object = trace->curr_hy;
	}
	if (on->type == CUBE)
	{
		curr_cu = (t_cube *)on->object;
		trace->curr_cu = curr_cu->next;
		on->object = trace->curr_cu;
	}
	else
		return ;
}

static inline void	next_list_ob2(t_trace *trace, t_on *on)
{
	t_cylinder		*curr_cy;
	t_light			*curr_lt;

	if (on->type == CYLINDER)
	{
		curr_cy = (t_cylinder *)on->object;
		trace->curr_cy = curr_cy->next;
		on->object = trace->curr_cy;
	}
	else if (on->type == LIGHT)
	{
		curr_lt = (t_light *)on->object;
		trace->curr_lt = curr_lt->next;
		on->object = trace->curr_lt;
	}
	else
		next_list_ob3(trace, on);
}

// go the the next object on the current list, else is cam or non list object

void	next_list_ob(t_trace *trace, t_on *on)
{
	t_sphere		*curr_sp;
	t_plane			*curr_pl;

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
	if (trace->menu_open)
		reset_con_non(trace);
}
