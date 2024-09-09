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

//ISSUES WITH CYLINDERS NEED FIXING, TRY WITH ONE CYL AND A LIGHT,  ALSO CAPS NEEDED
//problem seems to be cyl shadowing self. works ok without the obscure check function.

//this function will check each cylinder passed to it
// a, b and c got combined into one t_vec3 because of the norm 
static inline bool	check_solutions(t_vec3 abc, double *t1, double *t2)
{
	double	a;
	double	b;
	double	c;
	double	discrim;
	double	inv_2a;

	a = abc.x;
	b = abc.y;
	c = abc.z;
	discrim = b * b - 4 * a * c;
	if (discrim < 0)
		return (false);
	inv_2a = 0.5 / a;
	*t1 = (-b - sqrt(discrim)) * inv_2a;
	*t2 = (-b + sqrt(discrim)) * inv_2a;
	return (true);
}

bool	is_within_height(double t_val, t_cylinder cylinder, t_vec3 ray_dir, t_vec3 ray_orig)
{
	t_vec3	intersect_pnt;
	t_vec3	axis;
	double	projection_len;

	if (t_val <= 0)
		return (false);
	intersect_pnt = add_vec(ray_orig, scalar_mult_vec(t_val, ray_dir));
	axis = normalize_vec(cylinder.norm_vector);
	projection_len = dot_product(subtract_vec(intersect_pnt, cylinder.center), axis);
	if (projection_len >= 0 && projection_len <= cylinder.height)
		return (true);
	else
		return (false);
}

static inline void	compute_abc(t_cylinder cylinder, t_vec3 ray_dir, t_vec3 ray_orig, t_vec3 *abc)
{
	t_vec3	oc;//ray origin to center
	t_vec3	axis;//cylinder axis
	t_vec3	oc_proj;//oc projected on a plane perpendicular to axis
	t_vec3	ray_proj;//ray_dir projected on a plane perpendicular to axis

	oc = subtract_vec(ray_orig, cylinder.center);
	axis = normalize_vec(cylinder.norm_vector);
	oc_proj = subtract_vec(oc, scalar_mult_vec(dot_product(oc, axis), axis));
	ray_proj = subtract_vec(ray_dir, scalar_mult_vec(dot_product(ray_dir, axis), axis));
	abc->x = dot_product(ray_proj, ray_proj); //a
	abc->y = 2.0 * dot_product(ray_proj, oc_proj); //b
	abc->z = dot_product(oc_proj, oc_proj) - cylinder.radius * cylinder.radius; //c
}

bool	ray_cylinder_intersect(t_cylinder cylinder, t_vec3 ray_dir, t_vec3 ray_orig, double *t)
{
	t_vec3	abc;//coefficients of quadratic; a is abc.x, b is abc.y, c is abc.z
	double	t1;
	double	t2;
	double	closest_t;

	compute_abc(cylinder, ray_dir, ray_orig, &abc);
	if (check_solutions(abc, &t1, &t2))
	{
		closest_t = INFINITY;
		if (is_within_height(t1, cylinder, ray_dir, ray_orig))
			closest_t = t1;
		if (is_within_height(t2, cylinder, ray_dir, ray_orig) && t2 < closest_t)
			closest_t = t2;
		if (closest_t < INFINITY)
		{
			*t = closest_t;
			return (true);
		}
	}
	return (false);
}

void	check_cylinders(t_cylinder *cylinders, t_track_hits *closest, t_ray ray, double *t)
{
	t_cylinder	*curr_cy;

	if (cylinders == NULL)
		return ;
	curr_cy = cylinders;
	while (true)
	{
		if (ray_cylinder_intersect(*curr_cy, ray.direction, ray.origin, t))
		{
			if (*t < closest->t && *t > 0)
			{
				closest->t = *t;
				closest->object = curr_cy;
				closest->object_type = CYLINDER;
			}
		}
		curr_cy = curr_cy->next;
		if (curr_cy == cylinders)
			break;
	}
}

unsigned int color_cylinder(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_vec3			intersect_pnt;//light intersect with surface
	t_vec3			normal;
	t_vec3			light_dir;
	t_vec3			view_dir;
	t_vec3			oc;
	t_vec3			axis;
	double			light_intensity;
	double			cos_angle;
	t_cylinder		*cylinder;

	cylinder = (t_cylinder *)closest->object;

	if (trace->lights)
	{
		//plug closest->t back into ray eq for intersect point;
		intersect_pnt = add_vec(r.origin, scalar_mult_vec(closest->t, r.direction));
		oc = subtract_vec(intersect_pnt, cylinder->center);
		axis = normalize_vec(cylinder->norm_vector);//already normed unless a non normed entry. have parser check these.
		normal = normalize_vec(subtract_vec(oc, scalar_mult_vec(dot_product(oc, axis), axis)));
		light_dir = normalize_vec(subtract_vec(intersect_pnt, trace->lights->center));
		view_dir = normalize_vec(subtract_vec(r.origin, intersect_pnt));
		if (dot_product(normal, view_dir) > 0)
			normal = scalar_mult_vec(-1, normal);
		if (obscured(trace, intersect_pnt, light_dir, normal))//shadow ray test here, not yet right for cyl, planes?
				light_intensity = 0;
		else
		{
			cos_angle = dot_product(normal, light_dir);
			light_intensity	= trace->lights->brightness * fmax(cos_angle, 0.0);
		}
	}
	else
		light_intensity = 0;
	return (get_final_color(trace, cylinder->color, light_intensity));
}
