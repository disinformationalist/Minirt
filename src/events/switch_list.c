#include "minirt.h"

	/**************** "ON" DIAGRAM *****************/
	/*
	ON is a single struct object as is cam, and light in non bonus
	ON has pointer to current object to be manipulated

		all list objects are doubly linked circular

   		(spheres)		(other)			(planes)
		SP == SP	(objs, bonus)	    PL == PL
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

static inline void	switch_list2(int keycode, t_trace *trace, t_on *on)
{
	if (keycode == N_9)
	{
		on->object = trace->curr_lt;
		on->type = LIGHT;
	}
	else if (keycode == N_0)
	{
		on->object = trace->cam;
		on->type = CAM;
	}
	else if (keycode == N_5)
	{
		on->object = trace->curr_cu;
		on->type = CUBE;
	}
}

/* move which list the current object is on
curr_ is the current spot on list stored in trace to keep track */

void	switch_list(int keycode, t_trace *trace, t_on *on)
{
	if (keycode == N_1)
	{
		on->object = trace->curr_sp;
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
	else if (keycode == N_4)
	{
		on->object = trace->curr_hy;
		on->type = HYPERBOLOID;
	}
	else
		switch_list2(keycode, trace, on);
	if (trace->menu_open)
	{
		trace->obj_control->pos_open = false;
		trace->obj_control->rot_open = false;
		trace->obj_control->sca_open = false;
		trace->obj_control->on_dials = false;
		reset_con_non(trace);
	}
}

void	next_list(t_trace *trace, t_on *on)
{
	t_type	type;

	type = on->type;
	if (type == VOID)
	{
		on->object = trace->curr_sp;
		on->type = SPHERE;
	}
	else if (type == SPHERE)
	{
		on->object = trace->curr_pl;
		on->type = PLANE;
	}
	else if (type == PLANE)
	{
		on->object = trace->curr_cy;
		on->type = CYLINDER;
	}
	else if (type == CYLINDER)
	{
		on->object = trace->curr_hy;
		on->type = HYPERBOLOID;
	}
	else if (type == HYPERBOLOID)
	{
		on->object = trace->curr_cu;
		on->type = CUBE;
	}
	else if (type == CUBE)
	{
		on->object = trace->curr_lt;
		on->type = LIGHT;
	}
	else if (type == LIGHT)
	{
		on->object = trace->cam;
		on->type = CAM;
	}
	else if (type == CAM)
	{
		on->object = trace->curr_sp;
		on->type = SPHERE;
	}
}

void	prev_list(t_trace *trace, t_on *on)
{
	t_type	type;

	type = on->type;
	if (type == VOID)
	{
		on->object = trace->cam;
		on->type = CAM;
	}
	else if (type == SPHERE)
	{
		on->object = trace->cam;
		on->type = CAM;
	}
	else if (type == CAM)
	{
		on->object = trace->curr_lt;
		on->type = LIGHT;
	}
	else if (type == LIGHT)
	{
		on->object = trace->curr_cu;
		on->type = CUBE;
	}
	else if (type == CUBE)
	{
		on->object = trace->curr_hy;
		on->type = HYPERBOLOID;
	}
	else if (type == HYPERBOLOID)
	{
		on->object = trace->curr_cy;
		on->type = CYLINDER;
	}
	else if (type == CYLINDER)
	{
		on->object = trace->curr_pl;
		on->type = PLANE;
	}
	else if (type == PLANE)
	{
		on->object = trace->curr_sp;
		on->type = SPHERE;
	}
}

/* typedef enum e_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	HYPERBOLOID,
	CUBE,
	TRI,
	LIGHT,
	CSG,
	GROUP,
	CAM,
	MESH,
	VOID
}	t_type; */