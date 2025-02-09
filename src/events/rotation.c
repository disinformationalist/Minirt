#include "minirt.h"

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

void 	set_rotknob(t_trace *trace, t_position knobrot, t_control control, double yshift)
{
	int				i;
	int				j;
	int				putxx;
	int				putxy;
	unsigned int	color;

	putxx = knobrot.i + 92;
	putxy = -knobrot.j + yshift;
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

static inline t_vec3 set_rots(t_vec3 rots, t_vec3 add)
{
	t_vec3 res;

	res = add_vec(rots, add);
	res.x = fmod(res.x, TWO_PI);
	res.y = fmod(res.y, TWO_PI);
	res.z = fmod(res.z, TWO_PI);
	return (res);
}

//cam rotates in local space for best use

static inline void	rotate_cam(t_trace *trace, t_mat4 rot, t_vec3 add)
{
	t_vec3	g_up;
	t_vec3	f;

	f = vec(0.0, 0.0, 1.0, 0.0);
	g_up = vec(0.0, 1.0, 0.0, 0.0);
	trace->cam->transform = mat_mult(rot, trace->cam->transform);
	trace->cam->transform_up = mat_mult(rot, trace->cam->transform_up);
	trace->cam->orient = norm_vec(mat_vec_mult(trace->cam->transform, f));
	trace->cam->true_up = \
		norm_vec(mat_vec_mult(trace->cam->transform_up, g_up));
	trace->cam->rottran = \
			mat_mult(rot, trace->cam->rottran);
	trace->cam->rots = set_rots(trace->cam->rots, add);
	reinit_viewing(trace);
}

//rotates an area light, the light, not its emitter, lt uses forward transforms

void	rot_arealt(t_light *lt, t_mat4 rot, t_vec3 add)
{
	lt->curr_rottran = mat_mult(inverse(rot), lt->curr_rottran);
	lt->rots = set_rots(lt->rots, add);
	lt->transform = mat_mult(lt->curr_scale, lt->curr_rottran);
	lt->emitter->curr_rottran = mat_mult(rot, lt->emitter->curr_rottran);
	lt->emitter->transform = \
		mat_mult(lt->emitter->curr_scale, lt->emitter->curr_rottran);
	lt->emitter->rots = set_rots(lt->emitter->rots, add);
	set_arealt(lt);
}

static inline void	rotate_object3(t_trace *trace, t_on *on, t_mat4 rot, t_vec3 add)
{
	if (on->type == CUBE)
	{
		trace->curr_cu->curr_rottran = \
			mat_mult(rot, trace->curr_cu->curr_rottran);
		trace->curr_cu->transform = \
			mat_mult(trace->curr_cu->curr_scale, trace->curr_cu->curr_rottran);
		trace->curr_cu->t_transform = transpose(trace->curr_cu->transform);
		trace->curr_cu->i_transform = inverse(trace->curr_cu->transform);
		trace->curr_cu->rots = set_rots(trace->curr_cu->rots, add);
	}
	else if (on->type == SPHERE)
	{
		trace->curr_sp->curr_rottran = \
			mat_mult(rot, trace->curr_sp->curr_rottran);
		trace->curr_sp->transform = \
			mat_mult(trace->curr_sp->curr_scale, trace->curr_sp->curr_rottran);
		trace->curr_sp->t_transform = transpose(trace->curr_sp->transform);
		trace->curr_sp->i_transform = inverse(trace->curr_sp->transform);
		trace->curr_sp->rots = set_rots(trace->curr_sp->rots, add);
	}
	else if (on->type == CAM)
		rotate_cam(trace, rot, add);
	else
		return ;
}

//continued below ft

static inline void	rotate_object2(t_trace *trace, t_on *on, t_mat4 rot, t_vec3 add)
{
	if (on->type == CYLINDER)
	{
		trace->curr_cy->curr_rottran = \
			mat_mult(rot, trace->curr_cy->curr_rottran);
		trace->curr_cy->transform = \
			mat_mult(trace->curr_cy->curr_scale, trace->curr_cy->curr_rottran);
		trace->curr_cy->t_transform = transpose(trace->curr_cy->transform);
		trace->curr_cy->i_transform = inverse(trace->curr_cy->transform);
		trace->curr_cy->rots = set_rots(trace->curr_cy->rots, add);
	}
	else if (on->type == HYPERBOLOID)
	{
		trace->curr_hy->curr_rottran = \
			mat_mult(rot, trace->curr_hy->curr_rottran);
		trace->curr_hy->transform = \
			mat_mult(trace->curr_hy->curr_scale, trace->curr_hy->curr_rottran);
		trace->curr_hy->t_transform = transpose(trace->curr_hy->transform);
		trace->curr_hy->i_transform = inverse(trace->curr_hy->transform);
		trace->curr_hy->rots = set_rots(trace->curr_hy->rots, add);
	}
	else
		rotate_object3(trace, on, rot, add);
}

// rotates current "on" object

void	rotate_object(t_trace *trace, t_on *on, t_mat4 rot, t_vec3 add, bool flag)
{
	t_vec3	up;

	up = vec(0, 1, 0, 0);
	if (on->object == NULL)
		return ;
	else if (on->type == PLANE)
	{
		trace->curr_pl->curr_rottran = \
			mat_mult(rot, trace->curr_pl->curr_rottran);
		trace->curr_pl->transform = \
			mat_mult(trace->curr_pl->curr_scale, trace->curr_pl->curr_rottran);
		trace->curr_pl->t_transform = transpose(trace->curr_pl->transform);
		trace->curr_pl->i_transform = inverse(trace->curr_pl->transform);
		trace->curr_pl->norm = \
			norm_vec(mat_vec_mult(trace->curr_pl->t_transform, up));
		trace->curr_pl->rots = set_rots(trace->curr_pl->rots, add);
	}
	else if (on->type == LIGHT)
	{
		if (trace->curr_lt->type == SPOT)
		{
			trace->curr_lt->dir = mat_vec_mult(rot, trace->curr_lt->dir);
			trace->curr_lt->curr_rottran = \
			mat_mult(rot, trace->curr_lt->curr_rottran);
			trace->curr_lt->rots = set_rots(trace->curr_lt->rots, add);
		}
		else if (trace->curr_lt->type == AREA)
			rot_arealt(trace->curr_lt, rot, add);
	}
	else
		rotate_object2(trace, on, rot, add);
	if (trace->obj_control->rot_open && !flag)
	{
		t_control cont = *trace->obj_control;
		void			*con = trace->mlx_connect;
		void			*win = trace->mlx_win;
		set_rotpos(trace);
		reset_rottracks(&trace->img, cont);
		set_rotknobs(trace, *trace->obj_control);
		update(con, win, trace);
		set_con_vals(con, win, trace);
	}
}
//one at a time version
/* 	if (add.x)
		{
			set_rotposx(trace);
			reset_prtrack(&trace->img, cont.dtx, 30, 50);
			set_rotknob(trace, cont.rotsx, cont, 182);
		}
		else if (add.y)
		{
			set_rotposy(trace);
			reset_prtrack(&trace->img, cont.dty, 30, 195);
			set_rotknob(trace, cont.rotsy, cont, 327);
		}
		else
		{
			set_rotposz(trace);
			reset_prtrack(&trace->img, cont.dtz, 30, 340);
			set_rotknob(trace, cont.rotsz, cont, 472);
		} */