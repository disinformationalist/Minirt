#include "minirt.h"

t_vec3	sine_bump(t_vec3 obj_pnt, double rad, double freq, double amp)
{
	double	dsdr;
	double	dfdx;
	double	dfdz;
	double	f_r;
	double	norm_fact;

	f_r = freq * rad;
	dsdr = amp * ((f_r * cos(f_r) - sin(f_r)) / (rad * rad));
	dfdx = dsdr * (obj_pnt.x / rad);
	dfdz = dsdr * (obj_pnt.z / rad);
	norm_fact = 1.0 / sqrt(dfdx * dfdx + dfdz * dfdz + 1.0);
	dfdx = -dfdx * norm_fact;
	dfdz = -dfdz * norm_fact;
	return (vec(dfdx, norm_fact, dfdz, 0));
}

//only on planes, makes ripples

void	sine_ring_norm(t_point obj_pnt, t_comps *comps, \
t_mat4 t_tran, t_mat4 i_tran)
{
	t_vec3	bumpv;
	t_vec3	bumpp;
	double	amp;
	double	freq;
	double	rad;

	amp = 0.025;
	freq = 13;
	rad = sqrt(obj_pnt.x * obj_pnt.x + obj_pnt.z * obj_pnt.z + 1e-6);
	bumpv = sine_bump(obj_pnt, rad, freq, amp);
	bumpp = bumpv;
	bumpv.y += 2;
	if (comps->inside)
		bumpv = neg(bumpv);
	comps->normal = mat_vec_mult(t_tran, bumpv);
	comps->normal.w = 0;
	comps->normal = norm_vec(comps->normal);
	bumpp = add_vec(vec(obj_pnt.x, 0, obj_pnt.z, 1), bumpp);
	comps->point = mat_vec_mult(i_tran, bumpp);
}

t_vec3	frost(t_vec3 norm)
{
	t_vec3	frost;
	double	strength;

	strength = .04;
	frost = vec(strength * randf2(), \
		strength * randf2(), strength * randf2(), 0);
	norm = norm_vec(add_vec(norm, frost));
	return (norm);
}
