#include "minirt.h"

//in col major all mod 5 elemets are the 1's

t_mat4 identity(void)
{
	t_mat4	id;
	int		i;

	i = -1;
	while (++i < 16)
	{
		if (i % 5 == 0)
			id.mat[i] = 1.0; 
		else
			id.mat[i] = 0.0;
	}
	return (id);
}

t_mat4	transpose(t_mat4 m)
{
	t_mat4	tp;
	int		i;
	int		j;
	int		id1;
	int		id2;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			id1 = i + j * 4;
			id2 = j + i * 4;
			tp.mat[id2] = m.mat[id1];
		}
	}
	return (tp);
}

t_mat4	rot_x(double rad)
{
	t_mat4 rot;

	rot = identity();
	rot.mat[5] = cos(rad);
	rot.mat[6] = sin(rad);
	rot.mat[9] = -sin(rad);
	rot.mat[10] = cos(rad);
	return (rot);
}

t_mat4	rot_y(double rad)
{
	t_mat4 rot;

	rot = identity();
	rot.mat[0] = cos(rad);
	rot.mat[2] = -sin(rad);
	rot.mat[8] = sin(rad);
	rot.mat[10] = cos(rad);
	return (rot);
}

t_mat4	rot_z(double rad)
{
	t_mat4 rot;

	rot = identity();
	rot.mat[0] = cos(rad);
	rot.mat[1] = sin(rad);
	rot.mat[4] = -sin(rad);
	rot.mat[5] = cos(rad);
	return (rot);
}
