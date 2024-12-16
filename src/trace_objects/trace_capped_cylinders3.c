/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_capped_cylinders3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:10:00 by arybarsk          #+#    #+#             */
/*   Updated: 2024/12/16 13:10:05 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	check_trunk_solutions(t_vec3 abc, double *t1, double *t2)
{
	double	discrim;
	double	inv_2a;
	double	sq_discrim;

	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-5)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	*t1 = (-abc.y - sq_discrim) * inv_2a;
	*t2 = (-abc.y + sq_discrim) * inv_2a;
	return (true);
}

void	compute_abc(t_vec3 *abc, t_ray ray)
{
	abc->x = ray.dir.x * ray.dir.x + ray.dir.z * ray.dir.z;
	abc->y = 2 * (ray.origin.x * ray.dir.x + ray.origin.z * ray.dir.z);
	abc->z = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - 1;
}
