#include "minirt.h"

char *get_obj_type(t_type type)
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

void	set_color_chan(t_on *on, int chan, double new_col)
{
	t_light	*lt;


	if (on->type == LIGHT)
	{
		lt = (t_light *)on->object;
		if (chan == 0) lt->color.r= new_col / 255;
		else if (chan == 1)lt->color.g= new_col / 255;
		else if (chan == 2)lt->color.b= new_col / 255;

		if (lt->type == AREA)
		{
			if (chan == 0) lt->emitter->color.r = new_col;
			else if (chan == 1) lt->emitter->color.g = new_col;
			else if (chan == 2) lt->emitter->color.b = new_col;
		}
	}
	else if (on->type == SPHERE)
	{
		t_sphere *sp = (t_sphere *)on->object;
		if (chan == 0)sp->color.r = new_col;
		else if (chan == 1)sp->color.g = new_col;
		else if (chan == 2)sp->color.b = new_col;
	}
	else if (on->type == PLANE)
	{
		t_plane *pl = (t_plane *)on->object;
		if (chan == 0) pl->color.r = new_col;
		else if (chan == 1) pl->color.g = new_col;
		else if (chan == 2) pl->color.b = new_col;
	}
	else if (on->type == CYLINDER)
	{
		t_cylinder *cy = (t_cylinder *)on->object;
		if (chan == 0) cy->color.r = new_col;
		else if (chan == 1) cy->color.g = new_col;
		else if (chan == 2) cy->color.b = new_col;
	}
	else if (on->type == CUBE)
	{
		t_cube *cu = (t_cube *)on->object;
		if (chan == 0) cu->color.r = new_col;
		else if (chan == 1) cu->color.g = new_col;
		else if (chan == 2) cu->color.b = new_col;
	}
	else if (on->type == HYPERBOLOID)
	{
		t_hyperboloid *hy = (t_hyperboloid *)on->object;
		if (chan == 0) hy->color.r = new_col;
		else if (chan == 1) hy->color.g = new_col;
		else if (chan == 2) hy->color.b = new_col;
	}
	else
		return ;
}

void	set_prop(t_on *on, t_type type, int knob, double new_val)
{
	if (type == SPHERE)
	{
		t_sphere *sp = (t_sphere *)on->object;
		if (knob == 3)sp->mat.amb = new_val;
		else if (knob == 4)sp->mat.diff = new_val;
		else if (knob == 5)sp->mat.spec = new_val;
		else if (knob == 6)sp->mat.shine = new_val;
		else if (knob == 7)sp->mat.ref = new_val;
		else if (knob == 8)sp->mat.transp = new_val;
		else if (knob == 9)sp->mat.refract = new_val;
		sp->mat.preset = -1;
	}
	else if (type == PLANE)
	{
		t_plane *pl = (t_plane *)on->object;
		if (knob == 3)pl->mat.amb = new_val;
		else if (knob == 4)pl->mat.diff = new_val;
		else if (knob == 5)pl->mat.spec = new_val;
		else if (knob == 6)pl->mat.shine = new_val;
		else if (knob == 7)pl->mat.ref = new_val;
		else if (knob == 8)pl->mat.transp = new_val;
		else if (knob == 9)pl->mat.refract = new_val;
		pl->mat.preset = -1;
	}
	else if (type == CYLINDER)
	{
		t_cylinder *cy = (t_cylinder *)on->object;
		if (knob == 3)cy->mat.amb = new_val;
		else if (knob == 4)cy->mat.diff = new_val;
		else if (knob == 5)cy->mat.spec = new_val;
		else if (knob == 6)cy->mat.shine = new_val;
		else if (knob == 7)cy->mat.ref = new_val;
		else if (knob == 8)cy->mat.transp = new_val;
		else if (knob == 9)cy->mat.refract = new_val;
		cy->mat.preset = -1;
	}
	else if (type == HYPERBOLOID)
	{
		t_hyperboloid *hy = (t_hyperboloid *)on->object;
		if (knob == 3)hy->mat.amb = new_val;
		else if (knob == 4)hy->mat.diff = new_val;
		else if (knob == 5)hy->mat.spec = new_val;
		else if (knob == 6)hy->mat.shine = new_val;
		else if (knob == 7)hy->mat.ref = new_val;
		else if (knob == 8)hy->mat.transp = new_val;
		else if (knob == 9)hy->mat.refract = new_val;
		hy->mat.preset = -1;
	}
	else if (type == CUBE)
	{
		t_cube *cu = (t_cube *)on->object;
		if (knob == 3)cu->mat.amb = new_val;
		else if (knob == 4)cu->mat.diff = new_val;
		else if (knob == 5)cu->mat.spec = new_val;
		else if (knob == 6)cu->mat.shine = new_val;
		else if (knob == 7)cu->mat.ref = new_val;
		else if (knob == 8)cu->mat.transp = new_val;
		else if (knob == 9)cu->mat.refract = new_val;
		cu->mat.preset = -1;
	}
}

void	reset_track(t_img *img, t_control control, int move_y)
{
	int				j;
	int				i;
	unsigned int	color;

	j = -1;
	while (++j < control.ct_height)
	{
		i = -1;
		while (++i < control.ct_width)
		{
			color = pixel_color_get3(i, j , control.ct);
				my_pixel_put(9 + i, move_y + j, img, color);
		}
	}
}

void	set_rknob(t_img *img, t_control control, t_on *on)
{
	int				i;
	int				j;
	unsigned int	color;
	int				x_startr;
	int				y_startr;
	t_norm_color	obj_col;

	obj_col = get_obj_color2(on);
	x_startr = 14 + (int)obj_col.r;
	y_startr = 103;
	j = -1;
	while (++j < 17)
	{
		i = -1;
		while (++i < 17)
		{
			color = pixel_color_get3(i, j , control.r);
			if (color != 0xFF202020)
				my_pixel_put(x_startr + i, y_startr + j, img, color);
		}
	}
}

void	set_gknob(t_img *img, t_control control, t_on *on)
{
	int				i;
	int				j;
	unsigned int	color;
	int				x_startr;
	int				y_startr;
	t_norm_color	obj_col;

	obj_col = get_obj_color2(on);
	x_startr = 14 + (int)obj_col.g;
	y_startr = 128;
	j = -1;
	while (++j < 17)
	{
		i = -1;
		while (++i < 17)
		{
			color = pixel_color_get3(i, j , control.g);
			if (color != 0xFF202020)
				my_pixel_put(x_startr + i, y_startr + j, img, color);
		}
	}
}

void	set_bknob(t_img *img, t_control control, t_on *on)
{
	int				i;
	int				j;
	unsigned int	color;
	int				x_startr;
	int				y_startr;
	t_norm_color	obj_col;

	obj_col = get_obj_color2(on);
	x_startr = 14 + (int)obj_col.b;
	y_startr = 153;
	j = -1;
	while (++j < 17)
	{
		i = -1;
		while (++i < 17)
		{
			color = pixel_color_get3(i, j , control.b);
			if (color != 0xFF202020)
				my_pixel_put(x_startr + i, y_startr + j, img, color);
		}
	}
}

void	reset_ptrack(t_img *img, t_control control, int move_x, int move_y)
{
	int				j;
	int				i;
	unsigned int	color;

	j = -1;
	while (++j < control.pt_height)
	{
		i = -1;
		while (++i < control.pt_width)
		{
			color = pixel_color_get3(i, j , control.pt);
				my_pixel_put(move_x + i, move_y + j, img, color);
		}
	}
}

void reset_scatrack(t_img *img, t_control control, int move_y)
{
	int				j;
	int				i;
	unsigned int	color;

	j = -1;
	while (++j < control.st_height)
	{
		i = -1;
		while (++i < control.st_width)
		{
			color = pixel_color_get3(i, j , control.sca_tracks);
				my_pixel_put(69 + i, move_y + j, img, color);
		}
	}
}

void	set_bumpknob(t_img *img, t_control control, t_on *on)
{
	int				i;
	int				j;
	unsigned int	color;
	double			level;

	level = get_bump_level(on);
	int x_start = 165 + level;
	int y_start = 453;
	j = -1;
	while (++j < 17)
	{
		i = -1;
		while (++i < 17)
		{
			color = pixel_color_get3(i, j , control.bron);
			if (color != 0xFF202020)
				my_pixel_put(x_start + i, y_start + j, img, color);
		}
	}
}

void	set_areaknobs(t_img *img, t_control control, t_on *on)
{
	int				i;
	int				j;
	unsigned int	color;
	t_light			*lt;

	lt = (t_light *)on->object;
	double wid = 1 / (lt->emitter->curr_scale.mat[0] * lt->emitter->h_width);
	double len = 1 / (lt->emitter->curr_scale.mat[10] * lt->emitter->h_depth);
	int x_starta = 165 + wid * 10;
	int x_startb = 165 + len * 10;
	int x_startc = 165 + lt->usteps * 4;
	int x_startd = 165 + lt->vsteps * 4;
	int y_starta = 379;
	int y_startb = 404;
	int y_startc = 429;
	int y_startd = 454;
	j = -1;
	while (++j < 17)
	{
		i = -1;
		while (++i < 17)
		{
			color = pixel_color_get3(i, j , control.sil);
			if (color != 0xFF202020)
			{
				my_pixel_put(x_starta + i, y_starta + j, img, color);
				my_pixel_put(x_startb + i, y_startb + j, img, color);
				my_pixel_put(x_startc + i, y_startc + j, img, color);
				my_pixel_put(x_startd + i, y_startd + j, img, color);
			}
		}
	}
}

void	set_spotknobs(t_img *img, t_control control, t_on *on)
{
	int				i;
	int				j;
	unsigned int	color;
	t_light			*lt;

	lt = (t_light *)on->object;
	int x_starta = 165 + ft_round(lt->in_angle * 10 / 9);
	int x_startb = 165 + ft_round(lt->out_angle * 10 / 9);
	int y_starta = 299;
	int y_startb = 324;
	j = -1;
	while (++j < 17)
	{
		i = -1;
		while (++i < 17)
		{
			color = pixel_color_get3(i, j , control.sil);
			if (color != 0xFF202020)
			{
				my_pixel_put(x_starta + i, y_starta + j, img, color);
				my_pixel_put(x_startb + i, y_startb + j, img, color);
			}
		}
	}
}

void	set_brightknob(t_img *img, t_control control, double bright)
{
	int				i;
	int				j;
	unsigned int	color;
	
	int x_starta = 165 + ft_round(bright * 100);
	int y_starta = 183;
	j = -1;
	while (++j < 17)
	{
		i = -1;
		while (++i < 17)
		{
			color = pixel_color_get3(i, j , control.sil);
			if (color != 0xFF202020)
				my_pixel_put(x_starta + i, y_starta + j, img, color);
		}
	}
}

void	set_ltknobs(t_img *img, t_control control, t_on *on)
{
	t_light			*lt;

	lt = (t_light *)on->object;
	set_brightknob(img, control, lt->brightness);
	if (lt->type == SPOT)
		set_spotknobs(img, control, on);
	else if (lt->type == AREA)
		set_areaknobs(img, control, on);
}

void	set_pknobs1(t_img *img, t_control control, t_on *on)
{
	int				i;
	int				j;
	unsigned int	color;
	t_mat			mat;

	mat	= get_obj_mat(on);
	int x_starta = 165 + mat.amb * 100;//+ property increment
	int x_startd = 165 + mat.diff * 100;
	int x_starts = 165 + mat.spec * 100;
	int x_startsh;
	if (mat.shine > 20)
		x_startsh = 165 + mat.shine / 10;
	else
		x_startsh = 165 + mat.shine;

	int x_startr = 165 + mat.ref * 100;
	int x_startt = 165 + mat.transp * 100;
	int x_startrt = 165 + (mat.refract - 1) * 33;
	int y_starta = 213;
	int y_startd = 238;
	int y_starts = 263;
	int y_startsh = 288;
	int y_startr = 313;
	int y_startt = 338;
	int y_startrt = 363;

	j = -1;
	while (++j < 17)
	{
		i = -1;
		while (++i < 17)
		{
			color = pixel_color_get3(i, j , control.sil);
			if (color != 0xFF202020)
			{
				my_pixel_put(x_starta + i, y_starta + j, img, color);
				my_pixel_put(x_startd + i, y_startd + j, img, color);
				my_pixel_put(x_starts + i, y_starts + j, img, color);
				my_pixel_put(x_startsh + i, y_startsh + j, img, color);
				my_pixel_put(x_startr + i, y_startr + j, img, color);
				my_pixel_put(x_startt + i, y_startt + j, img, color);
				my_pixel_put(x_startrt + i, y_startrt + j, img, color);
			}
		}
	}
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
	t_ray			r;
	t_vec3			current_pixel;
	t_intersects 	*intersects;

	current_pixel = add_vec(trace->pixel00, scale_vec(x, trace->pix_delta_rht));
	current_pixel = add_vec(current_pixel, scale_vec(y, trace->pix_delta_down));
	r.origin = trace->cam->center;
	r.dir = norm_vec(subtract_vec(current_pixel, r.origin));
	intersects = create_ints(trace->total_ints);
	if (!intersects)
		close_win(trace);
	find_closest(trace, r, intersects);
	set_mouse_on(trace, intersects->closest);
	if (trace->on->object)
	{
		trace->m_raydist = intersects->closest->t;
		trace->dragging = true;
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
	if (button == 1 && trace->dragging)
	{
		trace->dragging = false;
		trace->knob = -1;
		if (trace->low_flag)
			trace->low_inc = trace->m_lowinc;
		else
			trace->low_res = false;
		if (trace->menu_open && !trace->stash)
			reset_con(trace);
		else if (trace->menu_open)
			reset_con_non(trace);
		else if (!trace->stash)
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

void	reset_prtrack(t_img *img, t_img *track, int move_x, int move_y)
{
	int				j;
	int				i;
	unsigned int	color;

	move_y += 70;
	j = -1;
	while (++j < 141)
	{
		i = -1;
		while (++i < 141)
		{
			color = pixel_color_get3(i, j, track);
			my_pixel_put(move_x + i, move_y + j, img, color);
		}
	}
}

void	set_bump_level(t_on *on, double new_val)
{
	t_type	type;
	
	type = on->type;
	if (type == SPHERE)
	{
		t_sphere *sp = ((t_sphere *)on->object);
		sp->bump_level = new_val;
		sp->fuzz_lev = new_val / 1000;
	}
	else if (type == PLANE)
	{
		t_plane *pl= (t_plane *)on->object;
		pl->bump_level = new_val;
		pl->amp = new_val / 1000;
		pl->fuzz_lev = new_val / 1000;
	}
	else if (type == CYLINDER)
	{
		t_cylinder *cy = ((t_cylinder *)on->object);
		cy->bump_level = new_val;
		cy->fuzz_lev = new_val / 1000;
	}
	else if (type == CUBE)
	{
		t_cube *cu = (t_cube *)on->object;
		cu->bump_level = new_val;
		cu->amp = new_val / 1000;
		cu->fuzz_lev = new_val / 1000;
	}
	else if (type == HYPERBOLOID)
	{
		t_hyperboloid *hy = (t_hyperboloid *)on->object;
		hy->bump_level = new_val;
		hy->fuzz_lev = new_val / 1000;
	}
}

double	get_delta_translate(double y, double x, double *start)
{
	double	angle;
	double	delta_angle;

	angle = atan2(y, x);
	delta_angle = angle - *start;
	if (delta_angle > M_PI)
		delta_angle -= TWO_PI;
    else if (delta_angle < -M_PI)
		delta_angle += TWO_PI;
	*start = angle;
	return (delta_angle);
}

double set_newval_big(double angle, double max, double shift)
{
	double val;

	angle = fmin(max, angle) - shift;
	val = angle * 10.01 + 1;//10.01 =  9 * (1 / (M_PI / 3 - 0.14779));
	return (val);
}
	
double set_newval_small(double angle, double min, double shift)
{
	double val;

	angle = fmax(min, angle);
	angle = shift - angle;
	val = 1 - angle * 1.111;//1.111 = (1 / (M_PI / 3 - 0.14779));
	return (val);
}

double	handle_scale_knob(int x, int y, double center_angle, double min, double max)
{
	double angle;
	double new_val;

	angle = atan2((380 - y), x - 200);
	if (angle >= center_angle)
		new_val = set_newval_big(angle, max, center_angle);
	else
		new_val = set_newval_small(angle, min, center_angle);
	return (new_val);
}

void	set_waistknob(t_img *img, t_control control, t_on *on)
{
	int				i;
	int				j;
	unsigned int	color;
	double			val;

	val = ((t_hyperboloid *)(on->object))->waist_val;
	int x_start = 141 + (val + 1) * 50;
	int y_start = 525;
	j = -1;
	while (++j < 17)
	{
		i = -1;
		while (++i < 17)
		{
			color = pixel_color_get3(i, j , control.sil);
			if (color != 0xFF202020)
				my_pixel_put(x_start + i, y_start + j, img, color);
		}
	}
}

void	set_fovknob(t_img *img, t_control control, t_on *on)
{
	int				i;
	int				j;
	unsigned int	color;
	double			val;

	val = ((t_cam *)(on->object))->fov;
	int x_start = 102 + val;
	int y_start = 73;
	j = -1;
	while (++j < 17)
	{
		i = -1;
		while (++i < 17)
		{
			color = pixel_color_get3(i, j , control.sil);
			if (color != 0xFF202020)
				my_pixel_put(x_start + i, y_start + j, img, color);
		}
	}
}

// update the window with changes or just the menu, stash changes till close

void	update(void *con, void *win, t_trace *trace) 
{
	if (!trace->stash)
	{
		trace->low_res = true;
		render(trace);
	}
	else
		mlx_put_image_to_window(con, win, trace->img.img_ptr, 0, 0);
}

void	update_no_low(void *con, void *win, t_trace *trace) 
{
	if (!trace->stash)
		render(trace);
	else
		mlx_put_image_to_window(con, win, trace->img.img_ptr, 0, 0);
}

int mouse_move(int x, int y, t_trace *trace)
{
	double			delta_x;
	double			delta_y;
	t_vec3			move;
	t_norm_color	obj_col;
	int				current_knob_pos = 0;
	int 			new_knob_pos;
	double			new_val;
	int				knob;
	t_mat			mat;
	double			angle;
	double			delta_angle;
	void			*con = trace->mlx_connect;
	void			*win = trace->mlx_win;
	t_control		*control = trace->obj_control;
	t_control		cont = *trace->obj_control;

	knob = trace->knob;
	if (trace->dragging && control->rot_open && knob < 14)//rotation dials
	{
		if (knob == 11)
		{
			angle = atan2((190- y), x - 100);
			delta_angle = angle - trace->start_xangle;
			rotate_object(trace, trace->on, rot_x(delta_angle), vec(delta_angle, 0, 0, 0), 0);
			trace->start_xangle = angle;
		}
		else if (knob == 12)
		{
			angle = atan2((335 - y), x - 100);
			delta_angle = angle - trace->start_yangle;
			rotate_object(trace, trace->on, rot_y(delta_angle), vec(0, delta_angle, 0, 0), 0);
			trace->start_yangle = angle;
		}
		else if (knob == 13)
		{
			angle = atan2((480 - y), x - 100);
			delta_angle = angle - trace->start_zangle;
			//delta_angle = floor(delta_angle / DEG_TO_RAD) * DEG_TO_RAD;//magnetize to whole degrees, knob drag is bad with
			rotate_object(trace, trace->on, rot_z(delta_angle), vec(0, 0, delta_angle, 0), 0);
			trace->start_zangle = angle;
		}
	}
	else if (trace->dragging && cont.pos_open)//position dials
	{
		double	ratio = 1.59155;
		t_vec3	move;
		
		if (knob == 14)
		{
			delta_angle = get_delta_translate(190 - y, x - 300, &trace->start_xangle);
			move = vec(delta_angle * ratio, 0, 0, 0);
		}
		else if (knob == 15)
		{
			delta_angle = get_delta_translate(335 - y, x - 300, &trace->start_yangle);
			move = vec(0, delta_angle * ratio, 0, 0);
		}
		else if (knob == 16)
		{
			delta_angle = get_delta_translate(480 - y, x - 300, &trace->start_zangle);
			move = vec(0, 0, delta_angle * ratio, 0);
		}
		trace->low_res = true;
		translate_object(trace, trace->on, move, 0);
	}
	else if (trace->dragging && trace->menu_open && trace->on->type == LIGHT)
	{
		t_light *lt = (t_light *)(trace->on->object);
		delta_x = x - trace->start_x;

		if (knob >= 0 && knob < 3)
		{
			obj_col = get_obj_color2(trace->on);
			if (knob == 0) current_knob_pos = 16 + (int)(obj_col.r);
			else if (knob == 1) current_knob_pos = 16 + (int)(obj_col.g);
			else if (knob == 2) current_knob_pos = 16 + (int)(obj_col.b);
     		new_knob_pos= fmax(16, fmin(271, current_knob_pos + delta_x));
     		new_val = new_knob_pos - 16;
			set_color_chan(trace->on, trace->knob, new_val);	
			reset_track(&trace->img, *trace->obj_control, 103 + trace->knob * 25);
			if (knob == 0) set_rknob(&trace->img, *trace->obj_control, trace->on);
			else if (knob == 1) set_gknob(&trace->img, *trace->obj_control, trace->on);
			else if (knob == 2) set_bknob(&trace->img, *trace->obj_control, trace->on);
		}
		else if (knob == 17)
		{
			current_knob_pos = 167 + lt->brightness * 100;
			new_knob_pos = fmax(167, fmin(267, current_knob_pos + delta_x));
			new_val = (double)(new_knob_pos - 167) / 100;     		
			lt->brightness = new_val;
			reset_ptrack(&trace->img, *trace->obj_control, 162, 183);
			set_brightknob(&trace->img, *trace->obj_control, new_val);
		}
		else if (knob == 18)
		{
			current_knob_pos = 167 + ft_round(lt->in_angle * 10 / 9);
			new_knob_pos = fmax(167, fmin(267, current_knob_pos + delta_x));
			new_val = ft_round((double)(new_knob_pos - 167) * 9 / 10);     		
			lt->in_angle = new_val;
			lt->inner_cone = cos(new_val * DEG_TO_RAD);
			if (lt->inner_cone == lt->outer_cone)
				lt->inv_conediff = 0;
			else
				lt->inv_conediff = 1.0 / (lt->inner_cone - lt->outer_cone);
			reset_ptrack(&trace->img, *trace->obj_control, 162, 299);
			reset_ptrack(&trace->img, *trace->obj_control, 162, 324);
			set_spotknobs(&trace->img, *trace->obj_control, trace->on);
		}
		else if (knob == 19)
		{
			current_knob_pos = 167 + ft_round(lt->out_angle * 10 / 9);
			new_knob_pos = fmax(167, fmin(267, current_knob_pos + delta_x));
			new_val = ft_round((double)(new_knob_pos - 167) * 9 / 10);     		
			lt->out_angle = new_val;
			lt->outer_cone = cos(new_val * DEG_TO_RAD);
			if (lt->inner_cone == lt->outer_cone)
				lt->inv_conediff = 0;
			else
				lt->inv_conediff = 1.0 / (lt->inner_cone - lt->outer_cone);
			reset_ptrack(&trace->img, *trace->obj_control, 162, 299);
			reset_ptrack(&trace->img, *trace->obj_control, 162, 324);
			set_spotknobs(&trace->img, *trace->obj_control, trace->on);
		}
		else if (knob == 20)
		{
			double wid = 1 / (lt->emitter->curr_scale.mat[0] * lt->emitter->h_width);
			double len = 1 / (lt->emitter->curr_scale.mat[10] * lt->emitter->h_depth);
			current_knob_pos = 167 + ft_round(wid * 10);
			new_knob_pos = fmax(168, fmin(267, current_knob_pos + delta_x));
			set_lt_widlen(lt, ((double)(new_knob_pos - 167) / 10) * lt->emitter->h_width, len * lt->emitter->h_depth);		
			reset_ptrack(&trace->img, *trace->obj_control, 162, 379);
			set_areaknobs(&trace->img, *trace->obj_control, trace->on);
		}
		else if (knob == 21)
		{
			double wid = 1 / (lt->emitter->curr_scale.mat[0] * lt->emitter->h_width);
			double len = 1 / (lt->emitter->curr_scale.mat[10] * lt->emitter->h_depth);
			current_knob_pos = 167 + ft_round(len * 10);
			new_knob_pos = fmax(168, fmin(267, current_knob_pos + delta_x));
			set_lt_widlen(lt, wid * lt->emitter->h_width, ((double)(new_knob_pos - 167) / 10) * lt->emitter->h_depth);		
			reset_ptrack(&trace->img, *trace->obj_control, 162, 404);
			set_areaknobs(&trace->img, *trace->obj_control, trace->on);
		}
		else if (knob == 22)
		{
			current_knob_pos = 167 + lt->usteps * 4;
			new_knob_pos = fmax(171, fmin(267, current_knob_pos + delta_x * 1.25));
			lt->usteps = ft_round((double)(new_knob_pos - 167) / 4);
			lt->uvec = div_vec(lt->usteps, lt->v1);
			lt->samples = lt->usteps * lt->vsteps;
			reset_ptrack(&trace->img, *trace->obj_control, 162, 429);
			set_areaknobs(&trace->img, *trace->obj_control, trace->on);
		}
		else if (knob == 23)
		{
			current_knob_pos = 167 + lt->vsteps * 4;
			new_knob_pos = fmax(171, fmin(267, current_knob_pos + delta_x * 1.25));
			lt->vsteps = ft_round((double)(new_knob_pos - 167) / 4);
			lt->vvec = div_vec(lt->vsteps, lt->v2);
			lt->samples = lt->usteps * lt->vsteps;
			reset_ptrack(&trace->img, *trace->obj_control, 162, 454);
			set_areaknobs(&trace->img, *trace->obj_control, trace->on);
		}
		trace->start_x = x;
		update(con, win, trace);
		set_light_vals(trace, trace->on);
	}
	else if (trace->dragging && control->sca_open)
	{
		if (knob == 24)
		{
			new_val = handle_scale_knob(x, y, PI_SIXTHS, -0.3758, 1.423);
			gui_scale_object(trace, trace->on, vec(new_val / control->sca2.x, control->sca1.y, control->sca1.z, 0));
		}
		else if (knob == 25)
		{
			angle = atan2((380 - y), (x - 200));
			if (angle < 0)
				angle += TWO_PI;
			if (angle >= FIVE_PI_SIXTHS)
				new_val = set_newval_big(angle, 3.5174, FIVE_PI_SIXTHS);
			else
				new_val = set_newval_small(angle, 1.7186, FIVE_PI_SIXTHS);
			gui_scale_object(trace, trace->on, vec(control->sca1.x, new_val / control->sca2.y, control->sca1.z, 0));
		}
		else if (knob == 26)
		{
			new_val = handle_scale_knob(x, y, -PI_HALVES,  -2.4702, -0.6714);
			gui_scale_object(trace, trace->on, vec(control->sca1.x, control->sca1.y, new_val / control->sca2.z, 0));
		}
		else if (knob == 27)
		{
			new_val = handle_scale_knob(x, y, -PI_SIXTHS, -1.423, 0.3758);
			gui_scale_object(trace, trace->on, vec(new_val / control->sca2.x, control->sca1.y, new_val / control->sca2.z, 0));
		}
		else if (knob == 28)
		{
			angle = atan2((380 - y), (x - 200));
			if (angle > 0)
				angle -= TWO_PI;
			if (angle >= -FIVE_PI_SIXTHS)
				new_val = set_newval_big(angle, -1.7186, -FIVE_PI_SIXTHS);
			else
				new_val = set_newval_small(angle, -3.5174, -FIVE_PI_SIXTHS);
			gui_scale_object(trace, trace->on, vec(control->sca1.x, new_val / control->sca2.y, new_val / control->sca2.z, 0));
		}
		else if (knob == 29)
		{
			new_val = handle_scale_knob(x, y, PI_HALVES,  0.6714, 2.4702);
			gui_scale_object(trace, trace->on, vec(new_val / control->sca2.x, new_val / control->sca2.y, control->sca1.z, 0));
		}
		else if (knob == 30)
		{
			angle = atan2((380 - y), (x - 200));
			if (angle > PI_HALVES && angle <= M_PI)
				angle -= TWO_PI;
			if (angle >= -PI_HALVES)
			{
				angle = fmin(1.423, angle);
				angle += PI_HALVES;
				new_val = angle * 3.006 + 1;
			}
			else
			{
				angle = fmax(-0.3758 - FOUR_PI_THIRDS, angle);
				angle = -PI_HALVES - angle;
				new_val = 1 - (angle * 0.334);
			}
			gui_scale_object(trace, trace->on, vec(new_val / control->sca2.x, new_val / control->sca2.y, new_val / control->sca2.z, 0));
		}
		else if (knob == 31)
		{
			t_hyperboloid *hy = (t_hyperboloid *)(trace->curr_hy);
			delta_x = x - trace->start_x;
			double val = hy->waist_val;
			current_knob_pos = 141 + (val + 1) * 50;
     		new_knob_pos= fmax(141, fmin(241, current_knob_pos + delta_x));
			new_val = (double)(new_knob_pos - 141) / 50 - 1;
			if (new_val == -1)
				new_val = -.99;
			hy->waist_val = new_val;
			hy->waist3 = new_val * new_val * new_val;
			trace->start_x = x;
			reset_ptrack(&trace->img, *trace->obj_control, 138, 525);
			set_waistknob(&trace->img, *trace->obj_control, trace->on);
			update(con, win, trace);
			set_sca_vals(con, win, trace);
			set_type(con, win, trace->on->type);
			return (0);
		}
		set_sca(trace);
		reset_scatrack(&trace->img, *trace->obj_control, 249);
		set_scaknobs(trace, *trace->obj_control);
		update(con, win, trace);
		set_sca_vals(con, win, trace);
		set_type(con, win, trace->on->type);
	}
	else if (trace->dragging && trace->menu_open && knob != -1)
	{	
		delta_x = x - trace->start_x;
		//handle color knobs
		if (knob >= 0 && knob < 3)
		{
			obj_col = get_obj_color2(trace->on);
			if (knob == 0) current_knob_pos = 16 + (int)(obj_col.r);
			else if (knob == 1) current_knob_pos = 16 + (int)(obj_col.g);
			else if (knob == 2) current_knob_pos = 16 + (int)(obj_col.b);
     		new_knob_pos= fmax(16, fmin(271, current_knob_pos + delta_x));
     		new_val = new_knob_pos - 16;
			set_color_chan(trace->on, trace->knob, new_val);	
			reset_track(&trace->img, *trace->obj_control, 103 + trace->knob * 25);
			if (knob == 0) set_rknob(&trace->img, *trace->obj_control, trace->on);
			else if (knob == 1) set_gknob(&trace->img, *trace->obj_control, trace->on);
			else if (knob == 2) set_bknob(&trace->img, *trace->obj_control, trace->on);
		}
		//handle property knobs
		else if (knob > 2 && knob < 10)
		{
			mat	= get_obj_mat(trace->on);
			if (knob == 3) current_knob_pos = 167 + mat.amb * 100;
			else if (knob == 4) current_knob_pos = 167 + mat.diff * 100;
			else if (knob == 5) current_knob_pos = 167 + mat.spec * 100;
			else if (knob == 6 && mat.shine > 20) current_knob_pos = 167 + mat.shine / 10;
			else if (knob == 6 && mat.shine <= 20) current_knob_pos = 167 + mat.shine;
			else if (knob == 7) current_knob_pos = 167 + mat.ref * 100;
			else if (knob == 8) current_knob_pos = 167 + mat.transp * 100;
			else if (knob == 9) current_knob_pos = 167 + (mat.refract - 1) * 33;
     		new_knob_pos= fmax(167, fmin(267, current_knob_pos + delta_x));
			//set new param
			if (knob == 6)
     		{
				new_val = (double)(new_knob_pos - 167);// * 10;
				if (new_val > 20)
					new_val *= 10;
			}
			else if (knob == 9)
     			new_val = (double)(new_knob_pos - 167) / 33 + 1;
     		else
				new_val = (double)(new_knob_pos - 167) / 100;
			set_prop(trace->on, trace->on->type, knob, new_val);
			reset_ptrack(&trace->img, *trace->obj_control, 162,  213 + 25 * (knob - 3));
			set_pknobs1(&trace->img, *trace->obj_control, trace->on);
		}
		else if (knob == 32)//cam fov
		{
			t_cam *cam = (t_cam *)(trace->on->object);
			t_control control = *trace->obj_control;

			current_knob_pos = 100 + cam->fov;
     		new_knob_pos = fmax(100, fmin(280, current_knob_pos + delta_x));
			cam->fov = (int)(new_knob_pos - 100);
			reinit_viewing(trace);
			img_copyto(&trace->img, control.fov, 10, 70, control.f_width, control.f_height);
			set_fovknob(&trace->img, control, trace->on);
		}
		else//knob == 10, bump level
		{
			current_knob_pos = 167 + get_bump_level(trace->on);
     		new_knob_pos= fmax(167, fmin(267, current_knob_pos + delta_x));
     		new_val = (double)(new_knob_pos - 167);
			set_bump_level(trace->on, new_val);
			reset_ptrack(&trace->img, *trace->obj_control, 162, 453);
			set_bumpknob(&trace->img, *trace->obj_control, trace->on);
		}
		trace->start_x = x;
		update(con, win, trace);
		set_menu_vals(trace, trace->on);
	}
	else if (trace->dragging)
	{
		double factor;
		
		factor = trace->m_raydist;
		delta_x = (x - trace->start_x);
		delta_y = (y - trace->start_y);
		if (trace->shift_on)
			move = vec(0, 0, -(delta_y * factor) / 100, 0);
		else
		{
			move = add_vec(scale_vec(delta_x * factor, trace->pix_delta_rht), 
						scale_vec(delta_y * factor, trace->pix_delta_down));
		}
		trace->low_res = true;
		trace->start_x = x;
		trace->start_y = y;
		translate_object(trace, trace->on, move, 0);
		update_pos(trace);
	}
	return (0);
}
