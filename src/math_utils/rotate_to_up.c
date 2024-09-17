#include "minirt.h"

static inline bool	veccmp(t_vec3 v1, t_vec3 v2)
{
	if (v1.x != v2.x)
		return (true);
	if (v1.y != v2.y)
		return (true);
	if (v1.z != v2.z)
		return (true);
	return (false);
}

//uses rodrigues got get rotation

static inline t_matrix_4x4 get_rotation(t_vec3 ax, double cos, double sin)
{
	t_matrix_4x4 	res;
	double			t;

	t = 1 - cos;
	res.m[0][0] = t * ax.x * ax.x + cos;
	res.m[0][1] = t * ax.x * ax.y - sin * ax.z;
	res.m[0][2] = t * ax.x * ax.z + sin * ax.y;
	res.m[0][3] = 0;
	res.m[1][0] = t * ax.x * ax.y + sin * ax.z;
	res.m[1][1] = t * ax.y * ax.y + cos;
	res.m[1][2] = t * ax.y * ax.z - sin * ax.x;
	res.m[1][3] = 0;
	res.m[2][0] = t * ax.x * ax.z - sin * ax.y;
	res.m[2][1] = t * ax.y * ax.z + sin * ax.x;
	res.m[2][2] = t * ax.z * ax.z  + cos;
	res.m[2][3] = 0;
	res.m[3][0] = 0;
	res.m[3][1] = 0;
	res.m[3][2] = 0;
	res.m[3][3] = 1;
	return (res);
}

// takes an orientation vec, returns the matrix to transform to up vec

t_matrix_4x4 rot_up(t_vec3 ori)
{
	t_matrix_4x4	res;
	t_vec3			axis;
	t_vec3			up;
	double			angle;

	norm_vec(ori);
	up = vec(0, 1, 0, 0);
	if (!veccmp(ori, up))
		return (identity(&res), res);
	if (!veccmp(ori, vec(0, -1, 0, 0)))
		return (rot_x(M_PI));
	axis = cross_prod(ori, up);
	angle = acos(dot_product(norm_vec(ori), norm_vec(up)));
	res = get_rotation(axis, cos(angle), sin(angle));
	return (res);
}
