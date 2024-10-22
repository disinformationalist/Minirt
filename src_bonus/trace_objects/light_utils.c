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

	ref = subtract_vec(scale_vec(2 * comps.cos_a, comps.normal), comps.light_dir);
	spec = pow(fmax(dot_product(ref, comps.eyev), 0), mat.shine);
	light_int = mat.diff * fmax(comps.cos_a, 0.0) + mat.spec * spec;
	return (light_int);
}

static inline t_vec3	pnt_on_light(t_sqlight light, double u, double v)
{
	t_vec3		move_u;
	t_vec3		move_v;
	t_point		pos;
	static int	i = 0;

	move_u = scale_vec(u + light.jitter[i % light.samples], light.uvec);
	i++;
	move_v = scale_vec(v + light.jitter[i % light.samples], light.vvec);
	pos = add_vec(light.corner, add_vec(move_u, move_v));
	return (pos);
}

static inline double intensity_at(t_trace *trace, t_sqlight light, t_comps *comps)
{
	double	tot_int;
	int		i;
	int		j;
	t_point lt_pos;

	tot_int = 0.0;
	comps->sqlt_int = 0.0;
	j = -1;
	while (++j < light.vsteps)
	{
		i = -1;
		while(++i < light.usteps)
		{
			lt_pos = pnt_on_light(light, i, j);
			comps->light_dir = norm_vec(subtract_vec(lt_pos, comps->point));
			if (!obscured_b(trace, lt_pos, *comps))
			{
				comps->cos_a = dot_product(comps->normal, comps->light_dir);
				comps->sqlt_int += get_light_int(*comps, comps->mat);
				tot_int += 1.0;
			}
		}
	}
	comps->sqlt_int /= light.samples;
	return (tot_int / light.samples);
}

//handle all non ambient light contribution for each source

void	handle_light(t_trace *trace, t_comps *comps, t_norm_color *lt_color, t_light *curr_lt)
{
	if (curr_lt->type == SPOT)
	{
		comps->spot_int = get_spot_int(comps->light_dir, curr_lt);
		if (comps->spot_int)
		{
			comps->cos_a = dot_product(comps->normal, comps->light_dir);
			if (!obscured_b(trace, curr_lt->center, *comps))
				*lt_color = sum_rgbs(*lt_color, mult_color(curr_lt->brightness * comps->spot_int * get_light_int(*comps, comps->mat), curr_lt->color));
		}
	}
	else
	{
	/* 	// for sqlight if (curr_lt->type == AREA, sq)
		double sqlt_inten;
		sqlt_inten = intensity_at(trace, *(trace->sqlt), comps);
	//	*lt_color = sum_rgbs(*lt_color, mult_color(curr_lt->brightness * sqlt_inten * comps->sqlt_int, curr_lt->color));
		*lt_color = sum_rgbs(*lt_color, mult_color(trace->sqlt->brightness * sqlt_inten * comps->sqlt_int, curr_lt->color)); */

		
		//point light
		comps->cos_a = dot_product(comps->normal, comps->light_dir);
		if (!obscured_b(trace, curr_lt->center, *comps))
			*lt_color = sum_rgbs(*lt_color, mult_color(curr_lt->brightness * get_light_int(*comps, comps->mat), curr_lt->color));
	}
}
