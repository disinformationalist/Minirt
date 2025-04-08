#include "minirt.h"

//super on/off

int	supersample_handle(int keycode, t_trace *trace)
{
	(void)keycode;
	trace->supersample = !trace->supersample;
	trace->low_res = false;
	trace->low_flag = false;
	reinit_viewing(trace);
	render(trace);
	if (trace->menu_open)
		set_con_vals(trace->mlx_connect, trace->mlx_win, trace);
	return (0);
}

//increase/decrease supersample level

void	adjust_super(int keycode, t_trace *trace)
{
	if (keycode == XK_Up)
	{
		trace->n += 1.0;
		if (trace->n > 10.0)
			trace->n = 13.0;
	}
	if (keycode == XK_Down)
	{
		trace->n -= 1.0;
		if (trace->n < 2.0)
			trace->n = 2.0;
	}
	reinit_viewing(trace);
}
