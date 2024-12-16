/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supersample.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:18:09 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 14:18:11 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//super on/off

int	supersample_handle(int keycode, t_trace *trace)
{
	if (keycode == SPACE)
	{
		if (trace->supersample)
			reinit_viewing(trace);
		else
			reinit_viewing(trace);
		trace->supersample = !trace->supersample;
	}
	return (0);
}

//increase/decrease supersample level

void	adjust_super(int keycode, t_trace *trace)
{
	if (keycode == PAD_PLUS)
	{
		trace->n += 1.0;
		if (trace->n > 10.0)
			trace->n = 10.0;
	}
	if (keycode == PAD_MINUS)
	{
		trace->n -= 1.0;
		if (trace->n < 2.0)
			trace->n = 2.0;
	}
	reinit_viewing(trace);
}
