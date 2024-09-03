# include "minirt.h"

//move which list the current object is on

void	switch_list(int keycode, t_trace *trace, t_on *on)
{
	if (keycode == N_1)
	{
		on->object = trace->curr_sp;// use the current spot on list
		on->type = SPHERE;
	}
	else if (keycode == N_2)
	{
		on->object = trace->curr_pl;
		on->type = PLANE;
	}
	else if (keycode == N_3)
	{
		on->object = trace->curr_cy;
		on->type = CYLINDER;
	}
	//fourth type here
}

// go the the next object on the current list

void	next_list_ob(t_trace *trace, t_on *on)
{
	t_sphere		*curr_sp;
	t_plane 		*curr_pl;
	t_cylinder		*curr_cy;

	if (on->type == SPHERE)
	{
		curr_sp = (t_sphere *)on->object;// typecast current "on" object
		trace->curr_sp = curr_sp->next;//keep track of where we are in list as moving to next
		on->object = trace->curr_sp;//assign object to current spot in list
	}
	else if (on->type == PLANE)
	{
		curr_pl = (t_plane *)on->object;
		trace->curr_pl = curr_pl->next;
		on->object = trace->curr_pl;
	}
	else if (on->type == CYLINDER)
	{
		curr_cy = (t_cylinder *)on->object;
		trace->curr_cy = curr_cy->next;
		on->object = trace->curr_cy;
	}
	else// on cam or nonlist object
		return ;
}

//go to the previous object on each list

void	prev_list_ob(t_trace *trace, t_on *on)
{
	t_sphere		*curr_sp;
	t_plane 		*curr_pl;
	t_cylinder		*curr_cy;

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
	else if (on->type == CYLINDER)
	{
		curr_cy = (t_cylinder *)on->object;
		trace->curr_cy = curr_cy->prev;
		on->object = trace->curr_cy;
	}
	else//on cam
		return ;
}