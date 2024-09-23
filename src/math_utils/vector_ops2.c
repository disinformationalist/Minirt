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

bool	is_normalized(t_vec3 vec)
{
	double	magnit;

	magnit = magnitude(vec);
	if ((magnit >= 1 - (1e-3)) && (magnit <= 1 + (1e-3)))
		return (true);
	else
		return (false);
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
