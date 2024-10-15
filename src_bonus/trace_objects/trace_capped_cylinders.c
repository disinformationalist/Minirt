/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_cylinders.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 18:11:21 by arybarsk          #+#    #+#             */
/*   Updated: 2024/09/02 18:11:23 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void ft_swap(double *a, double *b)
{
	double temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static inline bool	check_trunk_solutions(t_vec3 abc, double *t1, double *t2)
{
	double	discrim;
	double	inv_2a;
	double 	sq_discrim;
	
	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-5)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	*t1 = (-abc.y - sq_discrim) * inv_2a;
	*t2 = (-abc.y + sq_discrim) * inv_2a;
	//if (*t1 > *t2)//from buck, just sorting i think
	//	ft_swap(t1, t2);
	return (true);
} 

void	compute_abc(t_vec3 *abc, t_ray ray)
{
	abc->x = ray.dir.x * ray.dir.x + ray.dir.z * ray.dir.z;
	abc->y = 2 * (ray.origin.x * ray.dir.x + ray.origin.z * ray.dir.z);
	abc->z = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - 1;
}

bool	check_cap(t_ray ray, double t)
{
	double x;
	double z;

	x = ray.origin.x + t * ray.dir.x;
	z = ray.origin.z + t * ray.dir.z;
	if ((x * x + z * z) <= 1)
		return (true);
	return (false);	
}

//later return false if cyl.closed == false

bool	intersect_caps(t_ray ray, double half_h, double *t3, double *t4)
{
	double	t;
	bool	hit1;
	bool	hit2;

	*t3 = INFINITY;
	*t4 = INFINITY;
	if (fabs(ray.dir.y) < 1e-5)
		return (false);
	t = (-half_h - ray.origin.y) / ray.dir.y;
	hit1 = check_cap(ray, t);
	if (hit1)
		*t4 = t;
	t = (half_h - ray.origin.y) / ray.dir.y;
	hit2 = check_cap(ray, t);
	if (hit2)
		*t3 = t;
	if (hit1 || hit2)
		return (true);
	return (false);
}

bool	within_height(t_ray ray, double min, double max, double t)
{
	double y;
	
	if (t < 1e-5)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	if (y > min && y < max)
		return (true);
	return (false);
}

void	ray_cylinder_intersect(t_cylinder *cylinder, t_ray ray, t_intersects *intersects)
{
	t_vec3	abc;
	double	t1;
	double	t2;
	double 	t3;
	double 	t4;
	double	half_h;

	half_h = cylinder->height / 2.0;// store this as cyl height  at parse?
	ray = transform(ray, cylinder->transform);
	compute_abc(&abc, ray);
	if (abc.x != 0)
	{
		if (check_trunk_solutions(abc, &t1, &t2))
		{
			if (within_height(ray, -half_h, half_h, t1))
				intersect(intersects, cylinder, t1, CYLINDER);
			if (within_height(ray, -half_h, half_h, t2))
				intersect(intersects, cylinder, t2, CYLINDER);
		}
	}
	if (intersect_caps(ray, half_h, &t3, &t4))
	{
		if (t3 < INFINITY)
			intersect(intersects, cylinder, t3, CYLINDER);
		if (t4 < INFINITY)
			intersect(intersects, cylinder, t4, CYLINDER);
	}
}

void	check_cylinders(t_cylinder *cylinders, t_intersects *intersects, t_ray ray)
{
	t_cylinder	*curr_cy;

	if (cylinders == NULL)
		return ;
	curr_cy = cylinders;
	while (true)
	{
		ray_cylinder_intersect(curr_cy, ray, intersects);
		curr_cy = curr_cy->next;
		if (curr_cy == cylinders)
			break;
	}
}
