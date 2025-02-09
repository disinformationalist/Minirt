#include "minirt.h"


//det3 and 4 expand on the first column. seems more natchy with col major order

double	det3(t_mat3 m)
{
	return (m.mat[0] * cofact3(m, 0, 0) + 
			m.mat[1] * cofact3(m, 1, 0) + 
			m.mat[2] * cofact3(m, 2, 0));
}

double	minor4(t_mat4 m, int row, int col)
{
	return (det3(submat4(m, row, col)));
}

double	cofact4(t_mat4 m ,int row, int col)
{
	double	co;

	co = minor4(m, row, col);
	if ((row + col) % 2 == 1)
		co = -co;
	return (co);
}

double	det4(t_mat4 m)
{
	return (m.mat[0] * cofact4(m, 0, 0) + 
			m.mat[1] * cofact4(m, 1, 0) + 
			m.mat[2] * cofact4(m, 2, 0) +
			m.mat[3] * cofact4(m, 3, 0));
}

t_mat4		inverse(t_mat4 m)
{
	int		i;
	int		j;
	double	det;
	t_mat4	inv;

	det = det4(m);
	if (fabs(det) < 1e-9)
	{
		i = -1;
		while (++i < 16)
			inv.mat[i] = 0.0;
		printf("Warning: non-invertable matrix, null matrix returned\n");
		return (inv);
	}
	i = -1;
	while(++i < 4)
	{
		j = -1;
		while (++j < 4)
			inv.mat[i * 4  + j] = cofact4(m, i, j) / det;
	}
	return (inv);
}