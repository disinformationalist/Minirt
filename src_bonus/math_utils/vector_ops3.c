#include "minirt.h"

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

t_vec3	reflect(t_vec3 in, t_vec3 normal)
{
	double cos_angle;

	/* in = norm_vec(in);
	normal = norm_vec(normal); */
	//in = neg(in);

	cos_angle = dot_product(in, normal);

	

	t_vec3 ref = subtract_vec(in, scale_vec(2 * cos_angle, normal));
	/* ref.x = -fabs(ref.x);
	ref.y = -fabs(ref.y);
	ref.z = -fabs(ref.z); */

	
		return (ref);
 	
}