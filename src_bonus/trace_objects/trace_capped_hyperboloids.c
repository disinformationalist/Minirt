/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_capped_hyperboloids.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:57:08 by arybarsk          #+#    #+#             */
/*   Updated: 2024/12/16 12:57:12 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline bool	check_trunk_solutions(t_vec3 abc, double *t1, double *t2)
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

// Equation: x * x / a * a + z * z / b * b - y * y / c * c = 1
// will need to parse a hyperboloid center, height, half_h, ray_max and ray_min

static inline void	compute_abc_hy(t_vec3 *abc, t_ray ray, \
t_hyperboloid *hyperboloid)
{
	abc->x = (ray.dir.x * ray.dir.x)
		+ (ray.dir.z * ray.dir.z)
		- (ray.dir.y * ray.dir.y);
	abc->y = 2 * ((ray.origin.x * ray.dir.x)
			+ (ray.origin.z * ray.dir.z)
			- (ray.origin.y * ray.dir.y));
	abc->z = (ray.origin.x * ray.origin.x)
		+ (ray.origin.z * ray.origin.z)
		- (ray.origin.y * ray.origin.y) - (hyperboloid->waist_val);
}

bool	within_height_hy(t_ray ray, double t)
{
	double	y;

	if (t < 1e-5)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	if (y > -1 && y < 1)
		return (true);
	return (false);
}

void	ray_hyperboloid_intersect(t_hyperboloid *hyperboloids, t_ray ray,
		t_intersects *intersects)
{
	t_vec3	abc;
	double	t1;
	double	t2;
	double	t3;
	double	t4;

	ray = transform(ray, hyperboloids->transform);
	compute_abc_hy(&abc, ray, hyperboloids);
	if (abc.x != 0)
	{
		if (check_trunk_solutions(abc, &t1, &t2))
		{
			if (within_height_hy(ray, t1))
				intersect(intersects, hyperboloids, t1, HYPERBOLOID);
			if (within_height_hy(ray, t2))
				intersect(intersects, hyperboloids, t2, HYPERBOLOID);
		}
	}
	if (intersect_caps_hy(ray, &t3, &t4, hyperboloids))
	{
		if (t3 < INFINITY)
			intersect(intersects, hyperboloids, t3, HYPERBOLOID);
		if (t4 < INFINITY)
			intersect(intersects, hyperboloids, t4, HYPERBOLOID);
	}
}

void	check_hyperboloids(t_hyperboloid *hyperboloids,
		t_intersects *intersects, t_ray ray)
{
	t_hyperboloid	*curr_hy;

	if (hyperboloids == NULL)
		return ;
	curr_hy = hyperboloids;
	while (true)
	{
		ray_hyperboloid_intersect(curr_hy, ray, intersects);
		curr_hy = curr_hy->next;
		if (curr_hy == hyperboloids)
			break ;
	}
}
