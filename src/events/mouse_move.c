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
		sp->mat.preset = 9;
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
		pl->mat.preset = 9;
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
		cy->mat.preset = 9;
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
		hy->mat.preset = 9;
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
		cu->mat.preset = 9;
	}
}

//set col track, + 25 in y for next
void	reset_track(t_img *img, t_control control, int move_y)//move_y 103, 128, 153
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

void	reset_ptrack(t_img *img, t_control control, int move_y)//x162 y213
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
				my_pixel_put(162 + i, move_y + j, img, color);
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
	int x_startsh = 165 + mat.shine / 10;
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
		if (trace->low_res)
			trace->low_flag = true;
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
		if (trace->low_flag)
			trace->low_inc = 10;
		else
			trace->low_res = false;
		trace->low_flag = false;
		if (!trace->menu_open)
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


void 	set_rotknobx(t_trace *trace, t_control control)
{
	int				i;
	int				j;
	int				putxx;
	int				putxy;
	int				shift = control.dials_ys - 8;
	unsigned int	color;

	putxx = control.rotsx.i + 92;
	putxy = -control.rotsx.j + shift + 120;

	/* printf("putxx: %d\n", putxx);
	printf("putxy: %d\n", putxy); */
	j = -1;
	while(++j < control.k_height)
	{
		i = -1;
		while (++i < control.k_width)
		{
			color = pixel_color_get3(i, j, control.rot);
			if (color != 0xFF202020)
				my_pixel_put(putxx + i, putxy + j, &trace->img, color);
		}
	}
}

void 	set_rotknoby(t_trace *trace, t_control control)
{
	int				i;
	int				j;
	int				putyx;
	int				putyy;
	int				shift = control.dials_ys - 8;
	unsigned int	color;

	putyx = control.rotsy.i + 92;
	putyy = -control.rotsy.j + shift + 265;
	j = -1;
	while(++j < control.k_height)
	{
		i = -1;
		while (++i < control.k_width)
		{
			color = pixel_color_get3(i, j, control.rot);
			if (color != 0xFF202020)
				my_pixel_put(putyx + i, putyy + j, &trace->img, color);
		}
	}
}

void 	set_rotknobz(t_trace *trace, t_control control)
{
	int				i;
	int				j;
	int				putzx;
	int				putzy;
	int				shift = control.dials_ys - 8;
	unsigned int	color;

	putzx = control.rotsz.i + 92;
	putzy = -control.rotsz.j + shift + 410;

	j = -1;
	while(++j < control.k_height)
	{
		i = -1;
		while (++i < control.k_width)
		{
			color = pixel_color_get3(i, j, control.rot);
			if (color != 0xFF202020)
				my_pixel_put(putzx + i, putzy + j, &trace->img, color);
		}
	}
}

void	reset_dtrack(t_img *img, t_control control, int move_y)//x30 y120 +?
{
	int				j;
	int				i;
	unsigned int	color;

	move_y += control.dials_ys; 
	j = -1;
	while (++j < control.dt_height)
	{
		i = -1;
		while (++i < control.dt_width)
		{
			color = pixel_color_get3(i, j, control.dtx);
			my_pixel_put(30 + i, move_y + j, img, color);
		}
	}
}

void	reset_dptrackx(t_img *img, t_control control, int move_y)
{
	int				j;
	int				i;
	unsigned int	color;

	move_y += control.dials_ys; 
	j = -1;
	while (++j < control.dt_height)
	{
		i = -1;
		while (++i < control.dt_width)
		{
			color = pixel_color_get3(i, j, control.dtpx);
			my_pixel_put(230 + i, move_y + j, img, color);
		}
	}
}

void	reset_dptracky(t_img *img, t_control control, int move_y)
{
	int				j;
	int				i;
	unsigned int	color;

	move_y += control.dials_ys; 
	j = -1;
	while (++j < control.dt_height)
	{
		i = -1;
		while (++i < control.dt_width)
		{
			color = pixel_color_get3(i, j, control.dtpy);
			my_pixel_put(230 + i, move_y + j, img, color);
		}
	}
}

void	reset_dptrackz(t_img *img, t_control control, int move_y)
{
	int				j;
	int				i;
	unsigned int	color;

	move_y += control.dials_ys; 
	j = -1;
	while (++j < control.dt_height)
	{
		i = -1;
		while (++i < control.dt_width)
		{
			color = pixel_color_get3(i, j, control.dtpz);
			my_pixel_put(230 + i, move_y + j, img, color);
		}
	}
}

void	reset_dtracky(t_img *img, t_control control, int move_y)
{
	int				j;
	int				i;
	unsigned int	color;

	move_y += control.dials_ys; 
	j = -1;
	while (++j < control.dt_height)
	{
		i = -1;
		while (++i < control.dt_width)
		{
			color = pixel_color_get3(i, j, control.dty);
			my_pixel_put(30 + i, move_y + j, img, color);
		}
	}
}

void	reset_dtrackz(t_img *img, t_control control, int move_y)
{
	int				j;
	int				i;
	unsigned int	color;

	move_y += control.dials_ys; 
	j = -1;
	while (++j < control.dt_height)
	{
		i = -1;
		while (++i < control.dt_width)
		{
			color = pixel_color_get3(i, j, control.dtz);
			my_pixel_put(30 + i, move_y + j, img, color);
		}
	}
}

void	set_rotposx(t_trace *trace)
{
	t_mat4			rottran;
	t_vec3			temp;
	t_vec3			rot;

	rottran = get_rottran(trace->on->type, trace);
	temp = mat_vec_mult(rottran, vec(0, 1, 0, 0));
	rot.x = atan2(temp.z, temp.y);
	trace->obj_control->rots.x = rot.x;
	trace->obj_control->rotsx.i = ft_round(60 * cos(rot.x));
	trace->obj_control->rotsx.j = ft_round(60 * sin(rot.x));
}

void	set_rotposy(t_trace *trace)
{
	t_mat4			rottran;
	t_vec3			temp;
	t_vec3			rot;

	rottran = get_rottran(trace->on->type, trace);
	temp = mat_vec_mult(rottran, vec(0, 0, 1, 0));
	rot.y = atan2(temp.x, temp.z);
	trace->obj_control->rots.y = rot.y;
	trace->obj_control->rotsy.i = ft_round(60 * cos(rot.y));
	trace->obj_control->rotsy.j = ft_round(60 * sin(rot.y));
}

void	set_rotposz(t_trace *trace)
{
	t_mat4			rottran;
	t_vec3			temp;
	t_vec3			rot;

	rottran = get_rottran(trace->on->type, trace);
	temp = mat_vec_mult(rottran, vec(0, 1, 0, 0));
	rot.z = -atan2(temp.x, temp.y);
	trace->obj_control->rots.z = rot.z;
	trace->obj_control->rotsz.i = ft_round(60 * cos(rot.z));
	trace->obj_control->rotsz.j = ft_round(60 * sin(rot.z));
}

void	set_bump_level(t_on *on, double new_val)
{
	t_type	type;
	
	type = on->type;
	if (type == SPHERE)
		((t_sphere *)on->object)->bump_level = new_val;
	else if (type == PLANE)
		((t_plane *)on->object)->bump_level = new_val;
	else if (type == CYLINDER)
		((t_cylinder *)on->object)->bump_level = new_val;
	else if (type == CUBE)
		((t_cube *)on->object)->bump_level = new_val;
	else if (type == HYPERBOLOID)
		((t_hyperboloid *)on->object)->bump_level = new_val;
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
	//t_control		control = *trace->obj_control;
	double			angle;
	double			delta_angle;

	knob = trace->knob;
	if (trace->dragging && trace->obj_control->rot_open && knob < 14)//mostly working... look into getting current rotation methods...
	{//cx 100, cy 120 + dials_ys
		if (knob == 11)
		{
			angle = atan2((120 + trace->obj_control->dials_ys - y), x - 100);//opti, 120 + dials_ys everywhere
			delta_angle = angle - trace->start_xangle;
		
			//apply angle rotation to on
			rotate_object(trace, trace->on, rot_x(delta_angle));
			trace->start_xangle = angle;

			/* set_rotposx(trace);
			reset_dtrack(&trace->img, *trace->obj_control, 50);
			set_rotknobx(trace, *trace->obj_control); */
		}
		else if (knob == 12)
		{
			angle = atan2((265 + trace->obj_control->dials_ys - y), x - 100);
			delta_angle = angle - trace->start_yangle;
			rotate_object(trace, trace->on, rot_y(delta_angle));
			trace->start_yangle = angle;
			
			/* set_rotposy(trace);
			reset_dtracky(&trace->img, *trace->obj_control, 195);
			set_rotknoby(trace, *trace->obj_control); */

		}
		else if (knob == 13)
		{
			angle = atan2((410 + trace->obj_control->dials_ys - y), x - 100);
			delta_angle = angle - trace->start_zangle;
		
			//delta_angle = floor(delta_angle / DEG_TO_RAD) * DEG_TO_RAD;//magnetize to whole degrees, knob drag is bad with
			rotate_object(trace, trace->on, rot_z(delta_angle));
			trace->start_zangle = angle;

			/* set_rotposz(trace);
			reset_dtrackz(&trace->img, *trace->obj_control, 340);
			set_rotknobz(trace, *trace->obj_control); */

		}
		set_rotpos(trace);//more test
		reset_dtrack(&trace->img, *trace->obj_control, 50);
		reset_dtracky(&trace->img, *trace->obj_control, 195);
		reset_dtrackz(&trace->img, *trace->obj_control, 340);
		set_rotknobs(trace, *trace->obj_control);


		mlx_put_image_to_window(trace->mlx_connect,
		trace->mlx_win, trace->img.img_ptr, 0, 0);
		set_rot_vals(trace->mlx_connect, trace->mlx_win, trace);//try setting for each knob using the trace->xyzangle val for the other two. or maybe just adjust 1 inrots
		if (!trace->obj_control->pos_open)
			set_menu_vals(trace, trace->on);
		else
		{
			set_pos_vals(trace->mlx_connect, trace->mlx_win, trace);
			set_type(trace->mlx_connect, trace->mlx_win, trace->on->type);
		}
	}
	else if (trace->dragging && trace->obj_control->pos_open)
	{//cx 300, cy 120 + dials_ys
		if (knob == 14)
		{
			angle = atan2((120 + trace->obj_control->dials_ys - y), x - 300);
			delta_angle = angle - trace->start_xangle;
			if (delta_angle > M_PI)
            	delta_angle -= 2 * M_PI;
        	else if (delta_angle < -M_PI)
            	delta_angle += 2 * M_PI;
			translate_object(trace, trace->on, vec(delta_angle * 6, 0, 0, 0));
			trace->start_xangle = angle;
		}
		else if (knob == 15)
		{
			angle = atan2((265 + trace->obj_control->dials_ys - y), x - 300);
			delta_angle = angle - trace->start_yangle;
			if (delta_angle > M_PI)
            	delta_angle -= 2 * M_PI;
        	else if (delta_angle < -M_PI)
            	delta_angle += 2 * M_PI;
			translate_object(trace, trace->on, vec(0, delta_angle * 6, 0, 0));
			trace->start_yangle = angle;
		}
		else if (knob == 16)
		{
			angle = atan2((410 + trace->obj_control->dials_ys - y), x - 300);
			delta_angle = angle - trace->start_zangle;
			if (delta_angle > M_PI)
            	delta_angle -= 2 * M_PI;
        	else if (delta_angle < -M_PI)
            	delta_angle += 2 * M_PI;
			translate_object(trace, trace->on, vec(0, 0, delta_angle * 6, 0));
			trace->start_zangle = angle;
		}
		set_pos(trace);
		reset_dptrackx(&trace->img, *trace->obj_control, 50);
		reset_dptracky(&trace->img, *trace->obj_control, 195);
		reset_dptrackz(&trace->img, *trace->obj_control, 340);
		set_posknobs(trace, *trace->obj_control);


		mlx_put_image_to_window(trace->mlx_connect,
		trace->mlx_win, trace->img.img_ptr, 0, 0);
		if (trace->obj_control->rot_open)
			set_rot_vals(trace->mlx_connect, trace->mlx_win, trace);
		if (!trace->obj_control->pos_open)
			set_menu_vals(trace, trace->on);
		else
		{
			set_pos_vals(trace->mlx_connect, trace->mlx_win, trace);
			set_type(trace->mlx_connect, trace->mlx_win, trace->on->type);
		}
	}
	else if (trace->dragging && trace->menu_open)
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
			else if (knob == 6) current_knob_pos = 167 + mat.shine / 10;
			else if (knob == 7) current_knob_pos = 167 + mat.ref * 100;
			else if (knob == 8) current_knob_pos = 167 + mat.transp * 100;
			else if (knob == 9) current_knob_pos = 167 + (mat.refract - 1) * 33;

			//printf("curr_pos : %d\n", current_knob_pos);
     		new_knob_pos= fmax(167, fmin(267, current_knob_pos + delta_x));
			//set new param
			//printf("new_pos : %d\n", new_knob_pos);
			if (knob == 6)
     			new_val = (double)(new_knob_pos - 167) * 10;//new param value
			else if (knob == 9)
     			new_val = (double)(new_knob_pos - 167) / 33 + 1;
     		else
				new_val = (double)(new_knob_pos - 167) / 100;

			//printf("new_val : %f\n", new_val);

			set_prop(trace->on, trace->on->type, knob, new_val);
			reset_ptrack(&trace->img, *trace->obj_control, 213 + 25 * (knob - 3));//x162 y213
			set_pknobs1(&trace->img, *trace->obj_control, trace->on);
		}
		else//knob == 10, bump level
		{
			current_knob_pos = 167 + get_bump_level(trace->on);
     		new_knob_pos= fmax(167, fmin(267, current_knob_pos + delta_x));
     		new_val = (double)(new_knob_pos - 167);
			set_bump_level(trace->on, new_val);
			reset_ptrack(&trace->img, *trace->obj_control, 453);
			set_bumpknob(&trace->img, *trace->obj_control, trace->on);
		}
		trace->start_x = x;
		mlx_put_image_to_window(trace->mlx_connect,
		trace->mlx_win, trace->img.img_ptr, 0, 0);
		set_menu_vals(trace, trace->on);
	}
	//distz = (typecast)trace->on->cen.z - trace->cam->cen.z; replace * 10 = * distz;//adjust as needed
	//movement must be scaled by a factor based on current distance from cam in z dir, need to fix
	else if (trace->dragging)
	{
		trace->low_res = true;
		trace->low_inc = 66;
		delta_x = (x - trace->start_x);
		delta_y = (y - trace->start_y);
		
		if (trace->shift_on)
			move = vec(0, 0, delta_y / 5, 0);
		else
		{
			move = add_vec(scale_vec(delta_x * 10, trace->pix_delta_rht), 
						scale_vec(delta_y * 10, trace->pix_delta_down));
		}
		translate_object(trace, trace->on, move);
		trace->start_x = x;
		trace->start_y = y;
		render(trace);
	}

	return (0);
}