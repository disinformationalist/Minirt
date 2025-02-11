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

	if (on->object == NULL)
		return(0);
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
	if (on->object == NULL)
		return (0);
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
	
	reset_track(img, control, 103);
	reset_track(img, control, 128);
	reset_track(img, control, 153);
	i = -1;
	while(++i < 7)
		reset_ptrack(img, control, 162, 213 + i * 25);
	reset_ptrack(img, control, 162, 303 + (i - 1) * 25);
	set_bumpknob(img, control, on);
	set_bknob(img, control, on);
	set_rknob(img, control, on);
	set_gknob(img, control, on);
	set_pknobs1(img, control, on);
}

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

	putxx = control.rotsx.i + 92;
	putxy = -control.rotsx.j + 182;
	putyx = control.rotsy.i + 92;
	putyy = -control.rotsy.j + 327;
	putzx = control.rotsz.i + 92;
	putzy = -control.rotsz.j + 472;
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

t_mat4	get_curr_scale(t_type type, t_trace *trace)
{
	t_mat4	mat;

	if (!trace->on->object)
		return (identity());
	if (type == PLANE)
		mat = trace->curr_pl->curr_scale;
	else if (type == SPHERE)
		mat = trace->curr_sp->curr_scale;
	else if (type == CYLINDER)
		mat = trace->curr_cy->curr_scale;
	else if (type == HYPERBOLOID)
		mat = trace->curr_hy->curr_scale;
	else if (type == CUBE)
		mat = trace->curr_cu->curr_scale;
	else
		mat = identity();
	return (mat);
}

//gets current trans(pos) (x,y,z,0)

t_vec3	get_pos(t_on *on, t_trace *trace)
{
	t_mat4			i_tran;
	t_vec3			temp;

	if (on->type == LIGHT)
	{
		t_light *lt = (t_light *)(on->object);
		temp = lt->center;
	}
	else
	{
		i_tran = get_transform(on->type, trace);
		temp = vec(i_tran.mat[12], i_tran.mat[13], i_tran.mat[14], 0);// tran xyz0
	}
	check_tolerance(&temp);//correct - 0s
	return (temp);
}

t_vec3	get_init_scale(t_on *on, t_trace *trace)
{
	t_vec3			temp;
	t_type			type = on->type;
	double			val;

	if (type == PLANE || !trace->on->object)
		temp = vec(1, 1, 1, 0);
	else if (type == SPHERE)
	{
		val = trace->curr_sp->radius;
		temp = vec(val, val, val, 0);
	}
	else if (type == CYLINDER)
	{
		val = trace->curr_cy->radius;
		temp = vec(val, trace->curr_cy->height / 2, val, 0);
	}
	else if (type == HYPERBOLOID)
	{
		val = trace->curr_hy->rad;
		temp = vec(val, trace->curr_hy->height / 2, val, 0);
	}
	else if (type == CUBE)
	{
		t_cube *cube = trace->curr_cu;
		temp = vec(cube->h_width, cube->h_height, cube->h_depth, 0);
	}
	return (temp);
}

t_vec3	get_scale(t_on *on, t_trace *trace)
{
	t_mat4			scale;
	t_vec3			temp;

	scale = get_curr_scale(on->type, trace);
	temp = vec(scale.mat[0], scale.mat[5], scale.mat[10], 0);
	check_tolerance(&temp);
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
	mlx_string_put(mlx_con, win, 281, 195, color, deg);
	//y
	sprintf(deg, "%.2f", pos.y);
	mlx_string_put(mlx_con, win, 281, 340, color, deg);
	//z
	sprintf(deg, "%.2f", pos.z);
	mlx_string_put(mlx_con, win, 281, 485, color, deg);
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
	unsigned int	color;

	putxx = control.posx.i + 292;
	putxy = -control.posx.j + 182;
	putyx = control.posy.i + 292;
	putyy = -control.posy.j + 327;
	putzx = control.posz.i + 292;
	putzy = -control.posz.j + 472;
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
	t_control	*con = trace->obj_control;

	pos = get_pos(trace->on, trace);
	con->poss = pos;
	pos.x = fmod(pos.x, 10) * PI_FIFTHS; 
	pos.y = fmod(pos.y, 10) * PI_FIFTHS;
	pos.z = fmod(pos.z, 10) * PI_FIFTHS;
	con->posx.i = ft_round(60 * cos(pos.x));
	con->posx.j = ft_round(60 * sin(pos.x));
	con->posy.i = ft_round(60 * cos(pos.y));
	con->posy.j = ft_round(60 * sin(pos.y));
	con->posz.i = ft_round(60 * cos(pos.z));
	con->posz.j = ft_round(60 * sin(pos.z));
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
	j = 69;
	k = 0;
	lstart = control.d_width - 1;
	while (++j < control.m_height)
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

void	set_sca_vals(void *mlx_con, void *win, t_trace *trace)
{
	t_vec3			pos;
	char			val[10];
	unsigned int	color;
	int				shifty;
	double			waist;

	color = 0x90C4FF;
	pos = trace->obj_control->sca1;
	shifty = 225;
	//y
	sprintf(val, "%.2f", pos.y);
	mlx_string_put(mlx_con, win, 86, shifty, color, val);
	//x
	sprintf(val, "%.2f", pos.x);
	mlx_string_put(mlx_con, win, 191, shifty, color, val);
	//z
	sprintf(val, "%.2f", pos.z);
	mlx_string_put(mlx_con, win, 296, shifty, color, val);
	if (trace->on->type == HYPERBOLOID)
	{
		waist = ((t_hyperboloid *)(trace->on->object))->waist_val;
		sprintf(val, "%.2f", waist);
		mlx_string_put(mlx_con, win, 296, 539, color, val);
	}
}

void 	set_scaknobs(t_trace *trace, t_control control)
{
	int				i;
	int				j;
	int				putxx;
	int				putxy;
	int				putyx;
	int				putyy;
	int				putzx;
	int				putzy;
	int				putxx2;
	int				putxy2;
	int				putyx2;
	int				putyy2;
	int				putzx2;
	int				putzy2;
	int				putxyzx;
	int				putxyzy;
	int				shift = 371;
	int				xshift = 191;
	unsigned int	color;

	//c1
	putxx = control.sca1x.i + xshift;
	putxy = -control.sca1x.j + shift;
	putyx = control.sca1y.i + xshift;
	putyy = -control.sca1y.j + shift;
	putzx = control.sca1z.i + xshift;
	putzy = -control.sca1z.j + shift;
	//c2
	putxx2 = control.sca2x.i + xshift;
	putxy2 = -control.sca2x.j + shift;
	putyx2 = control.sca2y.i + xshift;
	putyy2 = -control.sca2y.j + shift;
	putzx2 = control.sca2z.i + xshift;
	putzy2 = -control.sca2z.j + shift;
	//c3
	putxyzx = control.scale_xyz.i + xshift;
	putxyzy = -control.scale_xyz.j + shift;

	j = -1;
	while(++j < control.k_height)
	{
		i = -1;
		while (++i < control.k_width)
		{
			color = pixel_color_get3(i, j, control.sca);
			if (color != 0xFF202020)
			{
				my_pixel_put(putxx + i, putxy + j, &trace->img, color);
				my_pixel_put(putyx + i, putyy + j, &trace->img, color);
				my_pixel_put(putzx + i, putzy + j, &trace->img, color);

				my_pixel_put(putxyzx + i, putxyzy + j, &trace->img, color);
			}
			color = pixel_color_get3(i, j, control.sil);
			if (color != 0xFF202020)
			{
				my_pixel_put(putxx2 + i, putxy2 + j, &trace->img, color);
				my_pixel_put(putyx2 + i, putyy2 + j, &trace->img, color);
				my_pixel_put(putzx2 + i, putzy2 + j, &trace->img, color);
			}
		}
	}
}

void	set_sca(t_trace *trace)
{
	t_vec3		scale;
	t_vec3		init_scale;
	t_vec3		scale_2;
	double		scale_3;
	t_on		*on = trace->on;
	t_control	*con = trace->obj_control;


	scale = get_scale(on, trace);
	init_scale = get_init_scale(on, trace);
	scale.x = 1 / (scale.x);
	scale.y = 1 / (scale.y);
	scale.z = 1 / (scale.z);

	trace->obj_control->sca1 = scale;

	init_scale.x = 1 / (init_scale.x);
	init_scale.y = 1 / (init_scale.y);
	init_scale.z = 1 / (init_scale.z);
	trace->obj_control->sca2 = init_scale;
 
	//remove init
	scale = mult_vec(scale, init_scale);

	//xz, yz, xy, leave amount common to pair
	scale_2.x = fmin(scale.x, scale.z);
	scale_2.y = fmin(scale.y, scale.z);
	scale_2.z = fmin(scale.y, scale.x);
	scale_3 = fmin(scale_2.x, scale.y);

	//convert scale val to angle
	//scale = individual
	if (scale.x >= 1)
		scale.x = (scale.x - 1) / 10.01 + PI_SIXTHS;
	else
		scale.x = PI_SIXTHS - (1 - scale.x) / 1.111;
	if (scale.y >= 1)
		scale.y = (scale.y - 1) / 10.01 + FIVE_PI_SIXTHS;
	else
		scale.y = FIVE_PI_SIXTHS - (1 - scale.y) / 1.111;
	if (scale.z >= 1)
		scale.z = (scale.z - 1) / 10.01 - PI_HALVES;
	else
		scale.z = -PI_HALVES - (1 - scale.z) / 1.111;
	//scale_2 = pairs
	if (scale_2.x >= 1)
		scale_2.x = (scale_2.x - 1) / 10.01 - PI_SIXTHS;
	else
		scale_2.x = -PI_SIXTHS - (1 - scale_2.x) / 1.111;
	if (scale_2.y >= 1)
		scale_2.y = (scale_2.y - 1) / 10.01 - FIVE_PI_SIXTHS;
	else
		scale_2.y = - FIVE_PI_SIXTHS - (1 - scale_2.y) / 1.111;
	if (scale_2.z >= 1)
		scale_2.z = (scale_2.z - 1) / 10.01 + PI_HALVES;
	else
		scale_2.z = PI_HALVES - (1 - scale_2.z) / 1.111;
	//scale_3 = all
	if (scale_3 >= 1)
		scale_3 = (scale_3 - 1) / 3.006 - PI_HALVES;
	else
		scale_3 = -PI_HALVES - (1 - scale_3) * 3;

	//1st circle
	con->sca1x.i = ft_round(60 * cos(scale.x));//rad*cos(angle)
	con->sca1x.j = ft_round(60 * sin(scale.x));
	con->sca1y.i = ft_round(60 * cos(scale.y));
	con->sca1y.j = ft_round(60 * sin(scale.y));
	con->sca1z.i = ft_round(60 * cos(scale.z));
	con->sca1z.j = ft_round(60 * sin(scale.z));
	//2nd
	con->sca2x.i = ft_round(90 * cos(scale_2.x));
	con->sca2x.j = ft_round(90 * sin(scale_2.x));
	con->sca2y.i = ft_round(90 * cos(scale_2.y));
	con->sca2y.j = ft_round(90 * sin(scale_2.y));
	con->sca2z.i = ft_round(90 * cos(scale_2.z));
	con->sca2z.j = ft_round(90 * sin(scale_2.z));
	//3rd
	con->scale_xyz.i = ft_round(120 * cos(scale_3));
	con->scale_xyz.j = ft_round(120 * sin(scale_3));
}

void	img_copyto(t_img *to, t_img *from, int xs, int ys, int width, int height)
{
	int i, j;
	
	j = -1;
	while (++j < height)
	{
		i = -1;
		while (++i < width)
			my_pixel_put(xs + i, ys + j, to, pixel_color_get3(i, j, from));
	}
}

void	set_sca_dials(t_trace *trace)
{
	t_control		control;
	int				i;
	int				j;
	int				l;
	int				k;
	int				lstart;

	control = *trace->obj_control;
	j = 139;
	k = 0;
	lstart = -1;
	while (++j < control.m_height)
	{
		i = -1;
		l = lstart;
		while (++i < control.sd_width)
			my_pixel_put(++l, j, &trace->img, pixel_color_get3(i, k, control.sca_dials));
		k++;
	}
	set_sca(trace);
	set_scaknobs(trace, *trace->obj_control);
	if (trace->on && trace->on->type == HYPERBOLOID)
	{
		img_copyto(&trace->img, control.waist, 54, 522, control.w_width, control.w_height);
		set_waistknob(&trace->img, control, trace->on);
	}
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
	else if (type == LIGHT)
	{
		if (trace->curr_lt->type == AREA)
			mat = trace->curr_lt->emitter->curr_rottran;
		else
			mat = trace->curr_lt->curr_rottran;
	}
	else if (type == CAM)
		mat = trace->cam->rottran;
	else
		mat = identity();
	return (mat);
}

t_vec3	get_rots(t_type type, t_trace *trace)
{
	t_vec3	rots;

	if (type == PLANE)
		rots = trace->curr_pl->rots;
	else if (type == SPHERE)
		rots = trace->curr_sp->rots;
	else if (type == CYLINDER)
		rots = trace->curr_cy->rots;
	else if (type == HYPERBOLOID)
		rots = trace->curr_hy->rots;
	else if (type == CUBE)
		rots = trace->curr_cu->rots;
	else if (type == LIGHT)
	{
		if (trace->curr_lt->type == AREA)
			rots = trace->curr_lt->emitter->rots;
		else
			rots = trace->curr_lt->rots;
	}
	else if (type == CAM)
		rots = trace->cam->rots;
	else
		rots = vec(0, 0, 0, 0);
	return (rots);
}

void	set_rots(t_type type, t_trace *trace, t_vec3 new)
{
	if (type == PLANE)
		trace->curr_pl->rots = new;
	else if (type == SPHERE)
		trace->curr_sp->rots = new;
	else if (type == CYLINDER)
		trace->curr_cy->rots = new;
	else if (type == HYPERBOLOID)
		trace->curr_hy->rots = new;
	else if (type == CUBE)
		trace->curr_cu->rots = new;
	else if (type == LIGHT)
	{
		if (trace->curr_lt->type == AREA)
			trace->curr_lt->emitter->rots = new;
		else
			trace->curr_lt->rots = new;
	}
	else if (type == CAM)
		trace->cam->rots = new;
	else
		return ;
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
	if (rot.x < 0)
		rot.x += TWO_PI;
	
	temp = mat_vec_mult(rottran, vec(0, 0, 1, 0));
	rot.y = atan2(temp.x, temp.z);
	if (rot.y < 0)
		rot.y += TWO_PI;

	temp = mat_vec_mult(rottran, vec(0, 1, 0, 0));
	rot.z = -atan2(temp.x, temp.y);
	if (rot.z < 0)
		rot.z += TWO_PI;

	check_tolerance(&temp);//correct - 0s
	return (rot);
}

//set the x and y vals for the angle

void	set_rotpos(t_trace *trace)
{
	t_vec3		rot;
	t_control	*con =  trace->obj_control;
	/* set_rots(trace->on->type, trace, extract_rot(get_rottran(trace->on->type, trace)));
	rot = get_rots(trace->on->type, trace); */
	rot = get_rot(trace->on, trace);
	//rot = extract_rot(get_rottran(trace->on->type, trace));
	con->rots = rot;
	con->rotsx.i = ft_round(60 * cos(rot.x));
	con->rotsx.j = ft_round(60 * sin(rot.x));
	con->rotsy.i = ft_round(60 * cos(rot.y));
	con->rotsy.j = ft_round(60 * sin(rot.y));
	con->rotsz.i = ft_round(60 * cos(rot.z));
	con->rotsz.j = ft_round(60 * sin(rot.z));
}

void	set_rot_vals(void *mlx_con, void *win, t_trace *trace)
{
	double			degrees;
	char			deg[10];
	unsigned int	color;
	t_vec3			rot;

	rot = trace->obj_control->rots;
	check_tolerance(&rot);

	color = 0x90C4FF;
	//x
	degrees = rot.x / DEG_TO_RAD;
	sprintf(deg, "%.2f", degrees);
	mlx_string_put(mlx_con, win, 81, 195, color, deg);
	//y
	degrees = rot.y / DEG_TO_RAD;
	sprintf(deg, "%.2f", degrees);
	mlx_string_put(mlx_con, win, 81, 340, color, deg);
	//z
	degrees = rot.z / DEG_TO_RAD;
	sprintf(deg, "%.2f", degrees);
	mlx_string_put(mlx_con, win, 81, 485, color, deg);
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
	t_on			*on = trace->on;

	obj_col = get_obj_color2(on);
	mat	= get_obj_mat(on);
	int s_val;
	if (mat.shine > 20)
		s_val = mat.shine / 10;
	else
		s_val = mat.shine;
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
	else if (in_circle(x, y, 173 + s_val, 296, 8))//shine / 10
		set_drag(trace, x, 6);//shine
	else if (in_circle(x, y, 173 + mat.ref * 100, 321, 8))
		set_drag(trace, x, 7);//reflect
	else if (in_circle(x, y, 173 + mat.transp * 100, 346, 8))
		set_drag(trace, x, 8);//transp
	else if (in_circle(x, y, 173 + (mat.refract - 1) * 33, 371, 8))
		set_drag(trace, x, 9);//refract
	else if (in_circle(x, y, 173 + get_bump_level(on), 461, 8))
		set_drag(trace, x, 10);//bump level
	else
		return ;
}

void	reset_window(t_trace *trace)
{	
	t_control *con = trace->obj_control;	

	if (!con->pos_open)
	{
		con->pos_open = false;
		con->on_dials = false;
		con->rot_open = false;
		con->sca_open = false;
		trace->on_menu = true;
	}
	set_controls(trace);
}

void	buttons1(t_trace *trace, t_on *on, void (*action)(t_trace *trace, t_on *on))
{
	action(trace, on);
	reset_window(trace);
}

void	create_new_light(t_trace *trace, t_light **lt, bool (*create_lt)(t_trace *trace, t_light **lt))
{
	if (create_lt(trace, lt))
		clear_all(trace);
	reset_window(trace);
	if (create_lt == &insert_ltcopy_after2)
		update(trace->mlx_connect, trace->mlx_win, trace);
	else
		update_no_low(trace->mlx_connect, trace->mlx_win, trace);
}

void	prelim_buttons(int x, int y, t_trace *trace)
{
	t_type type = trace->on->type;

	if((y > 41 && y <= 64) && (x >= 52 && x <= 140))
		buttons1(trace, trace->on, prev_list);
	else if((y > 41 && y <= 64) && (x >= 257 && x <= 345))
		buttons1(trace, trace->on, next_list);
	if (type == CAM)
		return ;
	else if (type == LIGHT)
	{
		if ((y > 510 && y <= 534) && (x >= 203 && x <= 307))
		{
			buttons1(trace, trace->on, push_new_object);
			update_no_low(trace->mlx_connect, trace->mlx_win, trace);
		}
	}
	else if ((y > 510 && y <= 534) && (x >= 258 && x <= 362))
	{
		buttons1(trace, trace->on, push_new_object);
		update_no_low(trace->mlx_connect, trace->mlx_win, trace);
	}
	else if ((y > 510 && y <= 534) && (x >= 148 && x <= 252) && type != LIGHT)
	{
		buttons1(trace, trace->on, push_new_object2);
		update_no_low(trace->mlx_connect, trace->mlx_win, trace);
	}
}

void	prev_next_pop_obj(int x, int y, t_trace *trace)
{
	if ((y > 540 && y <= 564) && (x >= 93 && x <= 197))
		buttons1(trace, trace->on, prev_list_ob);
	else if ((y > 540 && y <= 564) && (x >= 203 && x <= 307))
		buttons1(trace, trace->on, next_list_ob);
	else if ((y > 510 && y <= 534) && (x >= 93 && x <= 197) && trace->on->type == LIGHT)
	{
		buttons1(trace, trace->on, pop_object);
		update_no_low(trace->mlx_connect, trace->mlx_win, trace);
	}
	else if ((y > 510 && y <= 534) && (x >= 38 && x <= 142))
	{
		buttons1(trace, trace->on, pop_object);
		update_no_low(trace->mlx_connect, trace->mlx_win, trace);
	}
}

void	open_dials(t_trace *trace, bool *open_dial, void (*set_dial)(t_trace *))
{
	*open_dial = true;
	set_dial(trace);
	mlx_put_image_to_window(trace->mlx_connect, trace->mlx_win, trace->img.img_ptr, 0, 0);
}

int lt_press(int x, int y, t_trace *trace)
{
	t_norm_color	obj_col;
	t_on			*on = trace->on;
	t_light			*lt;

	obj_col = get_obj_color2(on);
	trace->on_menu = true;
	prelim_buttons(x, y, trace);
	//create each type
	if ((y > 241 && y <= 265) && (x >= 38 && x <= 142))
		create_new_light(trace, &trace->curr_lt, insert_ltpoint);
	else if((y > 241 && y <= 265) && (x >= 148 && x <= 252))
		create_new_light(trace, &trace->curr_lt, insert_ltcopy_after3);
	else if((y > 241 && y <= 265) && (x >= 258 && x <= 362))
		create_new_light(trace, &trace->curr_lt, insert_ltcopy_after2);
	if (trace->on->object == NULL)
		return (0);
	lt = (t_light *)(on->object);
	if ((y > 480 && y <= 504) && (x >= 38 && x <= 142) && lt->type != POINT)
		open_dials(trace, &trace->obj_control->rot_open, set_rot_dials);
	else if ((y > 480 && y <= 504) && (x >= 258 && x <= 362))
		open_dials(trace, &trace->obj_control->pos_open, set_pos_dials);
	else if (in_circle(x, y, 23 + (int)obj_col.r, 112, 8))
		set_drag(trace, x, 0);
	else if (in_circle(x, y, 23 + (int)obj_col.g, 137, 8))
		set_drag(trace, x, 1);
	else if (in_circle(x, y, 23 + (int)obj_col.b, 162, 8))
		set_drag(trace, x, 2);
	else if (in_circle(x, y, 173 + lt->brightness * 100, 191 , 8))
		set_drag(trace, x, 17);
	prev_next_pop_obj(x, y, trace);
	if (lt->type == SPOT)
	{
		if (in_circle(x, y, 174 + ft_round(lt->in_angle * 10 / 9), 307 , 8))
			set_drag(trace, x, 18);
		if (in_circle(x, y, 174 + ft_round(lt->out_angle * 10 / 9), 332 , 8))
			set_drag(trace, x, 19);
	}
	else if (lt->type == AREA)
	{
		double wid = 1 / (lt->emitter->curr_scale.mat[0] * lt->emitter->h_width);
		double len = 1 / (lt->emitter->curr_scale.mat[10] * lt->emitter->h_depth);
		if (in_circle(x, y, 174 + ft_round(wid * 10), 387 , 8))
			set_drag(trace, x, 20);
		else if (in_circle(x, y, 174 + ft_round(len * 10), 412 , 8))
			set_drag(trace, x, 21);
		else if (in_circle(x, y, 174 + lt->usteps * 4, 437 , 8))
			set_drag(trace, x, 22);
		else if (in_circle(x, y, 174 + lt->vsteps * 4, 462 , 8))
			set_drag(trace, x, 23);
	}
	return (0);
}

void	next_preset_material(t_trace *trace)
{
	int			j;
	t_img		*img = &trace->img;
	t_control	control = *trace->obj_control;
	t_mat		curr;
	t_mat		new;

	curr = get_obj_mat(trace->on);
	new = get_mat(curr.preset + 1);
	change_mat(trace, trace->on, new);
	j = -1;
	while (++j < 7)
		reset_ptrack(img, control, 162, 213 + j * 25);
	set_pknobs1(img, control, trace->on);
	update_no_low(trace->mlx_connect, trace->mlx_win, trace);
}

//testing idea for button press effect
/* void	press_button(t_trace *trace, int x, int y, int width, int height)
{
	unsigned int curr_col;
	int i;//, k;
	width += 1;
	height += 1;
	
	i = width;
	while (--height > -1)
	{
		width = i;
		while (--width > -1)
		{
			curr_col = pixel_color_get3(x + width, y + height, trace->obj_control->menu);
			my_pixel_put(x + width + 1, y + height + 1, &trace->img, curr_col);
		}
		my_pixel_put(x + width, y + height, &trace->img, 0x202020);
	}
	trace->obj_control->pressed = true;
} */

//handles presses upon the menu when open for object manipulation

int menu_press(int x, int y, t_trace *trace)
{
	trace->on_menu = true;

	prelim_buttons(x, y, trace);
	if (trace->on->object == NULL)
		return (0);
	else if ((y > 480 && y <= 504) && (x >= 38 && x <= 142))
		open_dials(trace, &trace->obj_control->rot_open, set_rot_dials);
	else if ((y > 480 && y <= 504) && (x >= 258 && x <= 362))
		open_dials(trace, &trace->obj_control->pos_open, set_pos_dials);
	if (trace->on->type == CAM)
	{
		if (in_circle(x, y, 111 + ((t_cam *)(trace->on->object))->fov, 82, 8))
			set_drag(trace, x, 32);
		return (0);
	}
	else if ((x >= 172 && x <= 274) && (y > 390 && y <= 413)) 
	{
		toggle_shadow(trace, trace->on);
		update_no_low(trace->mlx_connect, trace->mlx_win, trace);
	}
	else if ((x >= 172 && x <= 274) && (y > 420 && y <= 443)) 
	{
		toggle_bump(trace, trace->on);
		update_no_low(trace->mlx_connect, trace->mlx_win, trace);
	}
	else if((y > 480 && y <= 504) && (x >= 148 && x <= 252))
		open_dials(trace, &trace->obj_control->sca_open, set_sca_dials);
	prev_next_pop_obj(x, y, trace);
	if ((y > 180 && y <= 203) && (x >= 172 && x <= 274))
		next_preset_material(trace);
	else if ((y > 70 && y <= 93) && (x >= 21 && x <= 99))//color
	{
		set_option(trace, trace->on, 0);
		/* press_button(trace, 21, 71, 79, 23);//to start button press effect
		mlx_put_image_to_window(trace->mlx_connect, trace->mlx_win, trace->img.img_ptr, 0, 0);
		set_menu_vals(trace, trace->on); */
		update_no_low(trace->mlx_connect, trace->mlx_win, trace);
	}
	else if ((y > 70 && y <= 93) && (x >= 104 && x <= 189))
	{
		if (get_option(trace->on) == 1)
			set_next_tx(4, trace->textures, trace->on);
		else
			set_option(trace, trace->on, 1);
		update_no_low(trace->mlx_connect, trace->mlx_win, trace);
	}
	else if ((y > 70 && y <= 93) && (x >= 194 && x <= 297))
	{
		set_option(trace, trace->on, 2);
		update_no_low(trace->mlx_connect, trace->mlx_win, trace);
	}
	else
		check_knobs(x, y, trace);
	return (0);
}

//close controls menu

void	close_con(t_trace *trace)
{
	t_control	*con;

	con = trace->obj_control;
	trace->on_menu = false;
	con->on_dials = false;
	con->rot_open = false;
	con->pos_open = false;
	con->sca_open = false;
	trace->low_res = false;
}

void	set_windows(t_trace *trace, bool *win1, bool *win2)
{
	*win1 = false;
	*win2 = false;
	set_controls(trace);
}

void	off_dials_click(t_trace *trace, t_control control, t_control *con)
{
	if (control.rot_open && !con->pos_open)
		set_windows(trace, &con->rot_open, &con->sca_open);
	else if (control.pos_open && !con->rot_open)
		set_windows(trace, &con->pos_open, &con->sca_open);
	else if (control.pos_open && control.rot_open)
		set_windows(trace, &con->rot_open, &con->pos_open);
	else if (control.sca_open)
	{
		trace->obj_control->sca_open = false;
		set_windows(trace, &con->rot_open, &con->pos_open);
	}
}

void	manage_windows(t_trace *trace, t_control control)
{
		t_control	*con = trace->obj_control;	

		if (!con->on_dials)
			off_dials_click(trace, control, con);
		mlx_put_image_to_window(trace->mlx_connect,
		trace->mlx_win, trace->img.img_ptr, 0, 0);
		if (!con->pos_open && !con->sca_open)
			set_menu_vals(trace, trace->on);
		else
		{
			if (!con->sca_open)
				set_pos_vals(trace->mlx_connect, trace->mlx_win, trace);
			if (trace->on->type == LIGHT)
				set_lttype(trace->mlx_connect, trace->mlx_win, trace->curr_lt);
			else	
				set_type(trace->mlx_connect, trace->mlx_win, trace->on->type);
		}
		if (con->rot_open)
			set_rot_vals(trace->mlx_connect, trace->mlx_win, trace);
		else if (con->sca_open)
			set_sca_vals(trace->mlx_connect, trace->mlx_win, trace);
}

//handle lclick on open control panel

void	click_on_menu(int x, int y, t_trace *trace, t_control control)
{
	if (control.rot_open && x < control.d_width && y > control.dials_ys && y < control.m_height)
	{
		trace->obj_control->on_dials = true;
		rotation_press(x, y, trace, control);
	}
	else if (control.pos_open && x > control.d_width && y > control.dials_ys && y < control.m_height)
	{
		trace->obj_control->on_dials = true;
		position_press(x, y, trace, control);
	}
	else if (control.sca_open && y > control.sdials_ys && y < control.m_height)
	{
		trace->obj_control->on_dials = true;
		scale_press(x, y, trace, control);
	}
	else
	{
		trace->obj_control->on_dials = false;
		if (trace->on->type == LIGHT)
			lt_press(x, y, trace);
		else
			menu_press(x, y, trace);
	}
}

//left click

void	handle_left_click(int x, int y, t_trace *trace, t_control con)
{
	if (trace->dragging && trace->low_res)
		trace->low_flag = true;
	if (trace->menu_open && x < con.m_width && y < con.m_height)
		click_on_menu(x, y, trace, con);
	else
	{
		if (trace->menu_open && trace->dragging)//
			;
		else if (trace->on_menu)//or menu open, then close
			close_con(trace);
		track_object(trace, x, y);
	}
}

//right click, wheel back/forward

void	handle_other_click(int button, t_trace *trace, t_on *on)
{
	t_norm_color	curr_col;
	int				option;
	
	option = get_option(on);
	if (button == 3)
	{
		select_option(trace, on);
		reset_con(trace);
	}
	else if (button == 4 || button == 5)//wheel
	{
		if (option == 0)
			rotate_colors(trace, button, &curr_col);
		else if (option == 1)
			set_next_tx(button, trace->textures, on);
		reset_con(trace);
	}
}

//handle mouse hooks

int	mouse_press(int button, int x, int y, t_trace *trace)
{
	t_control 		control;
	t_on			*on;

	control = *trace->obj_control;
	on = trace->on;
	if (button == 1)
		handle_left_click(x, y, trace, control);
	else
		handle_other_click(button, trace, on);
	if (trace->menu_open && !trace->on_menu)//closes when click off menu
		trace->menu_open = false;
	if (trace->on_menu && trace->menu_open)
		manage_windows(trace, control);
	return (0);
}
