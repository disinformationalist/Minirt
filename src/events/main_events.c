#include "minirt.h"

void	print_stat(bool state, char *stat)
{
	ft_putstr_fd(stat, 1);
	if (state)
		ft_putstr_fd(GREEN": true\n"RESET, 1);
	else
		ft_putstr_fd(RED": false\n"RESET, 1);
}

void	print_menu_status(t_trace *trace)
{
	t_control con = *trace->obj_control;

	ft_putstr_fd(BLUE"---------Current menu stats-----------\n\n"RESET, 1);
	
	printf("Supersample level: "GREEN"%.0f"RESET" (%0.fsamples per pixel)\n", trace->n, trace->n * trace->n);
	printf("Lowres level     : "GREEN"%d"RESET" (%dtimes lower resolution)\n\n", trace->m_lowinc, trace->m_lowinc * trace->m_lowinc);
	print_stat(trace->supersample, "super on  ");
	printf("\n");

	print_stat(trace->menu_open, "menu open ");
	print_stat(trace->on_menu, "on menu   ");
	print_stat(trace->low_res, "low res   ");
	print_stat(trace->low_flag, "low flag  ");
	print_stat(trace->dragging, "dragging  ");
	printf("\n");
	print_stat(con.on_dials, "on dials  ");
	print_stat(con.rot_open, "rot open  ");
	print_stat(con.pos_open, "pos open  ");
	print_stat(con.sca_open, "sca open  ");
	ft_putstr_fd(YELLOW"(currently deprecated)"RESET, 1);
	print_stat(con.lt_con_open, "ltcon open");
	printf("\n");
}

void		update_button(t_trace *trace, void *con, void *win)
{
	trace->obj_control->pos_open = false;
	trace->obj_control->on_dials = false;
	trace->obj_control->rot_open = false;
	trace->obj_control->sca_open = false;
	trace->on_menu = true;
	update_no_low(con, win, trace);	
	set_controls(trace);
	set_menu_vals(trace, trace->on);
}

//transfigure() changes material of current object

int	transfigure(int keycode, t_trace *trace)
{
	if (keycode == PAD_1)
		change_mat(trace, trace->on, get_mat(DEFAULT));
	else if (keycode == PAD_2)
		change_mat(trace, trace->on, get_mat(METAL));
	else if (keycode == PAD_3)
		change_mat(trace, trace->on, get_mat(MATTE));
	else if (keycode == PAD_4)
		change_mat(trace, trace->on, get_mat(GLASS));
	else if (keycode == PAD_5)
		change_mat(trace, trace->on, get_mat(MIRROR));
	else if (keycode == PAD_6)
		change_mat(trace, trace->on, get_mat(DIAMOND));
	else if (keycode == PAD_7)
		change_mat(trace, trace->on, get_mat(WATER));
	else if (keycode == PAD_8)
		change_mat(trace, trace->on, get_mat(CHROME));
	else if (keycode == PAD_9)
		change_mat(trace, trace->on, get_mat(AIR));
	else
		return (0);
	if (trace->menu_open)
		reset_con(trace);
	else
		render(trace);
	return (1);
}

int	key_press_3(int keycode, t_trace *trace)
{
	if (keycode == A)
		rotate_object(trace, trace->on, rot_x(-PI_12THS), vec(PI_12THS, 0, 0, 0), 1);
	else if (keycode == D)
		rotate_object(trace, trace->on, rot_x(PI_12THS), vec(PI_12THS, 0, 0, 0), 1);
	else if (keycode == W)
		rotate_object(trace, trace->on, rot_y(PI_12THS), vec(0, PI_12THS, 0, 0), 1);
	else if (keycode == S)
		rotate_object(trace, trace->on, rot_y(-PI_12THS), vec(0, PI_12THS, 0, 0), 1);
	else if (keycode == Q)
		rotate_object(trace, trace->on, rot_z(PI_12THS), vec(0, 0, PI_12THS, 0), 1);
	else if (keycode == E)
		rotate_object(trace, trace->on, rot_z(-PI_12THS), vec(0, 0, PI_12THS, 0), 1);
	else if (keycode == C)
		toggle_caps(trace);
	else if (keycode == DIV)
		toggle_sine(trace);
	else if (keycode == MULT)
		frost_on(trace, *(trace->on));
	else if (keycode == SHIFT)
		trace->shift_on = true;
	else
		return (transfigure(keycode, trace));
	return (0);
}

//second layer of key press 2 controls, scale

int	key_press_2layer(int keycode, t_trace *trace)
{
	if (keycode == J)
		scale_object(trace, trace->on, vec(.9, 1, 1, 0), keycode);
	else if (keycode == L)
		scale_object(trace, trace->on, vec(1.1, 1, 1, 0), keycode);
	else if (keycode == I)
		scale_object(trace, trace->on, vec(1, 1.1, 1, 0), keycode);
	else if (keycode == K)
		scale_object(trace, trace->on, vec(1, .9, 1, 0), keycode);
	else if (keycode == U)
		scale_object(trace, trace->on, vec(1, 1, .9, 0), keycode);
	else if (keycode == O)
		scale_object(trace, trace->on, vec(1, 1, 1.1, 0), keycode);
	else if (keycode == M)
		scale_object(trace, trace->on, vec(1.1, 1.1, 1.1, 0), keycode);
	else if (keycode == N)
		scale_object(trace, trace->on, vec(.9, .9, .9, 0), keycode);
	else
		key_press_3(keycode, trace);
	if (trace->menu_open)
	{
		void *con = trace->mlx_connect;
		void *win = trace->mlx_win;
		update_no_low(con, win, trace);
		set_con_vals(con, win, trace);
	}
	else
		render(trace);
	return (0);
}

unsigned int pixel_color_get3(int x, int y, t_img *img)
{
	unsigned int	pixel_color;
	int 			offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	pixel_color = *(unsigned int *)(img->pixels_ptr + offset);
	return (pixel_color);
}

void	set_knobs(t_img *img, t_control control, int k_width, t_on *on)
{
	int				i;
	int				j;
	unsigned int	color;
	int				x_startr;
	int				x_startg;
	int				x_startb;
	int				y_startr;
	int				y_startg;
	int				y_startb;
	int				start;
	t_norm_color	obj_col;

	obj_col = get_obj_color2(on);
	start = 22 - k_width / 2;

	x_startr = start + (int)obj_col.r;
	x_startg = start + (int)obj_col.g;
	x_startb = start + (int)obj_col.b;
	y_startr = start + 89;
	y_startg = y_startr + 25;
	y_startb = y_startg + 25;

	j = -1;
	while (++j < k_width)
	{
		i = -1;
		while (++i < k_width)
		{
			color = pixel_color_get3(i, j , control.r);
			if (color != 0x202020)
				my_pixel_put(x_startr + i, y_startr + j, img, color);
			color = pixel_color_get3(i, j , control.g);
			if (color != 0x202020)
				my_pixel_put(x_startg + i, y_startg + j, img, color);
			color = pixel_color_get3(i, j , control.b);
			if (color != 0x202020)
				my_pixel_put(x_startb + i, y_startb + j, img, color);
		}
	}
}

void	set_controls(t_trace *trace)
{
	t_control	control;
	int			i;
	int			j;
	t_img		*img;
	t_on		*on;
	t_img		*con;

	on = trace->on;
	control = *trace->obj_control;
	if (on->type == LIGHT)
		con = control.lt_con;
	else
		con = control.menu;
	img = &trace->img;
	{
		j = -1;
		while(++j < control.m_height)
		{
			i = -1;
			while (++i < control.m_width)
				my_pixel_put(i, j, &trace->img, pixel_color_get3(i, j, con));
		}
		if (on->object && on->type != CAM)
		{
			set_knobs(img, control, control.k_width, on);
			if (on->type != LIGHT)
			{
				set_pknobs1(img, control, on);
				set_bumpknob(img, control, on);
			}
			else if (on->type == LIGHT)
				set_ltknobs(img, control, on);
		}
		else if (on->type == CAM)
		{
			img_copyto(&trace->img, control.fov, 10, 70, control.f_width, control.f_height);
			set_fovknob(&trace->img, control, on);
		}
	}
}

//if open putstrs after put to win

void	controls(t_trace *trace)
{
	if (!trace->menu_open)
		set_controls(trace);
	trace->menu_open = !trace->menu_open;
	if (!trace->menu_open)
	{
		trace->obj_control->rot_open = false;
		trace->obj_control->pos_open = false;
		trace->obj_control->sca_open = false;
		trace->knob = -1;
		render(trace);
	}
	else
	{
		mlx_put_image_to_window(trace->mlx_connect,
		trace->mlx_win, trace->img.img_ptr, 0, 0);
		if (!trace->obj_control->pos_open && !trace->obj_control->sca_open)
			set_menu_vals(trace, trace->on);
	}
}

void	adjust_lowres(int keycode, t_trace *trace)
{
	if (keycode == XK_Left)
	{
		trace->m_lowinc -= 1;
		if (trace->m_lowinc < 3)
			trace->m_lowinc = 3;
	}
	else if (keycode == XK_Right)
	{
		trace->m_lowinc += 1;
		if (trace->m_lowinc > 50)
			trace->m_lowinc = 50;
	}
}

void	set_con_vals(void *con, void *win, t_trace *trace)
{
	if (trace->obj_control->rot_open)
		set_rot_vals(con, win, trace);
	if (!trace->obj_control->pos_open && !trace->obj_control->sca_open)
		set_menu_vals(trace, trace->on);
	else if (trace->obj_control->sca_open)
	{
		set_sca_vals(con, win, trace);
		set_type(con, win, trace->on->type);
	}
	else
	{
		set_pos_vals(con, win, trace);
		set_type(con, win, trace->on->type);
	}
}

//translation, push, and pop functions

int	key_press_2(int keycode, t_trace *trace)
{
	void *con = trace->mlx_connect;
	void *win = trace->mlx_win;
	
	if (keycode == J)
		translate_object(trace, trace->on, vec(-.5, 0, 0, 0), 1);
	else if (keycode == L)
		translate_object(trace, trace->on, vec(.5, 0, 0, 0), 1);
	else if (keycode == I)
		translate_object(trace, trace->on, vec(0, .5, 0, 0), 1);
	else if (keycode == K)
		translate_object(trace, trace->on, vec(0, -.5, 0, 0), 1);
	else if (keycode == U)
		translate_object(trace, trace->on, vec(0, 0, .5, 0), 1);
	else if (keycode == O)
		translate_object(trace, trace->on, vec(0, 0, -.5, 0), 1);
	else if (keycode == M)
	{
		push_new_object(trace, trace->on);
		if (trace->menu_open)
		{
			update_button(trace, con, win);
			return (0);
		}
	}
	else if (keycode == N)
	{
		pop_object(trace, trace->on);
		if (trace->menu_open)
		{
			update_button(trace, con, win);
			return (0);
		}
	}
	else if (keycode == SPACE)
	{
		supersample_handle(SPACE, trace);
		return (0);
	}
	else if (keycode == XK_Control_R)
		toggle_shadow(trace, trace->on);
	else if (keycode == XK_Control_L)
		toggle_bump(trace, trace->on);
	else if (keycode == XK_Left || keycode == XK_Right)
	{
		adjust_lowres(keycode, trace);
		if (!trace->low_res)
			return (0);
	}
	else if (key_press_3(keycode, trace))
		return (0);
	if (trace->menu_open)
	{
		t_control cont = *trace->obj_control;
		if (cont.pos_open)
		{
			
			set_pos(trace);
			reset_postracks(&trace->img, cont);
			set_posknobs(trace, *trace->obj_control);
		}
		if (cont.rot_open)
		{
			set_rotpos(trace);
			reset_rottracks(&trace->img, cont);
			set_rotknobs(trace, *trace->obj_control);
		
		}
		update_no_low(con, win, trace);
		set_con_vals(con, win, trace);
		return (0);
	}
	else
		render(trace);
	return (0);
}

//main function for keypress events

int	key_press(int keycode, t_trace *trace)
{
	//printf("%d\n", keycode);
	if (keycode == XK_Escape)
		close_win(trace);
	else if (keycode == B)
		print_menu_status(trace);
	else if (keycode == XK_Tab)
		trace->layer = !trace->layer;
	else if (keycode == XK_1 || keycode == XK_2 || keycode == XK_3 || \
		keycode == XK_4 || keycode == XK_9 || keycode == XK_0 || \
		keycode == XK_5)
		switch_list(keycode, trace, trace->on);
	else if (keycode == XK_Up)
		adjust_super(keycode, trace);
	else if (keycode == XK_Down)
		adjust_super(keycode, trace);
	else if (keycode == PAD_PLUS)
		next_list_ob(trace, trace->on);
	else if (keycode == PAD_MINUS)
		prev_list_ob(trace, trace->on);
	else if ((keycode == F1) | (keycode == F3))
		forge_or_export(keycode, trace);
	else if (keycode == F4)
		controls(trace);
	else if (keycode == F2)
		print_guide();
	else if (keycode == F5)
		print_gui_guide();
	else if (keycode == X)
		trace->stash = !trace->stash;
	else if (keycode == PAD_0)
		toggle_lowres(trace);
	else if (trace->layer)
		key_press_2layer(keycode, trace);
	else
		key_press_2(keycode, trace);
	return (0);
}
