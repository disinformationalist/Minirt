#include "minirt.h"

/* 	       soft
           spot
          light
         / 	|  \
        /  / \  \
       /  /   \  \
      /  /     \  \
     /  /       \  \
            ^     ^
            |     |
    inner cone  outer cone
(full intensity)  (attenuating toward outer edge)*/	

//get the spotlight strength

static inline double	get_spot_int(t_vec3 light_dir, t_light *splight)
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

//diffuse plus specular, phong model

static inline double	get_light_int(t_comps comps, t_mat mat)
{
	t_vec3	ref;
	double	spec;
	double	light_int;

	ref = subtract_vec(scale_vec(2 * comps.cos_a, comps.normal), \
		comps.light_dir);
	spec = pow(fmax(dot_product(ref, comps.eyev), 0), mat.shine);
	light_int = mat.diff * fmax(comps.cos_a, 0.0) + mat.spec * spec;
	return (light_int);
}

//getting the current point to test on an area light

static inline t_vec3	pnt_on_light(t_light light, double u, double v)
{
	t_vec3		move_u;
	t_vec3		move_v;
	t_point		pos;

	move_u = scale_vec(u + randf(), light.uvec);
	move_v = scale_vec(v + randf(), light.vvec);
	pos = add_vec(light.corner, add_vec(move_u, move_v));
	return (pos);
}

//delivers light intensity of light from an area light

static inline double	inten_at(t_trace *trace, t_light lt, t_comps *comps)
{
	double	tot_int;
	int		i;
	int		j;
	t_point	lt_pos;

	tot_int = 0.0;
	comps->sqlt_int = 0.0;
	j = -1;
	while (++j < lt.vsteps)
	{
		i = -1;
		while (++i < lt.usteps)
		{
			lt_pos = pnt_on_light(lt, i, j);
			comps->light_dir = norm_vec(subtract_vec(lt_pos, comps->point));
			if (!obscured_b(trace, lt_pos, *comps))
			{
				comps->cos_a = dot_product(comps->normal, comps->light_dir);
				comps->sqlt_int += get_light_int(*comps, comps->mat);
				tot_int += 1.0;
			}
		}
	}
	comps->sqlt_int /= lt.samples;
	return (tot_int / lt.samples);
}

//handle all non ambient light contribution for each source

void	handle_light(t_trace *trace, t_comps *comps, \
t_norm_color *lt_color, t_light *curr_lt)
{
	if (curr_lt->type == SPOT)
	{
		comps->spot_int = get_spot_int(comps->light_dir, curr_lt);
		if (comps->spot_int)
		{
			if (!obscured_b(trace, curr_lt->center, *comps))
			{
				comps->cos_a = dot_product(comps->normal, comps->light_dir);
				*lt_color = sum_rgbs(*lt_color, \
				mult_color(curr_lt->brightness * comps->spot_int \
				* get_light_int(*comps, comps->mat), curr_lt->color));
			}
		}
	}
	else if (curr_lt->type == AREA)
		*lt_color = sum_rgbs(*lt_color, mult_color(curr_lt->brightness \
		* inten_at(trace, *curr_lt, comps) * comps->sqlt_int, curr_lt->color));
	else
	{
		if (!obscured_b(trace, curr_lt->center, *comps))
		{
			comps->cos_a = dot_product(comps->normal, comps->light_dir);
			*lt_color = sum_rgbs(*lt_color, mult_color(curr_lt->brightness \
			* get_light_int(*comps, comps->mat), curr_lt->color));
		}
	}
}
