#include "minirt.h"

t_ray	ray(t_vec3 dir, t_point origin)
{
	t_ray	r;

	r.dir = dir;
	r.origin = origin;
	return (r);
}

t_ray	transform(t_ray r, t_mat4 m)
{
	t_ray	new;

	new.dir = mat_vec_mult(m, r.dir);
	new.origin = mat_vec_mult(m, r.origin);
	return (new);
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
