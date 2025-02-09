#include "minirt.h"

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

	if (comps.mat.ref && depths.refl > 0)
	{
		comps.reflectv = norm_vec(reflect(comps.ray.dir, comps.normal));
		depths.refl--;
		ref_col = check_intersects(trace, ray(comps.reflectv, \
		comps.over_pnt), intersects, depths);
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

t_norm_color	get_refracted(t_trace *trace, t_comps comps, \
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
