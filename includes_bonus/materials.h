#ifndef MATERIALS_H
# define MATERIALS_H

// gives vals for material TOFINISH

typedef struct s_mat
{
	double	amb;
	double	diff;
	double	spec;
	double	shine;
	double	ref;
}	t_mat;

typedef enum e_material
{
	DEFAULT,
	METAL,
	MATTE,
	GLASS,
	MIRROR
}	t_material;

# endif