/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:56:43 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 13:56:46 by jhotchki         ###   ########.fr       */
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
	trace->cam->true_up
		= norm_vec(mat_vec_mult(trace->cam->transform_up, g_up));
	reinit_viewing(trace);
}

//continued below ft
	//for spotlight rotation should work

static inline void	rotate_object2(t_trace *trace, t_on *on, t_matrix_4x4 rot)
{
	if (on->type == CYLINDER)
	{
		trace->curr_cy->curr_rottran = \
			mat_mult(rot, trace->curr_cy->curr_rottran);
		trace->curr_cy->transform = \
			mat_mult(trace->curr_cy->curr_scale, trace->curr_cy->curr_rottran);
	}
	else if (on->type == CAM)
		rotate_cam(trace, rot);
	else
		return ;
}

// rotates current "on" object

void	rotate_object(t_trace *trace, t_on *on, t_matrix_4x4 rot)
{
	t_vec3	up;

	up = vec(0, 1, 0, 0);
	if (on->object == NULL)
		return ;
	else if (on->type == SPHERE)
	{
		trace->curr_sp->curr_rottran = \
			mat_mult(rot, trace->curr_sp->curr_rottran);
		trace->curr_sp->transform = \
			mat_mult(trace->curr_sp->curr_scale, trace->curr_sp->curr_rottran);
	}
	else if (on->type == PLANE)
	{
		trace->curr_pl->curr_rottran = \
			mat_mult(rot, trace->curr_pl->curr_rottran);
		trace->curr_pl->transform = \
			mat_mult(trace->curr_pl->curr_scale, trace->curr_pl->curr_rottran);
		trace->curr_pl->norm = \
			norm_vec(mat_vec_mult(transpose(trace->curr_pl->transform), up));
	}
	else
		rotate_object2(trace, on, rot);
}
