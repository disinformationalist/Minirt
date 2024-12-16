/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:06:47 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 14:06:49 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

uint8_t	clamp_color(double color)
{
	if (color > 255)
		return (255);
	else
		return ((uint8_t)color);
}

t_norm_color	sum_rgbs(t_norm_color sum, t_norm_color to_add)
{
	sum.r += to_add.r;
	sum.g += to_add.g;
	sum.b += to_add.b;
	return (sum);
}

//averages samples components, packs and returns a pixel color

unsigned int	avg_samples(t_norm_color sum, double n)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	r = clamp_color(sum.r / n);
	g = clamp_color(sum.g / n);
	b = clamp_color(sum.b / n);
	return (r << 16 | g << 8 | b);
}

t_norm_color	get_final_color(t_trace *trace, t_norm_color color,
			double light_int)
{
	t_norm_color	color_out;

	color_out.r = color.r * (light_int + trace->amb->color.r);
	color_out.g = color.g * (light_int + trace->amb->color.g);
	color_out.b = color.b * (light_int + trace->amb->color.b);
	return (color_out);
}
