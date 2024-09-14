#include "minirt.h"

t_ray	ray(t_vec3 dir, t_point origin)
{
	t_ray r;

	r.dir = dir;
	r.origin = origin;
	return (r);
}

t_vec3	norm_vec(t_vec3 vec)
{
	t_vec3	normed;
	double	length;

	length = magnitude(vec);
	if (!length)
		return (vec);
	normed.x = vec.x / length;
	normed.y = vec.y / length;
	normed.z = vec.z / length;
	normed.w = 0;
	return (normed);
}

t_vec3	cross_prod(t_vec3 vec1, t_vec3 vec2)
{
	t_vec3	res;

	res.x = vec1.y * vec2.z - vec1.z * vec2.y;
	res.y = vec1.z * vec2.x - vec1.x * vec2.z;
	res.z = vec1.x * vec2.y - vec1.y * vec2.x;
	res.w = 0;
	return (res);
}

t_vec3	scale_vec(double scalar, t_vec3 vec)
{
	t_vec3	res;

	res.x = scalar * vec.x;
	res.y = scalar * vec.y;
	res.z = scalar * vec.z;
	res.w = 0;
	return (res);
}

t_vec3	div_vec(double scalar, t_vec3 vec)
{
	t_vec3	res;

	res.x = vec.x / scalar;
	res.y = vec.y / scalar;
	res.z = vec.z / scalar;
	res.w = 0;
	return (res);
}

t_vec3 neg(t_vec3 vec)
{
	t_vec3 neg;

	neg.x = -vec.x;
	neg.y = -vec.y;
	neg.z = -vec.z;
	neg.w = 0;
	return (neg);
}

t_vec3	mult_vec(t_vec3 v1, t_vec3 v2)
{
	t_vec3	res;

	res.x = v1.x * v2.x;
	res.y = v1.y * v2.y;
	res.z = v1.z * v2.z;
	res.w = 0;
	return (res);
}