#include "minirt.h"

//returns false when vectors are =

bool	veccmp(t_vec3 v1, t_vec3 v2)
{
	double	eps;

	eps = 1e-6;
	if (fabs(v1.x - v2.x) > eps)
		return (true);
	if (fabs(v1.y - v2.y) > eps)
		return (true);
	if (fabs(v1.z - v2.z) > eps)
		return (true);
	return (false);
}

//uses rodrigues rotation formula to get inverse rotation

t_matrix_4x4	get_rotation(t_vec3 ax, double cos, double sin)
{
	t_matrix_4x4	res;
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
	res.m[2][2] = t * ax.z * ax.z + cos;
	res.m[2][3] = 0;
	res.m[3][0] = 0;
	res.m[3][1] = 0;
	res.m[3][2] = 0;
	res.m[3][3] = 1;
	return (res);
}

// takes a vec "from" rotates to "to" returns the matrix to rotate from into to

t_matrix_4x4	rot_to(t_vec3 from, t_vec3 to)
{
	t_matrix_4x4	res;
	t_vec3			axis;
	double			angle;

	norm_vec(from);
	to = norm_vec(to);
	if (!veccmp(from, to))
		return (identity(&res), res);
	if (!veccmp(from, neg(from)))
		return (rot_x(M_PI));
	axis = norm_vec(cross_prod(from, to));
	angle = acos(dot_product(from, to));
	res = get_rotation(axis, cos(angle), sin(angle));
	return (res);
}
