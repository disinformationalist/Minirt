# include "minirt.h"
	/**************** "ON" DIAGRAM *****************/
	/*
	ON has pointer to current object to be manipulated

		all list objects are doubly linked circular

   		(spheres)		(other)			(planes)
		SP == SP	(4th ob, bonus)	    PL == PL
		\\	  //	==>         ==>     \\	  //
		   SP							   PL

	       ||			  ON			   ||
		   ||		(current object)	   ||
		   ||		 (moves to any)        ||

		LT == LT						CY == CY
		\\    //     ==>  CAM   ==>    	\\	  //
	   	   LT							   CY	
		(lights)					   (cylinders)
----------------------------------------------------------*/

//move which list the current object is on

void	switch_list(int keycode, t_trace *trace, t_on *on)
{
	if (keycode == N_1)
	{
		on->object = trace->curr_sp;// curr_ is the current spot on list stored in trace to keep track
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
	else if (keycode == N_3)
	{
		on->object = trace->curr_cy;
		on->type = CYLINDER;
	}
	else if (keycode == N_9)
	{
		on->object = trace->lights;
		on->type = LIGHT;
	}
	else if (keycode == N_0)
	{
		on->object = trace->cam;
		on->type = CAM;
	}
}

// go the the next object on the current list, else is cam or non list object

void	next_list_ob(t_trace *trace, t_on *on)
{
	t_sphere		*curr_sp;
	t_plane 		*curr_pl;
	t_cylinder		*curr_cy;

	if (on->type == SPHERE)
	{
		curr_sp = (t_sphere *)on->object;// typecast current "on" object so we can operate upon it
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
	else
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
	else
		return ;
}