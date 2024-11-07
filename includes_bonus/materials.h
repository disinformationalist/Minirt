#ifndef MATERIALS_H
# define MATERIALS_H

typedef struct s_mat
{
	double	amb;
	double	diff;
	double	spec;
	double	shine;
	double	ref;
	double	transp;
	double	refract;
}	t_mat;

typedef enum e_material
{
	DEFAULT,
	METAL,
	MATTE,
	GLASS,
	MIRROR,
	DIAMOND,
	WATER,
	CHROME,
	AIR
}	t_material;

int	roulette_interaction(t_mat mat);


# endif