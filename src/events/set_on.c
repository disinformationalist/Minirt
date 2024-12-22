#include "minirt.h"

// set the on and current object clicked by mouse

void	set_mouse_on(t_trace *trace, t_track_hits *closest)
{
	void			*obj;
	t_type			type;

	obj = closest->object;
	type = closest->object_type;
	trace->on->object = obj;
	trace->on->type = type;
	if (obj == NULL)
		return ;
	else if (type == SPHERE)
		trace->curr_sp = (t_sphere *)obj;
	else if (type == PLANE)
		trace->curr_pl = (t_plane *)obj;
	else if (type == CYLINDER)
		trace->curr_cy = (t_cylinder *)obj;
	else if (type == HYPERBOLOID)
		trace->curr_hy = (t_hyperboloid *)obj;
	else if (type == CUBE)
		trace->curr_cu = (t_cube *)obj;
	else if (type == LIGHT)
		trace->curr_lt = (t_light *)obj;
	else
		return ;
}
