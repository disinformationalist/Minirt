#include "minirt.h"

double	det2(t_mat2 m)
{
	return (m.mat[0] * m.mat[3] - m.mat[2] * m.mat[1]);
}

//submatrix of a 3 by 3, removes row, col

t_mat2	submat3(t_mat3 m, int row, int col)
{
	t_mat2	sub;
	int		i;
	int		j;
	int		subi;
	int		subj;

	i = -1;
	subi = 0;
	while (++i < 3)
	{
		if (i == row)
			continue ;
		j = -1;
		subj = 0;
		while (++j < 3)
		{
			if (j == col)
				continue ;
			sub.mat[subi + subj * 2] = m.mat[i + j * 3];
			subj++;
		}
		subi++;
	}
	return (sub);
}

//given a 3 by 3 matrix, returns determinate of the submatrix at row, col

double	minor3(t_mat3 m, int row, int col)
{
	return (det2(submat3(m, row, col)));
}

//cofact of 3 by 3, is a minor with sign change depending on position

double	cofact3(t_mat3 m ,int row, int col)
{
	double	co;

	co = minor3(m, row, col);
	if ((row + col) % 2 == 1)
		co = -co;
	return (co);
}

t_mat3	submat4(t_mat4 m, int row, int col)
{
	t_mat3	sub;
	int		i;
	int		j;
	int		subi;
	int		subj;

	i = -1;
	subi = 0;
	while (++i < 4)
	{
		if (i == row)
			continue ;
		j = -1;
		subj = 0;
		while (++j < 4)
		{
			if (j == col)
				continue ;
			sub.mat[subi + subj * 3] = m.mat[i + j * 4];
			subj++;
		}
		subi++;
	}
	return (sub);
}