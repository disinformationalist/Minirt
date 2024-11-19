#include "minirt.h"

t_vec3	sine_bump(t_vec3 obj_pnt, double rad, double freq, double amp)
{
	double dsdr;
	double dfdx;
	double dfdz;
	double f_r;
	double norm_fact;

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

void sine_ring_norm(t_point obj_pnt, t_comps *comps, t_plane plane)
{
	t_vec3	bumpv;
	double	amp;
	double	freq;
	double 	rad;

	amp = 0.025;
	freq = 13;
	rad = sqrt(obj_pnt.x * obj_pnt.x + obj_pnt.z * obj_pnt.z + 1e-6);
	bumpv = sine_bump(obj_pnt, rad, freq, amp);
	if (comps->inside)
		bumpv = neg(bumpv);
	comps->normal = mat_vec_mult(plane.t_transform, bumpv);
	comps->normal.w = 0;
	comps->normal = norm_vec(comps->normal);
	bumpv = add_vec(vec(obj_pnt.x, 0, obj_pnt.z, 1), bumpv);
	comps->point = mat_vec_mult(plane.i_transform, bumpv);
}

t_vec3 frost(t_vec3 norm)
{
	t_vec3	frost;
	double	strength;

	strength = .05;
	frost = vec(strength * randf2(), strength * randf2(), strength * randf2(), 0);
	norm = norm_vec(add_vec(norm, frost));
	return (norm);
}

//this one not working

/* t_vec3 honeycomb(t_vec3 obj_pnt)
{
	t_vec3	comb;
	double	sq3;

	double cell = .2;

	sq3 = sqrt(3.0);
	
	int q;
	q = ft_round((.6666667) * obj_pnt.x) / (cell * cell);
	int r;
	r = ft_round(((-.3333334) * obj_pnt.x + sq3 / 3.0) * obj_pnt.z / cell);

	double hex_x = cell * .66666667 * (double)q;
	double hex_z = cell * sq3 * ((double)r - .5 * (double)(q & 1));

	double dx = obj_pnt.x - hex_x;
	double dz = obj_pnt.z - hex_z;


	double dist = sqrt(dx * dx + dz * dz);
	double pert = .2 * exp(-dist / cell);

	comb = vec(pert * dx, 1, pert * dz, 0);


	return (norm_vec(comb));
} */


	//double	sine;
	//sine = amp * sin(freq * rad) / rad;
	//bumpv = vec(obj_pnt.x * sine, 0, obj_pnt.z * sine, 0);
	//bumpv.y = 1 - bumpv.y;
	//bumpv = norm_vec(bumpv);
	/* comps->over_pnt = add_vec(comps->point, scale_vec(1e-6, comps->normal));
	comps->under_pnt = subtract_vec(comps->point, scale_vec(1e-6, comps->normal)); */