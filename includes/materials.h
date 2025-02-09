#ifndef MATERIALS_H
# define MATERIALS_H

typedef enum e_material
{
	DEFAULT,
	METAL,
	MATTE,
	GLASS,
	MIRROR,
	DIAMOND,
	ENAMEL,
	WATER,
	CHROME,
	AIR,
	NONE
}	t_material;

typedef struct s_mat
{
	double		amb;
	double		diff;
	double		spec;
	double		shine;
	double		ref;
	double		transp;
	double		refract;
	t_material	preset;
}	t_mat;

int	roulette_interaction(t_mat mat);

#endif