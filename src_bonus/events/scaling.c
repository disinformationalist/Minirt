#include "minirt.h"

static inline void scale_object2(t_trace *trace, t_on *on, t_vec3 vec1)
{
	if (on->type == LENS)
	{
		trace->curr_le->sphere_1.curr_scale = mat_mult(inv_scaling(vec1.x, vec1.y, vec1.z), trace->curr_le->sphere_1.curr_scale);
		trace->curr_le->sphere_1.transform = mat_mult(trace->curr_le->sphere_1.curr_scale, trace->curr_le->sphere_1.curr_rottran);
		trace->curr_le->sphere_2.curr_scale = mat_mult(inv_scaling(vec1.x, vec1.y, vec1.z), trace->curr_le->sphere_2.curr_scale);
		trace->curr_le->sphere_2.transform = mat_mult(trace->curr_le->sphere_2.curr_scale, trace->curr_le->sphere_2.curr_rottran);
	}
	else if (on->type == LIGHT)//maybe this can scale the spotlight radii?
	{
		trace->curr_lt->brightness *= vec1.y;
		if (trace->curr_lt->brightness > 1.0)
			trace->curr_lt->brightness = 1.0;
		if (trace->curr_lt->brightness < 0.05)
			trace->curr_lt->brightness = 0.05;
	}
	else
		return ;
	/* else if (on->type == CAM)// no use case yet?! fov?
	{
		trace->cam->center = add_vec(trace->cam->center, vec1);
		init_viewing(trace);
	} */
}

//scales current object in xyz based on the vec1 passed in

void	scale_object(t_trace *trace, t_on *on, t_vec3 vec1)
{
	if (on->object == NULL)
		return ;
	if (on->type == SPHERE)
	{
		trace->curr_sp->curr_scale = mat_mult(inv_scaling(vec1.x, \
			vec1.y, vec1.z), trace->curr_sp->curr_scale);
		trace->curr_sp->transform = mat_mult(trace->curr_sp->curr_scale, \
			trace->curr_sp->curr_rottran);
	}
	else if (on->type == PLANE)
		trace->curr_pl->transform = mat_mult(inv_scaling(vec1.x, vec1.y, \
			vec1.z), trace->curr_pl->transform);
	else if (on->type == CYLINDER)
	{
		trace->curr_cy->curr_scale = mat_mult(inv_scaling(vec1.x, vec1.y, \
			vec1.z), trace->curr_cy->curr_scale);
		trace->curr_cy->transform = mat_mult(trace->curr_cy->curr_scale, \
			trace->curr_cy->curr_rottran);
	}
	else
		scale_object2(trace, on, vec1);
}
