/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_ops3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:27:54 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 14:27:56 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec3	scale_vec(double scalar, t_vec3 vec)
{
	t_vec3	res;

	res.x = scalar * vec.x;
	res.y = scalar * vec.y;
	res.z = scalar * vec.z;
	res.w = 0;
	return (res);
}

t_vec3	div_vec(double scalar, t_vec3 vec)
{
	t_vec3	res;

	res.x = vec.x / scalar;
	res.y = vec.y / scalar;
	res.z = vec.z / scalar;
	res.w = 0;
	return (res);
}

t_vec3	neg(t_vec3 vec)
{
	t_vec3	neg;

	neg.x = -vec.x;
	neg.y = -vec.y;
	neg.z = -vec.z;
	neg.w = 0;
	return (neg);
}

t_vec3	mult_vec(t_vec3 v1, t_vec3 v2)
{
	t_vec3	res;

	res.x = v1.x * v2.x;
	res.y = v1.y * v2.y;
	res.z = v1.z * v2.z;
	res.w = 0;
	return (res);
}
