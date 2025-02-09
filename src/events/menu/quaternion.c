#include "minirt.h"

t_vec3	norm_quat(t_vec3 quat)
{
	t_vec3	normed;
	double	mag;

	mag = sqrt(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w);
	if (mag == 0)
		return (quat);
	normed.x = quat.x / mag;
	normed.y = quat.y / mag;
	normed.z = quat.z / mag;
	normed.w = quat.w / mag;
	return (normed);
}

t_vec3	quat_to_rot(t_vec3 quat)
{
	double	x, y, z, w;
	t_vec3	rot;

	x = quat.x;
	y = quat.y;
	z = quat.z;
	w = quat.w;

	rot.x = atan2( 2 * (x * w + y * z), 1 - 2 * (x * x + y * y));
	rot.y = asin(-2 * (y * w - x * z));
	rot.z = atan2(2 * (z * w + x * y), 1 - 2 * (y * y + z * z));
	rot.w = 0;
	return (rot);
}

t_vec3	quat_get_rot(t_on *on, t_trace *trace)
{
	t_mat4			rottran;
	t_vec3			rot;
	double			qtrace;
	double			s;
	t_vec3			quat;

	double r11, r22, r33, max, r32, r23, r31, r13, r21, r12;
	rottran = get_rottran(on->type, trace);
	r11 = rottran.mat[0];
	r22 = rottran.mat[5];
	r33 = rottran.mat[10];
	max = fmax(fmax(r11, r22), r33);

	r32 = rottran.mat[6];
	r23 = rottran.mat[11];
	r21 = rottran.mat[1];
	r12 = rottran.mat[4];
	r31 = rottran.mat[2];
	r13 = rottran.mat[8];
	qtrace = r11 + r22 + r33;
	if (qtrace > 0)
	{
		s = 2 * sqrt(1 + qtrace);
		quat.w = s / 4;
		quat.x = (r32 - r23) / s;
		quat.y = (r13 - r31) / s;
		quat.z = (r21 - r12) / s;
	}
	else if (max == r11)
	{
		s = 2 * sqrt(1 + r11 - r22 - r33);
		quat.x = s / 4;
		quat.w = (r32 - r23) / s;
		quat.y = (r12 + r21) / s;
		quat.z = (r13 + r31) / s;	
	}
	else if (max == r22)
	{
		s = 2 * sqrt(1 + r22 - r11 - r33);
		quat.y = s / 4;
		quat.w = (r13 - r31) / s;
		quat.x = (r12 + r21) / s;
		quat.z = (r23 + r32) / s;	
	}
	else
	{
		s = 2 * sqrt(1 + r33 - r11 - r22);
		quat.z = s / 4;
		quat.w = (r21 - r12) / s;
		quat.x = (r13 + r31) / s;
		quat.y = (r23 + r32) / s;	
	}
	quat = norm_quat(quat);
	rot = quat_to_rot(quat);
	//print_vec(quat);
	check_tolerance(&rot);//correct - 0s
	return (rot);
}

//quat for an angle around axis xyz => q = cos(ang / 2) + (xi + yj + zk)sin(ang / 2)
//quat for rot about axis

t_quat	angle_to_quat(double angle, double x, double y, double z)
{
	t_quat	q;
	double	half_ang;
	double	sinhalf;

	half_ang = angle / 2;
	sinhalf = sin(half_ang);
	q.x = x * sinhalf;
	q.y = y * sinhalf;
	q.z = z * sinhalf;
	q.w = cos(half_ang);
	return (q);
}

t_quat	quat_mult(t_quat q1, t_quat q2)
{
	t_quat	res;

	res.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    res.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    res.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    res.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
	return (res);
}