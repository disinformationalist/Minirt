#include "minirt.h"

char *get_type(t_type type)
{
	if (type == SPHERE)
		return ("SPHERE");
	else if (type == PLANE)
		return ("PLANE");
	else if (type == CYLINDER)
		return ("CYLINDER");
	else if (type == HYPERBOLOID)
		return ("HYPERBOLOID");
	else if (type == CUBE)
		return ("CUBE");
	else if (type == TRI)
		return ("TRI");
	else if (type == LIGHT)
		return ("LIGHT");
	else if (type == CAM)
		return ("CAM");
	else if (type == VOID)
		return ("VOID");
	else
		return ("UNIDENTIFIED OBJECT");
}

/* cast a ray through the center of mouse pixel(adjusted to viewport)
and get first hit in order to find what object we are on. Also useful 
as a single ray cast testing tool */
/* //print selected object info
printf("\n");
printf("object enum: %s\n", get_type(intersects->closest->object_type));
printf("t val: %f\n", intersects->closest->t);
printf("object pointer: %p\n\n", intersects->closest->object); */

void	track_object(t_trace *trace, double x, double y)
{
	t_ray	r;
	t_vec3	current_pixel;
	t_intersects *intersects;

	current_pixel = add_vec(trace->pixel00, scale_vec(x, trace->pix_delta_rht));
	current_pixel = add_vec(current_pixel, scale_vec(y, trace->pix_delta_down));
	r.origin = trace->cam->center;
	r.dir = norm_vec(subtract_vec(current_pixel, r.origin));

	intersects = create_ints(trace->total_ints);
	if (!intersects)
		close_win(trace) ;//handle failure
	find_closest(trace, r, intersects);
	set_mouse_on(trace, intersects->closest);
	if (trace->on->object)
	{
		trace->dragging = true;
		trace->low_inc = 50;
		trace->start_x = x;
		trace->start_y = y;
	}
	free(intersects->hits);
	free(intersects->closest);
	free(intersects);
}


int	mouse_release(int button, int x, int y, t_trace *trace)
{
	(void)x;
	(void)y;
	if (button == 1)
	{
		trace->dragging = false;
		trace->low_res = false;
		render(trace);
	}
	return (0);
}

int key_release(int keycode, t_trace *trace)
{
	if (keycode == SHIFT)
		trace->shift_on = false;
	return (0);
}

int mouse_move(int x, int y, t_trace *trace)
{
	double	delta_x;
	double	delta_y;
	t_vec3	move;
	
	if (trace->on->object && trace->dragging)
	{
		trace->low_res = true;
		delta_x = (x - trace->start_x) * 10;
		delta_y = (y - trace->start_y) * 10;
		if (trace->shift_on)
			move = vec(0, 0, delta_y / 50, 0);
		else
		{
			move = add_vec(scale_vec(delta_x, trace->pix_delta_rht), 
						scale_vec(delta_y, trace->pix_delta_down));
		}
		translate_object(trace, trace->on, move);
		trace->start_x = x;
		trace->start_y = y;
		render(trace);
	}
	return (0);
}