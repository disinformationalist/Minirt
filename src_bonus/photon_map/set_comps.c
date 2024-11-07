#include "minirt.h"

t_vec3	sp_norm_at(t_point int_pnt, t_matrix_4x4 transform)
{
	t_vec3	norm;
	t_point	obj_pnt;

	obj_pnt = mat_vec_mult(transform, int_pnt);
	norm = mat_vec_mult(transpose(transform), obj_pnt);
	norm.w = 0;
	return (norm_vec(norm));
}

t_vec3 cyl_norm_at(t_point int_pnt, t_matrix_4x4 transform, double half_h)
{
	t_vec3	norm;
	double	dist;

	int_pnt = mat_vec_mult(transform, int_pnt);
	dist = int_pnt.x * int_pnt.x + int_pnt.z * int_pnt.z;
	if (dist < .99999 && int_pnt.y >= half_h - 1e-5)
		norm = vec(0, 1, 0, 0);
	else if (dist < .99999 && int_pnt.y <= -half_h + 1e-5)
		norm = vec(0, -1, 0, 0);
	else
		norm = vec(int_pnt.x, 0, int_pnt.z, 0);
	norm = mat_vec_mult(transpose(transform), norm);
	norm.w = 0;
	return (norm_vec(norm));
}

t_vec3	cu_norm_at(t_point int_pnt, t_matrix_4x4 transform)
{
	t_vec3	norm;
	double	max;
	double	absx;
	double	absy;
	double	absz;

	int_pnt = mat_vec_mult(transform, int_pnt);
	absx = fabs(int_pnt.x);
	absy = fabs(int_pnt.y);
	absz = fabs(int_pnt.z);
	max = fmax(fmax(absx, absy), absz);
	if (max == absx)
		norm = vec(int_pnt.x, 0, 0, 0);
	else if (max == absy)
		norm = vec(0, int_pnt.y, 0, 0);
	else if (max == absz)
		norm = vec(0, 0, int_pnt.z, 0);
	else
		norm = vec(0, 1, 0, 0);
	norm = mat_vec_mult(transpose(transform), norm);
	norm.w = 0;
	return (norm_vec(norm));
}

void	set_comps2(t_track_hits *closest, t_pcomps *c)
{
	t_cylinder	*cyl;

	if (closest->object_type == CYLINDER)
	{
		cyl = (t_cylinder *)closest->object;
		c->mat = cyl->mat;
		c->norm = cyl_norm_at(c->pnt, cyl->transform, cyl->half_h);
	}
	else if (closest->object_type == CUBE)
	{
		c->mat = ((t_cube *)closest->object)->mat;
		c->norm = cu_norm_at(c->pnt, ((t_cube *)closest->object)->transform);
	}	
	else if (closest->object_type == TRI)
	{
		c->mat = ((t_tri *)closest->object)->mat;
		c->norm = ((t_tri *)closest->object)->norm;
	}
}

t_pcomps	set_comps(t_track_hits *closest, t_ray pho)
{
	t_pcomps c;
	//bool in;

	c.pnt = add_vec(pho.origin, scale_vec(closest->t, pho.dir));
	if (closest->object_type == SPHERE)
	{
		c.mat = ((t_sphere *)closest->object)->mat;
		c.norm = sp_norm_at(c.pnt, ((t_sphere *)closest->object)->transform);
	}
	else if (closest->object_type == PLANE)
	{
		c.mat = ((t_plane *)closest->object)->mat;
		c.norm = ((t_plane *)closest->object)->norm;
	}
	if (dot_product(c.norm, pho.dir) > 0)
	{
		c.norm = neg(c.norm);
		//in = true;
	}
	set_comps2(closest, &c);
	c.interact = roulette_interaction(c.mat);
 	/* if (in)
		c.over_start = subtract_vec(c.pnt, scale_vec(1e-6, c.norm));
	else
		c.over_start = add_vec(c.pnt, scale_vec(1e-6, c.norm)); */
	return (c);
}

