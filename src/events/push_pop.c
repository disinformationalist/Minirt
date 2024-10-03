#include "minirt.h"

/* static inline void push_new2(t_trace *trace, t_on *on)
{
	//else if (on->type == LIGHT)////---------------
	//{
	//	if (insert_ltcopy_after(trace, &trace->lights))
	//		close_win(trace);	
	//}
	return ;
} */

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
