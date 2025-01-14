#include "minirt.h"

void	increment_option(t_trace *trace, int *option)
{
	(*option)++;
	if (*option == 1 && !trace->textures)
		(*option)++;
	if (*option == 3)
		*option = 0;
}

//rotating through opts, color, texture, pattern

void	select_option(t_trace *trace, t_on *on)
{
	if (on->type == PLANE)
		increment_option(trace, &trace->curr_pl->option);
	else if (on->type == SPHERE)
		increment_option(trace, &trace->curr_sp->option);
	else if (on->type == CYLINDER)
		increment_option(trace, &trace->curr_cy->option);
	else if (on->type == CUBE)
		increment_option(trace, &trace->curr_cu->option);
	else if (on->type == HYPERBOLOID)
		increment_option(trace, &trace->curr_hy->option);
	else
		return ;
}

int	get_option(t_on *on)
{
	int	option;

	if (on->type == SPHERE)
		option = ((t_sphere *)on->object)->option;
	else if (on->type == PLANE)
		option = ((t_plane *)on->object)->option;
	else if (on->type == CYLINDER)
		option = ((t_cylinder *)on->object)->option;
	else if (on->type == CUBE)
		option = ((t_cube *)on->object)->option;
	else if (on->type == HYPERBOLOID)
		option = ((t_hyperboloid *)on->object)->option;
	else
		option = 0;
	return (option);
}

void	set_option(t_trace *trace, t_on *on, int option)
{
	if (on->type == PLANE)
		trace->curr_pl->option = option;
	else if (on->type == SPHERE)
		trace->curr_sp->option = option;
	else if (on->type == CYLINDER)
		trace->curr_cy->option = option;
	else if (on->type == CUBE)
		trace->curr_cu->option = option;
	else if (on->type == HYPERBOLOID)
		trace->curr_hy->option = option;
	else
		return ;
}

double	get_bump_level(t_on *on)
{
	if (on->type == SPHERE)
		return (((t_sphere *)on->object)->bump_level);
	else if (on->type == PLANE)
		return (((t_plane *)on->object)->bump_level);
	else if (on->type == CYLINDER)
		return (((t_cylinder *)on->object)->bump_level);
	else if (on->type == CUBE)
		return (((t_cube *)on->object)->bump_level);
	else if (on->type == HYPERBOLOID)
		return (((t_hyperboloid *)on->object)->bump_level);
	else
		return (0);
}

//rerender knobs and tracks 

void		reset_knobs(t_img *img, t_control control, t_on *on)
{
	int	i;

	if (!on->object)
		return ;	
	reset_track(img, control, 103);
	reset_track(img, control, 128);
	reset_track(img, control, 153);
	i = -1;
	while(++i < 7)
		reset_ptrack(img, control, 213 + i * 25);
	reset_ptrack(img, control, 213 + (i - 1) * 25 + 90);
	set_bumpknob(img, control, on);
	set_bknob(img, control, on);
	set_rknob(img, control, on);
	set_gknob(img, control, on);
	set_pknobs1(img, control, on);
}

bool	in_circle(int x, int y, int cx, int cy, int rad)
{	
	double	dx;
	double	dy;
	double	dist2;

	dx = x - cx;
	dy = y - cy;
	dist2 = dx * dx + dy *dy;
	return (dist2 <= rad * rad);
}

void 	set_rotknobs(t_trace *trace, t_control control)
{
	int				i;
	int				j;
	int				putxx;
	int				putxy;
	int				putyx;
	int				putyy;
	int				putzx;
	int				putzy;
	unsigned int	color;
	int				shift = control.dials_ys - 8;

	putxx = control.rotsx.i + 92;
	putxy = -control.rotsx.j + shift + 120;
	putyx = control.rotsy.i + 92;
	putyy = -control.rotsy.j + shift + 265;
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
			{
				my_pixel_put(putxx + i, putxy + j, &trace->img, color);
				my_pixel_put(putyx + i, putyy + j, &trace->img, color);
				my_pixel_put(putzx + i, putzy + j, &trace->img, color);
			}
		}
	}
}

void	set_rot_dials(t_trace *trace)
{
	t_control		control;
	int				i;
	int				j;
	int				k;

	control = *trace->obj_control;
	j = control.dials_ys - 1;
	k = 0;
	while(++j < control.m_height)
	{
		i = -1;
		while (++i < control.d_width)
			my_pixel_put(i, j, &trace->img, pixel_color_get3(i, k, control.rot_dials));
		k++;
	}
	set_rotpos(trace);
	set_rotknobs(trace, *trace->obj_control);
}

t_mat4	get_transform(t_type type, t_trace *trace)
{
	t_mat4	mat;

	if (type == PLANE)
		mat = trace->curr_pl->i_transform;
	else if (type == SPHERE)
		mat = trace->curr_sp->i_transform;
	else if (type == CYLINDER)
		mat = trace->curr_cy->i_transform;
	else if (type == HYPERBOLOID)
		mat = trace->curr_hy->i_transform;
	else if (type == CUBE)
		mat = trace->curr_cu->i_transform;
	else if (type == CAM)
		mat = inverse(trace->cam->rottran);
	else
		mat = identity();
	return (mat);
}

//gets current trans(pos) (x,y,z,0)

t_vec3	get_pos(t_on *on, t_trace *trace)
{
	t_mat4			i_tran;
	t_vec3			temp;

	i_tran = get_transform(on->type, trace);
	temp = vec(i_tran.mat[12], i_tran.mat[13], i_tran.mat[14], 0);// tran xyz0
	check_tolerance(&temp);//correct - 0s
	return (temp);
}

void	set_pos_vals(void *mlx_con, void *win, t_trace *trace)
{
	t_vec3			pos;
	char			deg[10];
	unsigned int	color;
	
	color = 0x90C4FF;
	pos = trace->obj_control->poss;

	//x
	sprintf(deg, "%.2f", pos.x);
	mlx_string_put(mlx_con, win, 281, 125 + trace->obj_control->dials_ys, color, deg);
	//y
	sprintf(deg, "%.2f", pos.y);
	mlx_string_put(mlx_con, win, 281, 270 + trace->obj_control->dials_ys, color, deg);
	//z
	sprintf(deg, "%.2f", pos.z);
	mlx_string_put(mlx_con, win, 281, 415 + trace->obj_control->dials_ys, color, deg);
}

void 	set_posknobs(t_trace *trace, t_control control)
{
	int				i;
	int				j;
	int				putxx;
	int				putxy;
	int				putyx;
	int				putyy;
	int				putzx;
	int				putzy;
	int				shift = control.dials_ys - 8;
	unsigned int	color;

	putxx = control.posx.i + 292;
	putxy = -control.posx.j + shift + 120;
	putyx = control.posy.i + 292;
	putyy = -control.posy.j + shift + 265;
	putzx = control.posz.i + 292;
	putzy = -control.posz.j + shift + 410;
	j = -1;
	while(++j < control.k_height)
	{
		i = -1;
		while (++i < control.k_width)
		{
			color = pixel_color_get3(i, j, control.pos);
			if (color != 0xFF202020)
			{
				my_pixel_put(putxx + i, putxy + j, &trace->img, color);
				my_pixel_put(putyx + i, putyy + j, &trace->img, color);
				my_pixel_put(putzx + i, putzy + j, &trace->img, color);
			}
		}
	}
}

void	set_pos(t_trace *trace)
{
	t_vec3		pos;

	pos = get_pos(trace->on, trace);
	trace->obj_control->poss = pos;

	pos.x = fmod(pos.x, 12 * M_PI) / 6;
	pos.y = fmod(pos.y, 12 * M_PI) / 6;
	pos.z = fmod(pos.z, 12 * M_PI) / 6;

	trace->obj_control->posx.i = ft_round(60 * cos(pos.x));
	trace->obj_control->posx.j = ft_round(60 * sin(pos.x));
	trace->obj_control->posy.i = ft_round(60 * cos(pos.y));
	trace->obj_control->posy.j = ft_round(60 * sin(pos.y));
	trace->obj_control->posz.i = ft_round(60 * cos(pos.z));
	trace->obj_control->posz.j = ft_round(60 * sin(pos.z));
}

void	set_pos_dials(t_trace *trace)
{
	t_control		control;
	int				i;
	int				j;
	int				l;
	int				k;
	int				lstart;

	control = *trace->obj_control;
	j = control.dials_ys - 1;
	k = 0;
	lstart = control.d_width - 1;
	while(++j < control.m_height)
	{
		i = -1;
		l = lstart;
		while (++i < control.d_width)
			my_pixel_put(++l, j, &trace->img, pixel_color_get3(i, k, control.pos_dials));
		k++;
	}
	set_pos(trace);
	set_posknobs(trace, *trace->obj_control);
}

t_mat4	get_rottran(t_type type, t_trace *trace)
{
	t_mat4	mat;

	if (type == PLANE)
		mat = trace->curr_pl->curr_rottran;
	else if (type == SPHERE)
		mat = trace->curr_sp->curr_rottran;
	else if (type == CYLINDER)
		mat = trace->curr_cy->curr_rottran;
	else if (type == HYPERBOLOID)
		mat = trace->curr_hy->curr_rottran;
	else if (type == CUBE)
		mat = trace->curr_cu->curr_rottran;
	else if (type == LIGHT)// && (trace->curr_lt->type == SPOT || trace->curr_lt->type == AREA))
		mat = trace->curr_lt->curr_rottran;
	else if (type == CAM)//must set up and a rottran
		mat = trace->cam->rottran;
	else
		mat = identity();
	return (mat);
}


//getter for rot vals packed into vec
t_vec3	get_rot(t_on *on, t_trace *trace)
{
	t_mat4			rottran;
	t_vec3			rot;
	t_vec3			temp;

	rottran = get_rottran(on->type, trace);
	temp = mat_vec_mult(rottran, vec(0, 1, 0, 0));
	rot.x = atan2(temp.z, temp.y);
	//printf("rot.x: %f\n", rot.x / DEG_TO_RAD);
	temp = mat_vec_mult(rottran, vec(0, 0, 1, 0));
	rot.y = atan2(temp.x, temp.z);

	temp = mat_vec_mult(rottran, vec(0, 1, 0, 0));
	rot.z = -atan2(temp.x, temp.y);

	return (rot);
}

//maybe do separate sets for when adjusting only one
void	set_rotpos(t_trace *trace)
{
	t_vec3		rot;

	rot = get_rot(trace->on, trace);
	trace->obj_control->rots = rot;

	trace->obj_control->rotsx.i = ft_round(60 * cos(rot.x));
	trace->obj_control->rotsx.j = ft_round(60 * sin(rot.x));
	trace->obj_control->rotsy.i = ft_round(60 * cos(rot.y));
	trace->obj_control->rotsy.j = ft_round(60 * sin(rot.y));
	trace->obj_control->rotsz.i = ft_round(60 * cos(rot.z));
	trace->obj_control->rotsz.j = ft_round(60 * sin(rot.z));
}

void	set_rot_vals(void *mlx_con, void *win, t_trace *trace)
{
	double			degrees;
	char			deg[10];
	unsigned int	color;
	t_vec3			rot;
	
	rot = trace->obj_control->rots;
	color = 0x90C4FF;
	
	//x
	degrees = rot.x / DEG_TO_RAD;
	sprintf(deg, "%.2f", degrees);
	mlx_string_put(mlx_con, win, 81, 125 + trace->obj_control->dials_ys, color, deg);
	
	//y
	degrees = rot.y / DEG_TO_RAD;
	sprintf(deg, "%.2f", degrees);
	mlx_string_put(mlx_con, win, 81, 270 + trace->obj_control->dials_ys, color, deg);
	
	//z
	degrees = rot.z / DEG_TO_RAD;
	sprintf(deg, "%.2f", degrees);
	mlx_string_put(mlx_con, win, 81, 415 + trace->obj_control->dials_ys, color, deg);
}

static inline void	set_drag(t_trace *trace, int x, int knob)
{
	trace->start_x = x;
	trace->dragging = true;
	trace->knob = knob;
}

void	check_knobs(int x, int y, t_trace *trace)
{
	t_norm_color	obj_col;
	t_mat			mat;

	obj_col = get_obj_color2(trace->on);
	mat	= get_obj_mat(trace->on);
	//color
	if (in_circle(x, y, 23 + (int)obj_col.r, 112, 8))
		set_drag(trace, x, 0);
	else if (in_circle(x, y, 23 + (int)obj_col.g, 137, 8))
		set_drag(trace, x, 1);
	else if (in_circle(x, y, 23 + (int)obj_col.b, 162, 8))
		set_drag(trace, x, 2);
	//props
	else if (in_circle(x, y, 173 + mat.amb * 100, 221 , 8))// 167 + 6 + start val
		set_drag(trace, x, 3);
	else if (in_circle(x, y, 173 + mat.diff * 100, 246, 8))
		set_drag(trace, x, 4);//diff
	else if (in_circle(x, y, 173 + mat.spec * 100, 271, 8))
		set_drag(trace, x, 5);//spec
	else if (in_circle(x, y, 173 + mat.shine / 10, 296, 8))
		set_drag(trace, x, 6);//shine
	else if (in_circle(x, y, 173 + mat.ref * 100, 321, 8))
		set_drag(trace, x, 7);//reflect
	else if (in_circle(x, y, 173 + mat.transp * 100, 346, 8))
		set_drag(trace, x, 8);//transp
	else if (in_circle(x, y, 173 + (mat.refract - 1) * 33, 371, 8))
		set_drag(trace, x, 9);//refract
	else if (in_circle(x, y, 173 + get_bump_level(trace->on), 461, 8))
		set_drag(trace, x, 10);//bump level
	else
		return ;
}


//handles presses upon the menu when open for object manipulation

int menu_press(int x, int y, t_trace *trace)
{
	trace->on_menu = true;

	if ((x >= 172 && x <= 274) && (y > 390 && y <= 413)) 
		toggle_shadow(trace, trace->on);
	else if ((x >= 172 && x <= 274) && (y > 420 && y <= 443)) 
		toggle_bump(trace, trace->on);
	else if((y > 480 && y <= 504) && (x >= 38 && x <= 142))//set the rot dials and test method
	{
		trace->obj_control->rot_open = true;
		set_rot_dials(trace);
		mlx_put_image_to_window(trace->mlx_connect, trace->mlx_win, trace->img.img_ptr, 0, 0);
	}
	else if((y > 480 && y <= 504) && (x >= 148 && x <= 252))
	{
		printf("on scale button\n");
	}
	else if((y > 480 && y <= 504) && (x >= 258 && x <= 362))
	{
		printf("on position button\n");
		trace->obj_control->pos_open = true;
		set_pos_dials(trace);
		mlx_put_image_to_window(trace->mlx_connect, trace->mlx_win, trace->img.img_ptr, 0, 0);
	}
	else if ((y > 510 && y <= 534) && (x >= 93 && x <= 197))
	{
		pop_object(trace, trace->on);
		reset_knobs(&trace->img, *(trace->obj_control), trace->on);
	}
	else if ((y > 510 && y <= 534) && (x >= 203 && x <= 307)) 
	{
		push_new_object(trace, trace->on);
		reset_knobs(&trace->img, *(trace->obj_control), trace->on);
	}
	else if ((y > 540 && y <= 564) && (x >= 93 && x <= 197))
	{
		prev_list_ob(trace, trace->on);
		reset_knobs(&trace->img, *(trace->obj_control), trace->on);
	}
	else if ((y > 540 && y <= 564) && (x >= 203 && x <= 307))
	{
		next_list_ob(trace, trace->on);
		reset_knobs(&trace->img, *(trace->obj_control), trace->on);
	}
	else if ((y > 180 && y <= 203) && (x >= 172 && x <= 274))//NEEDS fix to bug on initial use with new pushed obj
	{
		static int	i = 0;
		int			j;
		t_img		*img = &trace->img;
		t_control	control = *trace->obj_control;
		t_mat		mat;
	
		i = (i) % 9 + 1;
		printf("mat_i %d\n", i);
		mat = get_mat(i);
		change_mat(trace, trace->on, mat);
		j = -1;
		while (++j < 7)
			reset_ptrack(img, control, 213 + j * 25);
		set_pknobs1(img, control, trace->on);
	}
	else if ((y > 70 && y <= 93) && (x >= 21 && x <= 99))//color
	{
		set_option(trace, trace->on, 0);
		
	/* 	//TEST FLASH
		int x, y, i, j;
		y = 69;
		j = 0;
		while (++y < 93)
		{
			x = 20;
			i = -1;
			while (++x < 99)
			{
				my_pixel_put(++i, j, trace->obj_control->flash, pixel_color_get3(x, y, trace->obj_control->menu));
				my_pixel_put(x, y, &trace->img, 0xFFFFFF);//white test
			}
			j++;
		}
		mlx_put_image_to_window(trace->mlx_connect, trace->mlx_win, trace->img.img_ptr, 0, 0);
		set_menu_vals(trace, trace->on);
		usleep(1500);
		//maybe try on mouse button release to return color to normal
		y = 69;
		j = 0;
		while (++y < 93)
		{
			x = 20;
			i = -1;
			while (++x < 99)
				my_pixel_put(x, y, &trace->img,  pixel_color_get3(++i, j, trace->obj_control->flash));
			j++;
		} */
	}
	else if ((y > 70 && y <= 93) && (x >= 104 && x <= 189))
	{
		if (get_option(trace->on) == 1)
			set_next_tx(4, trace->textures, trace->on);
		else
			set_option(trace, trace->on, 1);
	}
	else if ((y > 70 && y <= 93) && (x >= 194 && x <= 297))
			set_option(trace, trace->on, 2);
	else
		check_knobs(x, y, trace);
	return (0);
}


int	rotation_press(int x, int y, t_trace *trace, t_control control)
{
	//rotation knobs 11, 12, 13 for x, y, z //rotx, 10 = bump knob
	
	if (in_circle(x, y, control.rotsx.i + 100, -control.rotsx.j + control.dials_ys + 120, 8))
	{
		trace->dragging = true;
		trace->start_xangle = atan2(control.rotsx.j, control.rotsx.i);
		trace->knob = 11;
		/* printf("sxangle: %f\n", trace->start_xangle / DEG_TO_RAD);
		printf("rotsxi_press: %d\n", trace->obj_control->rotsx.i);
		printf("rotsxj_press: %d\n", trace->obj_control->rotsx.j); */
	}
	else if (in_circle(x, y, control.rotsy.i + 100, -control.rotsy.j + control.dials_ys + 265, 8))
	{
		trace->dragging = true;
		trace->start_yangle = atan2(control.rotsy.j, control.rotsy.i);
		trace->knob = 12;
	}
	else if (in_circle(x, y, control.rotsz.i + 100, -control.rotsz.j + control.dials_ys + 410, 8))
	{
		trace->dragging = true;
		trace->start_zangle = atan2(control.rotsz.j, control.rotsz.i);
		trace->knob = 13;
	}
	return (0);
}

int	position_press(int x, int y, t_trace *trace, t_control control)
{
	if (in_circle(x, y, control.posx.i + 300, -control.posx.j + control.dials_ys + 120, 8))
	{
		trace->dragging = true;
		trace->start_xangle = fmod(control.poss.x, 12 * M_PI) / 6;
		trace->knob = 14;
	}
	else if (in_circle(x, y, control.posy.i + 300, -control.posy.j + control.dials_ys + 265, 8))
	{
		trace->dragging = true;
		trace->start_yangle = fmod(control.poss.y, 12 * M_PI) / 6;
		trace->knob = 15;
	}
	else if (in_circle(x, y, control.posz.i + 300, -control.posz.j + control.dials_ys + 410, 8))
	{
		trace->dragging = true;
		trace->start_zangle = fmod(control.poss.z, 12 * M_PI) / 6;
		trace->knob = 16;
	}
	return (0);
}

//handle mouse hooks,//try making a struct that contains the stuff for hooks sep of trace

int	mouse_press(int button, int x, int y, t_trace *trace)
{
	t_norm_color	curr_col;
	int				option;
	t_control 		control;

	control = *trace->obj_control;
	if (button == 1)
	{
		if (trace->menu_open && x < control.m_width && y < control.m_height)
		{		
			if (control.rot_open && x < control.d_width && y > control.dials_ys && y < control.m_height)
			{
				trace->obj_control->on_dials = true;
				rotation_press(x, y, trace, *trace->obj_control);
			}
			else if (control.pos_open && x > control.d_width && y > control.dials_ys && y < control.m_height)
			{
				trace->obj_control->on_dials = true;
				position_press(x, y, trace, *trace->obj_control);
			}
			else
			{
				trace->obj_control->on_dials = false;
				menu_press(x, y, trace);
			}
		}
		else
		{
			if (trace->on_menu)
			{
				trace->on_menu = false;
				trace->obj_control->on_dials = false;
				trace->obj_control->rot_open = false;
				trace->obj_control->pos_open = false;
			}
			track_object(trace, x, y);
		}
	}
	if (trace->on->object == NULL)
		return (0);
	option = get_option(trace->on);
	if (button == 3)
		select_option(trace, trace->on);
	else if (button == 4 || button == 5)
	{
		if (option == 0)
			rotate_colors(trace, button, &curr_col);
		else if (option == 1)
			set_next_tx(button, trace->textures, trace->on);
	}

	if (trace->menu_open && !trace->on_menu)
		trace->menu_open = false;
	if (trace->on_menu && trace->menu_open)
	{
		if (!trace->obj_control->on_dials)
		{
			if (control.rot_open && !trace->obj_control->pos_open)
			{
				trace->obj_control->rot_open = false;
				set_controls(trace);
			}
			if (control.pos_open && !trace->obj_control->rot_open)
			{
				trace->obj_control->pos_open = false;
				set_controls(trace);
			}
			if (control.pos_open && control.rot_open)
			{
				trace->obj_control->rot_open = false;
				trace->obj_control->pos_open = false;
				set_controls(trace);
			}
		}
		mlx_put_image_to_window(trace->mlx_connect,
		trace->mlx_win, trace->img.img_ptr, 0, 0);
		if (!trace->obj_control->pos_open)
			set_menu_vals(trace, trace->on);
		else
		{
			set_pos_vals(trace->mlx_connect, trace->mlx_win, trace);
			set_type(trace->mlx_connect, trace->mlx_win, trace->on->type);
		}
		if (trace->obj_control->rot_open)
			set_rot_vals(trace->mlx_connect, trace->mlx_win, trace);
	}
	else
		render(trace);
	return (0);
}
