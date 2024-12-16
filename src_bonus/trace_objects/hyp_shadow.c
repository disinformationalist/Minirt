/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hyp_shadow.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:58:04 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 14:58:05 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline bool	within_height2(t_ray ray, double t)
{
	double	y;

	if (t < 1e-5)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	if (y > -1 && y < 1)
		return (true);
	return (false);
}

static inline bool	check_trunk_solutions2_hy(t_vec3 abc, \
t_ray ray, double dist)
{
	double	discrim;
	double	inv_2a;
	double	sq_discrim;
	double	t;

	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-5)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	t = (-abc.y - sq_discrim) * inv_2a;
	if (within_height2(ray, t) && t < dist)
		return (true);
	t = (-abc.y + sq_discrim) * inv_2a;
	if (within_height2(ray, t) && t < dist)
		return (true);
	return (false);
}

static inline void	compute_abc_hy2(t_vec3 *abc, t_ray ray, \
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

static inline bool	ray_hyperboloid_intersect2(t_hyperboloid hyperboloid, \
t_ray ray, double dist)
{
	t_vec3	abc;

	ray = transform(ray, hyperboloid.transform);
	compute_abc_hy2(&abc, ray, &hyperboloid);
	if (abc.x == 0)
		return (false);
	if (check_trunk_solutions2_hy(abc, ray, dist))
		return (true);
	if (intersect_caps2_hy(ray, dist, hyperboloid))
		return (true);
	return (false);
}

bool	check_hy_dist(t_hyperboloid *hyperboloids, \
t_ray ray, double dist)
{
	t_hyperboloid	*curr_hy;

	if (hyperboloids == NULL)
		return (false);
	curr_hy = hyperboloids;
	while (true)
	{
		if (curr_hy->shadow && ray_hyperboloid_intersect2(*curr_hy, ray, dist))
			return (true);
		curr_hy = curr_hy->next;
		if (curr_hy == hyperboloids)
			break ;
	}
	return (false);
}
