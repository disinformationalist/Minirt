#include "minirt.h"

static inline t_vec3	sine_bump_dirs(t_vec2 dirs, double rad, \
double freq, double amp)
{
	double	dsdr;
	double	dfdx;
	double	dfdy;
	double	f_r;
	double	norm_fact;

	f_r = freq * rad;
	dsdr = amp * ((f_r * cos(f_r) - sin(f_r)) / (rad * rad));
	dfdx = dsdr * (dirs.x / rad);
	dfdy = dsdr * (dirs.y / rad);
	norm_fact = 1.0 / sqrt(dfdx * dfdx + dfdy * dfdy + 1.0);
	dfdx = -dfdx * norm_fact;
	dfdy = -dfdy * norm_fact;
	return (vec(dfdx, norm_fact, dfdy, 0));
}

static inline void	set_dirs(t_vec2 *dirs, t_face face, t_point obj_pnt)
{
	if (face == UP || face == DOWN)
	{
		dirs->x = obj_pnt.x;
		dirs->y = obj_pnt.z;
	}
	else if (face == FRONT || face == BACK)
	{
		dirs->x = obj_pnt.x;
		dirs->y = obj_pnt.y;
	}
	else if (face == LEFT || face == RIGHT)
	{
		dirs->x = obj_pnt.y;
		dirs->y = obj_pnt.z;
	}
}

void	adjust_bumpv(t_vec3 *bumpv, t_face face)
{
	if (face == UP)
		*bumpv = *bumpv;
	else if (face == DOWN)
		*bumpv = vec(bumpv->x, -bumpv->y, bumpv->z, 0);
	else if (face == LEFT)
		*bumpv = vec(-bumpv->y, bumpv->x, bumpv->z, 0);
	else if (face == RIGHT)
		*bumpv = vec(bumpv->y, bumpv->x, bumpv->z, 0);
	else if (face == FRONT)
		*bumpv = vec(bumpv->x, bumpv->z, -bumpv->y, 0);
	else
		*bumpv = vec(bumpv->x, bumpv->z, bumpv->y, 0);
}

void	adjust_bumpp(t_vec3 *bumpp, t_face face, t_vec3 obj_pnt)
{
	if (face == UP || face == DOWN)
		*bumpp = add_vec(vec(obj_pnt.x, 0, obj_pnt.z, 1), *bumpp);
	else if (face == LEFT || face == RIGHT)
		*bumpp = add_vec(vec(0, obj_pnt.y, obj_pnt.z, 1), *bumpp);
	else if (face == FRONT || face == BACK)
		*bumpp = add_vec(vec(obj_pnt.x, obj_pnt.y, 0, 1), *bumpp);
	else
		return ;
}

void	sine_ring_norm_cu(t_point obj_pnt, t_comps *comps, \
t_mat4 t_tran, t_mat4 i_tran, double amp)
{
	t_vec3	bumpv;
	t_vec3	bumpp;
	double	freq;
	double	rad;

	freq = 13;
	set_dirs(&comps->dirs, comps->face, obj_pnt);
	rad = sqrt(comps->dirs.x * comps->dirs.x + comps->dirs.y \
	* comps->dirs.y + 1e-6);
	bumpv = sine_bump_dirs(comps->dirs, rad, freq, amp);
	adjust_bumpv(&bumpv, comps->face);
	bumpp = bumpv;
	if (comps->inside)
	{
		bumpv = neg(bumpv);
		bumpp = neg(bumpp);
	}
	comps->normal = mat_vec_mult(t_tran, bumpv);
	comps->normal.w = 0;
	comps->normal = norm_vec(comps->normal);
	adjust_bumpp(&bumpp, comps->face, obj_pnt);
	comps->point = mat_vec_mult(i_tran, bumpp);
}
