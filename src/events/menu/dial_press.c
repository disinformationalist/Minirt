#include "minirt.h"

static inline bool	in_circle(int x, int y, int cx, int cy, int rad)
{	
	double	dx;
	double	dy;
	double	dist2;

	dx = x - cx;
	dy = y - cy;
	dist2 = dx * dx + dy *dy;
	return (dist2 <= rad * rad);
}

static inline void	set_rot_knob(t_trace *trace, int knob, double *start, int x, int y)
{
	trace->dragging = true;
	*start = atan2(y, x);
	trace->knob = knob;
}

int	rotation_press(int x, int y, t_trace *trace, t_control control)
{
	//rotation knobs 11, 12, 13 for x, y, z //rotx, 10 = bump knob
	if (in_circle(x, y, control.rotsx.i + 100, -control.rotsx.j + control.dials_ys + 120, 8))
		set_rot_knob(trace, 11, &trace->start_xangle, control.rotsx.i, control.rotsx.j);
	else if (in_circle(x, y, control.rotsy.i + 100, -control.rotsy.j + control.dials_ys + 265, 8))
		set_rot_knob(trace, 12, &trace->start_yangle, control.rotsy.i, control.rotsy.j);
	else if (in_circle(x, y, control.rotsz.i + 100, -control.rotsz.j + control.dials_ys + 410, 8))
		set_rot_knob(trace, 13, &trace->start_zangle, control.rotsz.i, control.rotsz.j);
	return (0);
}

static inline void	set_scale_knob(t_trace *trace, int knob)
{
	trace->dragging = true;
	trace->knob = knob;
}

int	scale_press(int x, int y, t_trace *trace, t_control control)
{
	int scale_ceny = 380;

	if (in_circle(x, y, control.sca1x.i + 200, -control.sca1x.j + scale_ceny, 8))
		set_scale_knob(trace, 24);
	else if (in_circle(x, y, control.sca1y.i + 200, -control.sca1y.j + scale_ceny, 8))
		set_scale_knob(trace, 25);
	else if (in_circle(x, y, control.sca1z.i + 200, -control.sca1z.j + scale_ceny, 8))
		set_scale_knob(trace, 26);
	else if (in_circle(x, y, control.sca2x.i + 200, -control.sca2x.j + scale_ceny, 8))
		set_scale_knob(trace, 27);
	else if (in_circle(x, y, control.sca2y.i + 200, -control.sca2y.j + scale_ceny, 8))
		set_scale_knob(trace, 28);
	else if (in_circle(x, y, control.sca2z.i + 200, -control.sca2z.j + scale_ceny, 8))
		set_scale_knob(trace, 29);
	else if (in_circle(x, y, control.scale_xyz.i + 200, -control.scale_xyz.j + scale_ceny, 8))
		set_scale_knob(trace, 30);
	else if (trace->on->type == HYPERBOLOID)
	{
		if (in_circle(x, y, 149 + (((t_hyperboloid *)(trace->curr_hy))->waist_val + 1) * 50, 534, 8))
		{
			trace->start_x = x;
			set_scale_knob(trace, 31);
		}
	}
	return (0);
}

static inline void	set_pos_knob(t_trace *trace, int knob, double *start, double angle)
{
	trace->dragging = true;
	*start = fmod(angle, 10) * PI_FIFTHS;
	trace->knob = knob;
}

int	position_press(int x, int y, t_trace *trace, t_control control)
{
	if (in_circle(x, y, control.posx.i + 300, -control.posx.j + control.dials_ys + 120, 8))
		set_pos_knob(trace, 14, &trace->start_xangle, control.poss.x );
	else if (in_circle(x, y, control.posy.i + 300, -control.posy.j + control.dials_ys + 265, 8))
		set_pos_knob(trace, 15, &trace->start_yangle, control.poss.y );
	else if (in_circle(x, y, control.posz.i + 300, -control.posz.j + control.dials_ys + 410, 8))
		set_pos_knob(trace, 16, &trace->start_zangle, control.poss.z );
	return (0);
}
