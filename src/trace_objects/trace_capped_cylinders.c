/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_capped_cylinders.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:09:24 by arybarsk          #+#    #+#             */
/*   Updated: 2024/12/16 13:09:28 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	check_cylinders(t_cylinder *cylinders, t_track_hits *closest,
		t_ray ray, double *t)
{
	t_cylinder	*curr_cy;

	if (cylinders == NULL)
		return ;
	curr_cy = cylinders;
	while (true)
	{
		if (ray_cylinder_intersect(*curr_cy, ray, t))
		{
			if (*t < closest->t)
			{
				closest->t = *t;
				closest->object = curr_cy;
				closest->object_type = CYLINDER;
			}
		}
		curr_cy = curr_cy->next;
		if (curr_cy == cylinders)
			break ;
	}
}

//diff plus specular for sp

static inline double	get_cylight_int(t_vec3 norm, t_vec3 light_dir,
				t_vec3 view_dir)
{
	t_vec3	ref;
	double	spec;
	double	light_int;
	double	cos_a;

	cos_a = dot_product(norm, light_dir);
	ref = subtract_vec(scale_vec(2 * cos_a, norm), light_dir);
	spec = pow(fmax(dot_product(ref, view_dir), 0), 200);
	light_int = fmax(cos_a, 0.0) + .5 * spec;
	return (light_int);
}

static inline t_vec3	cyl_normal_at(t_point int_pnt, t_matrix_4x4 transform)
{
	t_vec3	norm;
	double	dist;

	int_pnt = mat_vec_mult(transform, int_pnt);
	dist = int_pnt.x * int_pnt.x + int_pnt.z * int_pnt.z;
	if (dist < 1 && int_pnt.y >= 1 - 1e-6)
		norm = vec(0, 1, 0, 0);
	else if (dist < 1 && int_pnt.y <= -1 + 1e-6)
		norm = vec(0, -1, 0, 0);
	else
		norm = vec(int_pnt.x, 0, int_pnt.z, 0);
	norm = mat_vec_mult(transpose(transform), norm);
	norm.w = 0;
	return (norm_vec(norm));
}

t_norm_color	color_cylinder(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_cylinder		*cylinder;
	t_vec3			int_pnt;
	t_vec3			normal;
	t_vec3			light_dir;
	double			light_int;

	cylinder = (t_cylinder *)closest->object;
	light_int = 0;
	if (trace->lights)
	{
		int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		normal = cyl_normal_at(int_pnt, cylinder->transform);
		light_dir = norm_vec(subtract_vec(trace->lights->center, int_pnt));
		if (dot_product(normal, r.dir) > 0)
			normal = neg(normal);
		if (!obscured(trace, int_pnt, light_dir, normal))
			light_int = trace->lights->brightness * get_cylight_int(normal,
					light_dir, neg(r.dir));
	}
	return (get_final_color(trace, cylinder->color, light_int));
}
