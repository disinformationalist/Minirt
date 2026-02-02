#include "minirt.h"

//check if a point is with a circle, used to check mouse clicks

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

static inline void	set_dial_knob(t_trace *r, int knob)
{
	r->dragging = true;
	r->knob = knob;
}

//check if mouse click on rotation knobs
//rotation knobs 11, 12, 13 for x, y, z //rotx, 10 = bump knob

int	rotation_press(int x, int y, t_trace *trace, t_control con)
{
	if (in_circle(x, y, con.knobs.rotx.posx + 9, con.knobs.rotx.posy + 9, 8))
		set_dial_knob(trace, 11);
	else if (in_circle(x, y, con.knobs.roty.posx + 9, con.knobs.roty.posy + 9, 8))
		set_dial_knob(trace, 12);
	else if (in_circle(x, y, con.knobs.rotz.posx + 9, con.knobs.rotz.posy + 9, 8))
		set_dial_knob(trace, 13);
	return (0);
}

//check if mouse click on scaling knobs

int	scale_press(int x, int y, t_trace *trace, t_control con)
{
/* 	int scale_ceny = 380;

	if (in_circle(x, y, control.sca1x.i + 200, -control.sca1x.j + scale_ceny, 8))
		set_dial_knob(trace, 24);
	else if (in_circle(x, y, control.sca1y.i + 200, -control.sca1y.j + scale_ceny, 8))
		set_dial_knob(trace, 25);
	else if (in_circle(x, y, control.sca1z.i + 200, -control.sca1z.j + scale_ceny, 8))
		set_dial_knob(trace, 26);
	else if (in_circle(x, y, control.sca2x.i + 200, -control.sca2x.j + scale_ceny, 8))
		set_dial_knob(trace, 27);
	else if (in_circle(x, y, control.sca2y.i + 200, -control.sca2y.j + scale_ceny, 8))
		set_dial_knob(trace, 28);
	else if (in_circle(x, y, control.sca2z.i + 200, -control.sca2z.j + scale_ceny, 8))
		set_dial_knob(trace, 29);
	else if (in_circle(x, y, control.scale_xyz.i + 200, -control.scale_xyz.j + scale_ceny, 8))
		set_dial_knob(trace, 30);
	else if (trace->on->type == HYPERBOLOID)
	{
		if (in_circle(x, y, 149 + (((t_hyperboloid *)(trace->curr_hy))->waist_val + 1) * 50, 534, 8))
		{
			trace->start_x = x;
			set_dial_knob(trace, 31);
		}
	} */

	if (in_circle(x, y, con.knobs.scalex.posx + 9, con.knobs.scalex.posy + 9, 8))
		set_dial_knob(trace, 24);
	else if (in_circle(x, y, con.knobs.scaley.posx + 9, con.knobs.scaley.posy + 9, 8))
		set_dial_knob(trace, 25);
	else if (in_circle(x, y, con.knobs.scalez.posx + 9, con.knobs.scalez.posy + 9, 8))
		set_dial_knob(trace, 26);
		else if (in_circle(x, y, con.knobs.scalexz.posx + 9, con.knobs.scalexz.posy + 9, 8))
		set_dial_knob(trace, 27);
		else if (in_circle(x, y, con.knobs.scaleyz.posx + 9, con.knobs.scaleyz.posy + 9, 8))
		set_dial_knob(trace, 28);
		else if (in_circle(x, y, con.knobs.scalexy.posx + 9, con.knobs.scalexy.posy + 9, 8))
		set_dial_knob(trace, 29);
	else if (in_circle(x, y, con.knobs.scalexyz.posx + 9, con.knobs.scalexyz.posy + 9, 8))
		set_dial_knob(trace, 30);
	else if (trace->on->type == HYPERBOLOID)
	{
		if (in_circle(x, y, 149 + (((t_hyperboloid *)(trace->curr_hy))->waist_val + 1) * 50, 534, 8))
		{
			trace->start_x = x;
			set_dial_knob(trace, 31);
		}
	}
	return (0);
}

//check if mouse click on position knobs

int	position_press(int x, int y, t_trace *trace, t_control con)
{
	if (in_circle(x, y, con.knobs.posx.posx + 9, con.knobs.posx.posy + 9, 8))
		set_dial_knob(trace, 14);
	else if (in_circle(x, y, con.knobs.posy.posx + 9, con.knobs.posy.posy + 9, 8))
		set_dial_knob(trace, 15);
	else if (in_circle(x, y, con.knobs.posz.posx + 9, con.knobs.posz.posy + 9, 8))
		set_dial_knob(trace, 16);
	return (0);
}
