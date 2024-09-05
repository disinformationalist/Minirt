/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_tuple_arythm_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 20:19:45 by arybarsk          #+#    #+#             */
/*   Updated: 2024/07/28 20:19:47 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//Normalization converts a vector into a unit vector.
//Magnitude means the vector's length.
t_tuple	normalize(t_tuple a)
{
	double	magnitude;
	t_tuple b;

	magnitude = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2) + pow(a.w, 2));
	b.x = a.x / magnitude;
	b.y = a.y / magnitude;
	b.z = a.z / magnitude;
	b.w = a.w / magnitude;
	return (b);
}

//Produces a vector that is perpendicular
//to both of the original vectors.
t_tuple	cross(t_tuple a, t_tuple b)
{
	t_tuple c;

	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
	return (c);
}

//Takes two vectors and returns a scalar value.
double	dot(t_tuple a, t_tuple b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}
