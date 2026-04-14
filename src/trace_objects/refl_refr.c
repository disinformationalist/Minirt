#include "minirt.h"

//threshold comparison tool

static inline float get_diff(t_norm_color f, t_norm_color s)
{
	return (fabs(f.r - s.r) + fabs(f.g - s.g) + fabs(f.b - s.b));
}

static inline t_vec3	reflect(t_vec3 in, t_vec3 normal)
{
	double	cos_a;

	cos_a = dot_product(in, normal);
	return (subtract_vec(in, scale_vec(2 * cos_a, normal)));
}

//consider a threshold set in comps for the depths, maybe light intensity based.

t_norm_color	get_reflected(t_trace *trace, t_comps comps, \
t_intersects *intersects, t_depths depths)
{
	t_norm_color	ref_col;
	float			new_weight;

	ref_col = color(0, 0, 0);
	if (comps.mat.ref && depths.refl > 0)
	{
		//other fuzzing method
		//comps.reflectv = norm_vec(add_vec(reflect(comps.ray.dir, comps.normal), scale_vec(.08, random_unit_vec())));

		/* comps.reflectv = norm_vec(reflect(comps.ray.dir, comps.normal));
		depths.refl--;
		
		ref_col = check_intersects(trace, ray(comps.reflectv, \
		comps.over_pnt), intersects, depths); */

		new_weight = depths.weight * comps.mat.ref;// * comps.reflectance;
		if (comps.mat.transp > 0.0)
			new_weight *= comps.reflectance;
		if (new_weight > .03)
		{
			comps.reflectv = norm_vec(reflect(comps.ray.dir, comps.normal));
			depths.refl--;
			depths.weight = new_weight;
			ref_col = check_intersects(trace, ray(comps.reflectv,
						comps.over_pnt), intersects, depths);
		}
	}
	else
		ref_col = color(0, 0, 0);
	return (ref_col);
}

static inline bool	refract(double n_ratio, t_vec3 eyev, \
t_vec3 normal, t_vec3 *refr_dir)
{
	double	sin2_t;
	double	cos_i;
	double	cos_t;

	cos_i = dot_product(eyev, normal);
	sin2_t = n_ratio * n_ratio * (1 - cos_i * cos_i);
	if (sin2_t > 1.0)
		return (false);
	cos_t = sqrt(1.0 - sin2_t);
	*refr_dir = scale_vec(n_ratio * cos_i - cos_t, normal);
	*refr_dir = norm_vec(subtract_vec(*refr_dir, scale_vec(n_ratio, eyev)));

	return (true);
}


double	schlick(t_comps comps)
{
	double	n;
	double	cos;
	double	sin2_t;
	double	cos2;
	double	res;

	cos = dot_product(comps.eyev, comps.normal);
	if (comps.n1 > comps.n2)
	{
		n = comps.n1 / comps.n2;
		sin2_t = n * n * (1.0 - cos * cos);
		if (sin2_t > 1.0)
			return (1.0);
		cos = sqrt(1.0 - sin2_t);
	}
	res = ((comps.n1 - comps.n2) / (comps.n1 + comps.n2));
	res *= res;
	cos = 1 - cos;
	cos2 = cos * cos;
	res = res + (1 - res) * cos2 * cos2 * cos;
	return (res);
}


t_norm_color	get_refracted(t_trace *trace, t_comps comps,
	t_intersects *intersects, t_depths depths)
{
	t_norm_color	refr_col;
	t_vec3			refractv;
	bool			refracted;
	double			new_weight;

	refr_col = color(0, 0, 0);
	if (comps.mat.transp > 0.0 && depths.refr > 0)
	{
		refracted = refract(comps.n1 / comps.n2, comps.eyev,
				comps.normal, &refractv);
		if (!refracted)
			return (color(0, 0, 0));
		//reflectance = schlick(comps);//store value for use in final color
		new_weight = depths.weight * comps.mat.transp;
		if (comps.mat.ref > 0.0)
			new_weight *= comps.transmittance;
		if (new_weight > .01)
		{
			depths.refr--;
			depths.weight = new_weight;
			refr_col = check_intersects(trace, ray(refractv,
						comps.under_pnt), intersects, depths);
		}
	}
	return (refr_col);
}


//orig non adaptive
/* t_norm_color	get_refracted(t_trace *trace, t_comps comps, \
t_intersects *intersects, t_depths depths)
{
	t_norm_color	refr_col;
	t_vec3			refractv;
	bool			refracted;

	if (comps.mat.transp && depths.refr > 0)
	{
		refracted = refract(comps.n1 / comps.n2, comps.eyev, \
		comps.normal, &refractv);
		if (!refracted)
			return (color(0, 0, 0));
		depths.refr--;
		refr_col = check_intersects(trace, ray(refractv, \
		comps.under_pnt), intersects, depths);
	}
	else
		refr_col = color(0, 0, 0);
	return (refr_col);
} */
