#include "minirt.h"

/* t_ray	ray(t_vec3 dir, t_point origin)
{
	t_ray r;

	r.direction = dir;
	r.origin = origin;
	return (r);
} */

t_vec3	normalize_vec(t_vec3 vec)
{
	t_vec3	normed;
	double	length;

	length = magnitude(vec);
	if (!length)
		return (vec);
	normed.x = vec.x / length;
	normed.y = vec.y / length;
	normed.z = vec.z / length;
	return (normed);
}

t_vec3	cross_prod(t_vec3 vec1, t_vec3 vec2)
{
	t_vec3	res;

	res.x = vec1.y * vec2.z - vec1.z * vec2.y;
	res.y = vec1.z * vec2.x - vec1.x * vec2.z;
	res.z = vec1.x * vec2.y - vec1.y * vec2.x;
	return (res);
}

t_vec3	scalar_mult_vec(double scalar, t_vec3 vec)
{
	t_vec3	res;

	res.x = scalar * vec.x;
	res.y = scalar * vec.y;
	res.z = scalar * vec.z;
	return (res);
}

t_vec3	div_vec(double scalar, t_vec3 vec)
{
	t_vec3	res;

	res.x = vec.x / scalar;
	res.y = vec.y / scalar;
	res.z = vec.z / scalar;
	return (res);
}