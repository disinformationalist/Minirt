#include "minirt.h"

//returns false when vectors are =

bool	veccmp(t_vec3 from, t_vec3 v2)
{
	double	eps;

	eps = 1e-6;
	if (fabs(from.x - v2.x) > eps)
		return (true);
	if (fabs(from.y - v2.y) > eps)
		return (true);
	if (fabs(from.z - v2.z) > eps)
		return (true);
	return (false);
}

//uses rodrigues rotation formula to get inverse rotation

/* t_mat4	get_rotation(t_vec3 ax, double cos, double sin)
{
	t_mat4	res;
	double			t;

	t = 1 - cos;
	res.mat[0][0] = t * ax.x * ax.x + cos;
	res.mat[0][1] = t * ax.x * ax.y - sin * ax.z;
	res.mat[0][2] = t * ax.x * ax.z + sin * ax.y;
	res.mat[0][3] = 0;
	res.mat[1][0] = t * ax.x * ax.y + sin * ax.z;
	res.mat[1][1] = t * ax.y * ax.y + cos;
	res.mat[1][2] = t * ax.y * ax.z - sin * ax.x;
	res.mat[1][3] = 0;
	res.mat[2][0] = t * ax.x * ax.z - sin * ax.y;
	res.mat[2][1] = t * ax.y * ax.z + sin * ax.x;
	res.mat[2][2] = t * ax.z * ax.z + cos;
	res.mat[2][3] = 0;
	res.mat[3][0] = 0;
	res.mat[3][1] = 0;
	res.mat[3][2] = 0;
	res.mat[3][3] = 1;
	return (res);
} */

t_mat4	get_rotation(t_vec3 ax, double cos, double sin)
{
	t_mat4	res;
	double			t;

	t = 1 - cos;
	res.mat[0] = t * ax.x * ax.x + cos;
	res.mat[4] = t * ax.x * ax.y - sin * ax.z;
	res.mat[8] = t * ax.x * ax.z + sin * ax.y;
	res.mat[12] = 0;
	res.mat[1] = t * ax.x * ax.y + sin * ax.z;
	res.mat[5] = t * ax.y * ax.y + cos;
	res.mat[9] = t * ax.y * ax.z - sin * ax.x;
	res.mat[13] = 0;
	res.mat[2] = t * ax.x * ax.z - sin * ax.y;
	res.mat[6] = t * ax.y * ax.z + sin * ax.x;
	res.mat[10] = t * ax.z * ax.z + cos;
	res.mat[14] = 0;
	res.mat[3] = 0;
	res.mat[7] = 0;
	res.mat[11] = 0;
	res.mat[15] = 1;
	return (res);
}

// takes a vec "from" rotates to "to" returns the matrix to rotate from into to

t_mat4	rot_to(t_vec3 from, t_vec3 to)
{
	t_mat4	res;
	t_vec3	axis;
	double	angle;

	norm_vec(from);
	to = norm_vec(to);
	if (!veccmp(from, to))
		return (identity());
	if (!veccmp(from, neg(from)))
		return (rot_x(M_PI));
	axis = norm_vec(cross_prod(from, to));
	angle = acos(dot_product(from, to));
	res = get_rotation(axis, cos(angle), sin(angle));
	return (res);
}

t_quat	qrot_to(t_vec3 from, t_vec3 to)
{
	t_quat	res;
	t_vec3	axis;
	double	angle;

	from = norm_vec(from);
	to = norm_vec(to);
	if (!veccmp(from, to))
		return (vec(0, 0, 0, 1));
	if (!veccmp(from, neg(from)))
	{
		axis = (fabs(from.x) > fabs(from.z)) ? vec(-from.y, from.x, 0, 0) : vec(0, -from.z, from.y, 0);
		axis = norm_vec(axis);
		return (vec(0, axis.x, axis.y ,axis.z));
	}
	axis = norm_vec(cross_prod(from, to));
	angle = acos(dot_product(from, to));
	
	res = angle_to_quat(angle, axis.x, axis.y, axis.z);
	return (res);
}

t_vec3	extract_rot(t_mat4 rotmat)
{	
	t_vec3			rot;
	t_vec3			temp;

	temp = mat_vec_mult(rotmat, vec(0, 1, 0, 0));
	rot.x = atan2(temp.z, temp.y);
	/* if (rot.x < 0)
		rot.x += TWO_PI; */
	
	temp = mat_vec_mult(rotmat, vec(0, 0, 1, 0));
	rot.y = atan2(temp.x, temp.z);
	/* if (rot.y < 0)
		rot.y += TWO_PI; */

	temp = mat_vec_mult(rotmat, vec(0, 1, 0, 0));
	rot.z = -atan2(temp.x, temp.y);
	/* if (rot.z < 0)
		rot.z += TWO_PI; */
	check_tolerance(&temp);
	return (rot);
}
