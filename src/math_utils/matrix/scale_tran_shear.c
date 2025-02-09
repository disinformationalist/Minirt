#include "minirt.h"

t_mat4	translation(double x, double y, double z)
{
	t_mat4	m;

	m = identity();
	m.mat[12] = x;
	m.mat[13] = y;
	m.mat[14] = z;
	return (m);
}

t_mat4	scaling(double x, double y, double z)
{
	t_mat4	m;

	m = identity();
	m.mat[0] = x;
	m.mat[5] = y;
	m.mat[10] = z;
	m.mat[15] = 1;
	return (m);
}

t_mat4	inv_scaling(double x, double y, double z)
{
	t_mat4	m;

	m = identity();
	m.mat[0] = 1.0 / x;
	m.mat[5] = 1.0 / y;
	m.mat[10] = 1.0 / z;
	m.mat[15] = 1;
	return (m);
}

/* shearing
x moved in proportion to y
x in proportion to z
y in proportion to x
y in proportion to z
z in proportion to x
z in proportion to y */

t_mat4 shearing(double xy, double xz, double yx, double yz, double zx, double zy)
{
	t_mat4	shear;

	shear = identity();
	shear.mat[1] = yx;
	shear.mat[2] = zx;
	shear.mat[4] = xy;
	shear.mat[6] = zy;
	shear.mat[8] = xz;
	shear.mat[9] = yz;
	return (shear);
}

void	tuple_to_col(t_mat4 *m, t_vec3 tuple, int col)
{
	int	shift;

	shift = 4 * col;
	m->mat[shift] = tuple.x;
	m->mat[shift + 1] = tuple.y;
	m->mat[shift + 2] = tuple.z;
	m->mat[shift + 3] = tuple.w;
}
