#ifndef SCALE_H
# define SCALE_H

/****Funtions****/
/*
void			set_sca_vals(void *mlx_con, void *win, t_trace *trace);
t_vec3			get_scale(t_on *on, t_trace *trace);
t_vec3			get_init_scale(t_on *on, t_trace *trace);

 */


static inline double convert_to_angle(double val, double adjust)
{
	if (val >= 1)
		val = (val - 1) / 10.01 + adjust;
	else
		val = adjust - (1 - val) / 1.111;
	return (val);
}

static inline t_mat4	get_curr_scale(t_type type, t_trace *trace)
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

static inline t_vec3	get_init_scale(t_on *on, t_trace *trace)
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

static inline t_vec3	get_scale(t_on *on, t_trace *trace)
{
	t_mat4			scale;
	t_vec3			temp;

	scale = get_curr_scale(on->type, trace);
	temp = vec(scale.mat[0], scale.mat[5], scale.mat[10], 0);
	check_tolerance(&temp);
	return (temp);
}

static inline void	set_sca_vals(void *mlx_con, void *win, t_trace *trace)
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
	sprintf(val, "%.2f", pos.x);
	mlx_string_put(mlx_con, win, 86, shifty, color, val);
	//x
	sprintf(val, "%.2f", pos.y);
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

#endif

