/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_tuple_arythm.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 18:15:35 by arybarsk          #+#    #+#             */
/*   Updated: 2024/07/28 18:15:37 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_tuple	add(t_tuple a, t_tuple b)
{
	t_tuple	c;

	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	c.w = a.w + b.w;
	return (c);
}

t_tuple	substract(t_tuple a, t_tuple b)
{
	t_tuple	c;

	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	c.w = a.w - b.w;
	return (c);
}

t_tuple	negate(t_tuple a)
{
	t_tuple	b;

	b.x = 0 - a.x;
	b.y = 0 - a.y;
	b.z = 0 - a.z;
	b.w = 0 - a.w;
	return (b);
}

t_tuple	multiply(t_tuple a, double n)
{
	t_tuple	b;

	b.x = a.x * n;
	b.y = a.y * n;
	b.z = a.z * n;
	b.w = a.w * n;
	return (b);
}

t_tuple	divide(t_tuple a, double n)
{
	t_tuple	b;

	b.x = a.x / n;
	b.y = a.y / n;
	b.z = a.z / n;
	b.w = a.w / n;
	return (b);
}
