/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_hyperboloids.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:57:29 by arybarsk          #+#    #+#             */
/*   Updated: 2024/11/18 18:57:31 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline t_vec3 hyp_normal_at(t_point int_pnt, t_hyperboloid *hyperboloid)
{
	t_vec3	norm;

	int_pnt = mat_vec_mult(hyperboloid->transform, int_pnt);

	norm = vec(2 * int_pnt.x / (hyperboloid->rad1 * hyperboloid->rad1),
			-2 * int_pnt.y / (hyperboloid->half_h * hyperboloid->half_h),
			2 * int_pnt.z / (hyperboloid->rad2 * hyperboloid->rad2), 0);
	norm = mat_vec_mult(transpose(hyperboloid->transform), norm);
	norm.w = 0;
	return (norm_vec(norm));
}

static inline t_comps	set_hycomps(t_hyperboloid *hyperboloid, t_intersects *intersects, t_ray r, t_trace *trace)
{
	t_comps	comps;
	
	(void)trace;
	comps.t = intersects->closest->t;
	comps.ray = r;
	comps.point = add_vec(r.origin, scale_vec(comps.t, r.dir));
	comps.normal = hyp_normal_at(comps.point, hyperboloid);
	//comps.color = texture_cy_at(trace, comps.point, cylinder->transform, &comps.normal);//if texturing sets texture and bumps//todo for cyl
	comps.color = hyperboloid->color;//no texture, build out color options with patterns or texture
	comps.eyev = neg(r.dir);
	comps.mat = hyperboloid->mat;
	set_indicies(intersects, &comps.n1, &comps.n2);//if transp
	if (dot_product(comps.normal, comps.eyev) < 0)
	{
		comps.inside = true;
		comps.normal = neg(comps.normal);
	}
	else
		comps.inside = false;
	comps.over_pnt = add_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.under_pnt = subtract_vec(comps.point, scale_vec(1e-6, comps.normal));
	return (comps);
}

t_norm_color color_hyperboloid(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths)
{
	t_hyperboloid		*hyperboloid;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*curr_lt;
	/* t_norm_color	refl_col;
	t_norm_color	refr_col; */
	
	hyperboloid = (t_hyperboloid *)intersects->closest->object;
	lt_color = color(0, 0, 0);
	comps = set_hycomps(hyperboloid, intersects, r, trace);
	if (trace->lights)
	{
		curr_lt = trace->lights;
		while (true)
		{
			comps.light_dir = norm_vec(subtract_vec(curr_lt->center, comps.point));
			handle_light(trace, &comps, &lt_color, curr_lt);
			curr_lt = curr_lt->next;
			if (curr_lt == trace->lights)
				break;
		}	
	}
	comps.refl_col = get_reflected(trace, comps, intersects, depths);
	comps.refr_col = get_refracted(trace, comps, intersects, depths);
	return (get_final_color4(trace, comps, lt_color));		

	/* refl_col = get_reflected(trace, comps, intersects, depths);
	refr_col = get_refracted(trace, comps, intersects, depths);
	return (get_final_color3(trace, comps, lt_color, refl_col, refr_col)); */
}
//	return (get_final_color1(trace, color1, lt_color, comps.mat.amb));