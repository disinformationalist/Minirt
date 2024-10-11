#include "minirt.h"

/* 		   soft
		   spot
		  light 
		 / 	|  \
		/  / \  \
	   /  /   \  \
	  /  /     \  \
	 /  /	    \  \
			^	  ^
			|	  |
	inner cone    outer cone
(full intensity)  (attenuating toward outer edge)	 */	

//get the spotlight strength
static inline double get_spot_int(t_vec3 light_dir, t_light *splight)
{
	double	cos_theta;
	cos_theta = dot_product(splight->dir, light_dir);
	if (cos_theta > splight->inner_cone)
		return (1.0);
	else if (cos_theta > splight->outer_cone)
		return ((cos_theta - splight->outer_cone) * splight->inv_conediff);
	else
		return (0);
}

//diffuse plus specular

static inline double	get_light_int(t_comps comps, t_mat mat)
{
	t_vec3	ref;
	double	spec;
	double	light_int;

	ref = subtract_vec(scale_vec(2 * comps.cos_angle, comps.normal), comps.light_dir);
	spec = pow(fmax(dot_product(ref, comps.eyev), 0), mat.shine);
	light_int = mat.diff * fmax(comps.cos_angle, 0.0) + mat.spec * spec;
	return (light_int);
}

//handle all non ambient light contribution for each source

void	handle_light(t_trace *trace, t_comps *comps, t_norm_color *lt_color, t_light *curr_lt)
{
	if (curr_lt->type == SPOT)
	{
		comps->spot_int = get_spot_int(comps->light_dir, curr_lt);
		if (comps->spot_int)
		{
			comps->cos_angle = dot_product(comps->normal, comps->light_dir);
			if (!obscured_b(trace, ray(comps->light_dir, comps->over_pnt), curr_lt->center, comps->point))
				*lt_color = sum_rgbs(*lt_color, mult_color(curr_lt->brightness * comps->spot_int * get_light_int(*comps, comps->mat), curr_lt->color));
		}
	}
	else
	{
		comps->cos_angle = dot_product(comps->normal, comps->light_dir);
		if (!obscured_b(trace, ray(comps->light_dir, comps->over_pnt), curr_lt->center, comps->point))
			*lt_color = sum_rgbs(*lt_color, mult_color(curr_lt->brightness * get_light_int(*comps, comps->mat), curr_lt->color));
	}
}

t_norm_color get_reflected(t_trace *trace, t_comps comps, t_track_hits *closest, int depth)
{
	t_norm_color ref_col;

	if (comps.mat.ref && depth > 0)
	{
		
		comps.reflectv = norm_vec(reflect(comps.ray.dir, comps.normal));
		
		
		ref_col = check_intersects(trace, ray(comps.reflectv, comps.over_pnt), closest, depth - 1);

	}
	else
		ref_col = color(0, 0, 0);		
	return (ref_col);
}
