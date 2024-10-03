#include "minirt.h"

	/**************** "ON" DIAGRAM *****************/
	/*
	ON is a single struct object as is cam, and light in non bonus
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

static inline void	switch_list2(int keycode, t_trace *trace, t_on *on)
{
	if (keycode == N_9)
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
	else
		switch_list2(keycode, trace, on);
}
