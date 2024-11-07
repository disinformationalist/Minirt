#include "minirt.h"

static inline void push_new2(t_trace *trace, t_on *on)
{
	if (on->type == LENS)
	{
		if (insert_lecopy_after(trace, &trace->curr_le))
			close_win(trace);	
	}
	else if (on->type == LIGHT)
	{
		if (insert_ltcopy_after(trace, &trace->curr_lt))
			close_win(trace);	
	}
	else if (on->type == CUBE)
	{
		if (insert_cucopy_after(trace, &trace->curr_cu))
			close_win(trace);	
	}
	return ;
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
		push_new2(trace, on);
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
	else if (on->type == LENS)
		pop_le(trace, &trace->curr_le);
	else if (on->type == LIGHT)
		pop_lt(trace, &trace->curr_lt);
	else if (on->type == CUBE)
		pop_cu(trace, &trace->curr_cu);
	else
		return ;
}
