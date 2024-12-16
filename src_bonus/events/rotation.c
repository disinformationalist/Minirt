/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:17:56 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 14:17:58 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//cam rotates in local space for best use

static inline void	rotate_cam(t_trace *trace, t_matrix_4x4 rot)
{
	t_vec3	g_up;
	t_vec3	f;

	f = vec(0.0, 0.0, 1.0, 0.0);
	g_up = vec(0.0, 1.0, 0.0, 0.0);
	trace->cam->transform = mat_mult(trace->cam->transform, rot);
	trace->cam->transform_up = mat_mult(trace->cam->transform_up, rot);
	trace->cam->orient = norm_vec(mat_vec_mult(trace->cam->transform, f));
	trace->cam->true_up = \
		norm_vec(mat_vec_mult(trace->cam->transform_up, g_up));
	reinit_viewing(trace);
}

//rotates an area light, the light, not its emitter, lt uses forward transforms

void	rot_arealt(t_light *lt, t_matrix_4x4 rot)
{
	lt->curr_rottran = mat_mult(inverse(rot), lt->curr_rottran);
	lt->transform = mat_mult(lt->curr_scale, lt->curr_rottran);
	lt->emitter->curr_rottran = mat_mult(rot, lt->emitter->curr_rottran);
	lt->emitter->transform = \
		mat_mult(lt->emitter->curr_scale, lt->emitter->curr_rottran);
	set_arealt(lt);
}

static inline void	rotate_object3(t_trace *trace, t_on *on, t_matrix_4x4 rot)
{
	if (on->type == CUBE)
	{
		trace->curr_cu->curr_rottran = \
			mat_mult(rot, trace->curr_cu->curr_rottran);
		trace->curr_cu->transform = \
			mat_mult(trace->curr_cu->curr_scale, trace->curr_cu->curr_rottran);
		trace->curr_cu->t_transform = transpose(trace->curr_cu->transform);
		trace->curr_cu->i_transform = inverse(trace->curr_cu->transform);
	}
	else if (on->type == SPHERE)
	{
		trace->curr_sp->curr_rottran = \
			mat_mult(rot, trace->curr_sp->curr_rottran);
		trace->curr_sp->transform = \
			mat_mult(trace->curr_sp->curr_scale, trace->curr_sp->curr_rottran);
		trace->curr_sp->t_transform = transpose(trace->curr_sp->transform);
		trace->curr_sp->i_transform = inverse(trace->curr_sp->transform);
	}
	else if (on->type == CAM)
		rotate_cam(trace, rot);
	else
		return ;
}

//continued below ft

static inline void	rotate_object2(t_trace *trace, t_on *on, t_matrix_4x4 rot)
{
	if (on->type == CYLINDER)
	{
		trace->curr_cy->curr_rottran = \
			mat_mult(rot, trace->curr_cy->curr_rottran);
		trace->curr_cy->transform = \
			mat_mult(trace->curr_cy->curr_scale, trace->curr_cy->curr_rottran);
		trace->curr_cy->t_transform = transpose(trace->curr_cy->transform);
		trace->curr_cy->i_transform = inverse(trace->curr_cy->transform);
	}
	else if (on->type == HYPERBOLOID)
	{
		trace->curr_hy->curr_rottran = \
			mat_mult(rot, trace->curr_hy->curr_rottran);
		trace->curr_hy->transform = \
			mat_mult(trace->curr_hy->curr_scale, trace->curr_hy->curr_rottran);
		trace->curr_hy->t_transform = transpose(trace->curr_hy->transform);
		trace->curr_hy->i_transform = inverse(trace->curr_hy->transform);
	}
	else
		rotate_object3(trace, on, rot);
}

// rotates current "on" object

void	rotate_object(t_trace *trace, t_on *on, t_matrix_4x4 rot)
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
	}
	else if (on->type == LIGHT)
	{
		if (trace->curr_lt->type == SPOT)
			trace->curr_lt->dir = mat_vec_mult(rot, trace->curr_lt->dir);
		else if (trace->curr_lt->type == AREA)
			rot_arealt(trace->curr_lt, rot);
	}
	else
		rotate_object2(trace, on, rot);
}
