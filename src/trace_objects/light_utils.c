#include "minirt.h"

/* double	randf(void)
{
	return ((double)rand() / (double)(RAND_MAX + 1.0));
} */

static inline double randfx(Xoro128 *rng)
{
    return xoro128d(rng);
}


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

	if (comps.cos_a <= 0.0)
		return (0.0);
	ref = subtract_vec(scale_vec(2 * comps.cos_a, comps.normal), \
		comps.light_dir);
	spec = pow(fmax(dot_product(ref, comps.eyev), 0), mat.shine);
	return (mat.diff * fmax(comps.cos_a, 0.0) + mat.spec * spec);
}

//getting the current point to test on an area light

static inline t_vec3	pnt_on_light(t_light light, double u, double v, Xoro128 *rng)
{
	t_vec3		move_u;
	t_vec3		move_v;
	t_point		pos;

	move_u = scale_vec(u + randfx(rng), light.uvec);
	move_v = scale_vec(v + randfx(rng), light.vvec);
	pos = add_vec(light.corner, add_vec(move_u, move_v));
	return (pos);
}

//for fixed, non randomly jittered samples

static inline t_vec3	pnt_on_light_fixed(t_light light, double u, double v)
{
	t_vec3	move_u;
	t_vec3	move_v;
	t_point	pos;

	move_u = scale_vec(u + .5, light.uvec);
	move_v = scale_vec(v + .5, light.vvec);
	pos = add_vec(light.corner, add_vec(move_u, move_v));
	return (pos);
}
//delivers light intensity of light from an area light
/* 
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
				comps->cos_a = dot_product(comps->normal, norm_vec(comps->light_dir));
				comps->sqlt_int += get_light_int(*comps, comps->mat);
				tot_int += 1.0;
			}
		}
	}
	comps->sqlt_int /= lt.samples;
	return (tot_int / lt.samples);
} */

static inline double	inten_at(t_trace *trace, t_light lt, t_comps *comps, t_intersects *intersects)
{
	double	sum_int;
	int		i;
	int		j;
	t_point	lt_pos;

	sum_int = 0.0;
	j = -1;
	while (++j < lt.vsteps)
	{
		i = -1;
		while (++i < lt.usteps)
		{
			lt_pos = pnt_on_light(lt, i, j, &intersects->rng);
			comps->light_dir = norm_vec(subtract_vec(lt_pos, comps->point));
			if (!obscured_b(trace, lt_pos, *comps))
			{
				comps->cos_a = dot_product(comps->normal, comps->light_dir);
				sum_int += get_light_int(*comps, comps->mat);
			}
		}
	}
	return (sum_int / lt.samples);
}






//adaptive implement


static inline double sample_area_light_point(t_trace *trace, t_light lt,
	t_comps *comps, double u, double v, t_intersects *intersects)
{
	t_point	lt_pos;

	lt_pos = pnt_on_light(lt, u, v, &intersects->rng);
	comps->light_dir = norm_vec(subtract_vec(lt_pos, comps->point));
	if (obscured_b(trace, lt_pos, *comps))
		return (0.0);
	comps->cos_a = dot_product(comps->normal, comps->light_dir);
	return (get_light_int(*comps, comps->mat));
}

static inline double	sample_area_light_point_fixed(t_trace *trace, t_light lt,
	t_comps *comps, double u, double v)
{
	t_point	lt_pos;

	lt_pos = pnt_on_light_fixed(lt, u, v);
	comps->light_dir = norm_vec(subtract_vec(lt_pos, comps->point));
	if (obscured_b(trace, lt_pos, *comps))
		return (0.0);
	comps->cos_a = dot_product(comps->normal, comps->light_dir);
	return (get_light_int(*comps, comps->mat));
}

//select sampling version based upon branch(refl, refr) weight

static inline double	inten_at_low(t_trace *trace, t_light lt, t_comps *comps, t_intersects *intersects)
{
	return (sample_area_light_point(trace, lt, comps,
			lt.usteps * 0.5, lt.vsteps * 0.5, intersects));
}

static inline double	inten_at_medium(t_trace *trace, t_light lt, t_comps *comps, t_intersects *intersects)
{
	double	s0;
	double	s1;
	double	s2;
	double	s3;

	s0 = sample_area_light_point(trace, lt, comps,
			lt.usteps * 0.25, lt.vsteps * 0.25, intersects);
	s1 = sample_area_light_point(trace, lt, comps,
			lt.usteps * 0.75, lt.vsteps * 0.25, intersects);
	s2 = sample_area_light_point(trace, lt, comps,
			lt.usteps * 0.25, lt.vsteps * 0.75, intersects);
	s3 = sample_area_light_point(trace, lt, comps,
			lt.usteps * 0.75, lt.vsteps * 0.75, intersects);
	return ((s0 + s1 + s2 + s3) * 0.25);
}

static inline double	inten_at_adaptive(t_trace *trace, t_light lt, t_comps *comps, t_intersects *intersects)
{
	double	s0;
	double	s1;
	double	s2;
	double	s3;
	double	avg;
	double	minv;
	double	maxv;
	double	sum;
	int		i;
	int		j;

	// 4 coarse samples, one in each quadrant
	s0 = sample_area_light_point(trace, lt, comps, lt.usteps * 0.25, lt.vsteps * 0.25, intersects);
	s1 = sample_area_light_point(trace, lt, comps, lt.usteps * 0.75, lt.vsteps * 0.25, intersects);
	s2 = sample_area_light_point(trace, lt, comps, lt.usteps * 0.25, lt.vsteps * 0.75, intersects);
	s3 = sample_area_light_point(trace, lt, comps, lt.usteps * 0.75, lt.vsteps * 0.75, intersects);

	minv = fmin(fmin(s0, s1), fmin(s2, s3));
	maxv = fmax(fmax(s0, s1), fmax(s2, s3));
	avg = (s0 + s1 + s2 + s3) * 0.25;

	if (maxv - minv < 0.05)
		return (avg);

	// refine: full stratified sampling
	sum = 0.0;
	j = -1;
	while (++j < lt.vsteps)
	{
		i = -1;
		while (++i < lt.usteps)
			sum += sample_area_light_point(trace, lt, comps, i, j, intersects);
	}
	return (sum / lt.samples);
}

static inline double	inten_at_weighted(t_trace *trace, t_light lt, t_comps *comps, t_intersects *intersects)
{
	if (comps->weight <= 0.015)
		return (inten_at_low(trace, lt, comps, intersects));
	if (comps->weight <= 0.05)
		return (inten_at_medium(trace, lt, comps, intersects));
	return (inten_at_adaptive(trace, lt, comps, intersects));
}


//end adaptive

//handle all non ambient light contribution for each source

void	handle_light(t_trace *trace, t_comps *comps, \
t_norm_color *lt_color, t_light *curr_lt, t_intersects *intersects)
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
	/* else if (curr_lt->type == AREA)//old
		*lt_color = sum_rgbs(*lt_color, mult_color(curr_lt->brightness \
		* inten_at(trace, *curr_lt, comps) * comps->sqlt_int, curr_lt->color)); */
	/* else if (curr_lt->type == AREA)//current in use
		*lt_color = sum_rgbs(*lt_color, mult_color(curr_lt->brightness \
		* inten_at(trace, *curr_lt, comps), curr_lt->color)); */
	else if (curr_lt->type == AREA)//adaptive
	*lt_color = sum_rgbs(*lt_color, mult_color(
		curr_lt->brightness * inten_at_weighted(trace, *curr_lt, comps, intersects),
		curr_lt->color));
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

//testing bvh shadow
static inline double sample_area_light_point_fixed_testing(t_trace *trace, t_light lt,
	t_comps *comps, double u, double v, t_intersects *intersects)
{
	t_point	lt_pos;

	lt_pos = pnt_on_light_fixed(lt, u, v);
	comps->light_dir = norm_vec(subtract_vec(lt_pos, comps->point));
	if (obscured_b_testing(trace, lt_pos, *comps, intersects))
		return (0.0);
	comps->cos_a = dot_product(comps->normal, comps->light_dir);
	return (get_light_int(*comps, comps->mat));
}

static inline double sample_area_light_point_testing(t_trace *trace, t_light lt,
	t_comps *comps, double u, double v, t_intersects *intersects)
{
	t_point	lt_pos;

	lt_pos = pnt_on_light(lt, u, v, &intersects->rng);
	comps->light_dir = norm_vec(subtract_vec(lt_pos, comps->point));
	if (obscured_b_testing(trace, lt_pos, *comps, intersects))
		return (0.0);
	comps->cos_a = dot_product(comps->normal, comps->light_dir);
	return (get_light_int(*comps, comps->mat));
}

static inline double	inten_at_low_testing(t_trace *trace, t_light lt, t_comps *comps, t_intersects *intersects)//make two points used
{
	return (sample_area_light_point_testing(trace, lt, comps,
			lt.usteps * 0.5, lt.vsteps * 0.5, intersects));
}

static inline double	inten_at_medium_testing(t_trace *trace, t_light lt, t_comps *comps, t_intersects *intersects)
{
	double	s0;
	double	s1;
	double	s2;
	double	s3;

	s0 = sample_area_light_point_testing(trace, lt, comps,
			lt.usteps * 0.25, lt.vsteps * 0.25, intersects);
	s1 = sample_area_light_point_testing(trace, lt, comps,
			lt.usteps * 0.75, lt.vsteps * 0.25, intersects);
	s2 = sample_area_light_point_testing(trace, lt, comps,
			lt.usteps * 0.25, lt.vsteps * 0.75, intersects);
	s3 = sample_area_light_point_testing(trace, lt, comps,
			lt.usteps * 0.75, lt.vsteps * 0.75, intersects);
	return ((s0 + s1 + s2 + s3) * 0.25);
}

static inline double	inten_at_adaptive_testing(t_trace *trace, t_light lt, t_comps *comps, t_intersects *intersects)
{
	double	s0;
	double	s1;
	double	s2;
	double	s3;
	double	avg;
	double	minv;
	double	maxv;
	double	sum;
	int		i;
	int		j;

	// 4 samples, one in each quadrant
	s0 = sample_area_light_point_testing(trace, lt, comps, lt.usteps * 0.25, lt.vsteps * 0.25, intersects);
	s1 = sample_area_light_point_testing(trace, lt, comps, lt.usteps * 0.75, lt.vsteps * 0.25, intersects);
	s2 = sample_area_light_point_testing(trace, lt, comps, lt.usteps * 0.25, lt.vsteps * 0.75, intersects);
	s3 = sample_area_light_point_testing(trace, lt, comps, lt.usteps * 0.75, lt.vsteps * 0.75, intersects);

	minv = fmin(fmin(s0, s1), fmin(s2, s3));
	maxv = fmax(fmax(s0, s1), fmax(s2, s3));
	avg = (s0 + s1 + s2 + s3) * 0.25;

	if (maxv - minv < 0.05)
		return (avg);

	// refine: full stratified sampling
	sum = 0.0;
	j = -1;
	while (++j < lt.vsteps)
	{
		i = -1;
		while (++i < lt.usteps)
			sum += sample_area_light_point_testing(trace, lt, comps, i, j, intersects);
	}
	return (sum / lt.samples);
}

static inline double	inten_at_weighted_testing(t_trace *trace, t_light lt, t_comps *comps, t_intersects *intersects)
{
	if (comps->weight <= 0.015)
		return (inten_at_low_testing(trace, lt, comps, intersects));
	if (comps->weight <= 0.05)
		return (inten_at_medium_testing(trace, lt, comps, intersects));
	return (inten_at_adaptive_testing(trace, lt, comps, intersects));
}


void	handle_light_testing(t_trace *trace, t_comps *comps, \
t_norm_color *lt_color, t_light *curr_lt, t_intersects *intersects)
{
	if (curr_lt->type == SPOT)
	{
		comps->spot_int = get_spot_int(comps->light_dir, curr_lt);
		if (comps->spot_int)
		{
			if (!obscured_b_testing(trace, curr_lt->center, *comps, intersects))
			{
				comps->cos_a = dot_product(comps->normal, comps->light_dir);
				*lt_color = sum_rgbs(*lt_color, \
				mult_color(curr_lt->brightness * comps->spot_int \
				* get_light_int(*comps, comps->mat), curr_lt->color));
			}
		}
	}
	else if (curr_lt->type == AREA)//adaptive
	*lt_color = sum_rgbs(*lt_color, mult_color(
		curr_lt->brightness * inten_at_weighted_testing(trace, *curr_lt, comps, intersects),
		curr_lt->color));
	else
	{
		if (!obscured_b_testing(trace, curr_lt->center, *comps, intersects))
		{
			comps->cos_a = dot_product(comps->normal, comps->light_dir);
			*lt_color = sum_rgbs(*lt_color, mult_color(curr_lt->brightness \
			* get_light_int(*comps, comps->mat), curr_lt->color));
		}
	}
}
