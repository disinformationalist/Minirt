#include "minirt.h"

//reset withou rerender

void	reset_con_non(t_trace *trace)
{
	t_control	con;

	con = *trace->obj_control;
	if (trace->menu_open)
	{
		trace->menu_open = false;
		trace->on_menu = true;
		controls(trace);
		if (con.sca_open)
			set_sca_dials(trace);
		else
		{
			if (con.pos_open)
				set_pos_dials(trace);
			if (con.rot_open)
				set_rot_dials(trace);
		}
		set_con_vals(trace->mlx_connect, trace->mlx_win, trace);
	}
}

void	reset_con(t_trace *trace)
{
	t_control	con;

	con = *trace->obj_control;
	render(trace);
	if (trace->menu_open)
	{
		trace->menu_open = false;
		trace->on_menu = true;
		controls(trace);
		if (con.sca_open)
			set_sca_dials(trace);
		else
		{
			if (con.pos_open)
				set_pos_dials(trace);
			if (con.rot_open)
				set_rot_dials(trace);
		}
		set_con_vals(trace->mlx_connect, trace->mlx_win, trace);
	}
}

//reset all pos tracks

void	reset_postracks(t_img *img, t_control control)
{
	int				j;
	int				i;
	int				k;
	unsigned int	color;
	int				y_lim;

	j = 69;
	k = 0;
	y_lim = control.dt_height + control.dials_ys;
	while (++j < y_lim)
	{
		i = -1;
		while (++i < control.dt_width)
		{
			color = pixel_color_get3(i, k, control.dtpx);
			my_pixel_put(230 + i, 50 + j, img, color);
			color = pixel_color_get3(i, k, control.dtpy);
			my_pixel_put(230 + i, 195 + j, img, color);
			color = pixel_color_get3(i, k, control.dtpz);
			my_pixel_put(230 + i, 340 + j, img, color);
		}
		k++;
	}
}

//reset all rot tracks

void	reset_rottracks(t_img *img, t_control control)
{
	int				j;
	int				i;
	int				k;
	unsigned int	color;
	int				y_lim;

	j = 69;
	k = 0;
	y_lim = 211;
	while (++j < y_lim)
	{
		i = -1;
		while (++i < 141)
		{
			color = pixel_color_get3(i, k, control.dtx);
			my_pixel_put(30 + i, 50 + j, img, color);
			color = pixel_color_get3(i, k, control.dty);
			my_pixel_put(30 + i, 195 + j, img, color);
			color = pixel_color_get3(i, k, control.dtz);
			my_pixel_put(30 + i, 340 + j, img, color);
		}
		k++;
	}
}

void	update_pos(t_trace *trace)
{
	t_control	cont;
	t_img		*img;
	void		*con;
	void		*win;

	con = trace->mlx_connect;
	win = trace->mlx_win;
	cont = *trace->obj_control;
	set_pos(trace);
	if (cont.pos_open)
	{
		img = &trace->img;
		reset_postracks(img, cont);
		set_posknobs(trace, *trace->obj_control);
		//set_con_vals(con, win, trace);
	}
	if (!trace->stash)
	{
		trace->low_res = true;
		render(trace);
	}
	else
		mlx_put_image_to_window(con, win, trace->img.img_ptr, 0, 0);
}
