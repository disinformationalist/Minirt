/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_spheres2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:09:34 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 14:09:36 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline bool	check_solutions(t_vec3 abc, double *t1, double *t2)
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

//using object space

bool	ray_sphere_intersect(t_sphere sphere, t_ray ray, double *t)
{
	double	t1;
	double	t2;
	double	a;
	double	b;
	double	c;

	ray = transform(ray, sphere.transform);
	a = dot_product(ray.dir, ray.dir);
	b = 2.0 * dot_product(ray.origin, ray.dir);
	c = dot_product(ray.origin, ray.origin) - 1;
	if (check_solutions(vec(a, b, c, 0), &t1, &t2))
	{
		if (t1 > 0 && t1 < *t)
			*t = t1;
		if (t2 > 0 && t2 < *t)
			*t = t2;
	}
	if (*t < INFINITY)
		return (true);
	return (false);
}

void	check_spheres(t_sphere *spheres, t_track_hits *closest, t_ray ray,
		double *t)
{
	t_sphere	*curr_sp;

	if (spheres == NULL)
		return ;
	curr_sp = spheres;
	while (true)
	{
		if (ray_sphere_intersect(*curr_sp, ray, t))
		{
			if (*t < closest->t && *t > 0)
			{
				closest->t = *t;
				closest->object = curr_sp;
				closest->object_type = SPHERE;
			}
		}
		curr_sp = curr_sp->next;
		if (curr_sp == spheres)
			break ;
	}
}
