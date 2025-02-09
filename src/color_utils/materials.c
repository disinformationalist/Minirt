#include "minirt.h"

//properties: {amb, diff, spec, shine, reflect, transp, refract}
//mat = (t_mat){0.02, 0.05, 0.9, 200.0, 0.2, 0.8, 1.52};

t_mat	get_mat(t_material material)
{
	t_mat	mat;

	if (material == DEFAULT)
		mat = (t_mat){0.8, 0.9, 0.9, 200.0, 0.0, 0.0, 1.0, 0};
	else if (material == METAL)
		mat = (t_mat){0.2, 0.8, 1.0, 128.0, 0.8, 0.0, 1.0, 1};
	else if (material == MATTE)
		mat = (t_mat){0.2, 0.7, 0.1, 5.0, 0.05, 0.0, 1.0, 2};
	else if (material == GLASS)
		mat = (t_mat){0.02, 0.05, 0.9, 200.0, 0.3, 0.8, 1.52, 3};
	else if (material == MIRROR)
		mat = (t_mat){0.0, 0.0, 1.0, 1000.0, 1.0, 0.0, 1.0, 4};
	else if (material == DIAMOND)
		mat = (t_mat){0.0, 0.1, 1.0, 300.0, 0.5, 0.9, 2.417, 5};
	else if (material == ENAMEL)
		mat = (t_mat){0.69, 0.9, 0.46, 7.0, 0.09, 0.0, 1.0, 6};
	else if (material == WATER)
		mat = (t_mat){0.1, 0.1, 0.5, 40.0, 0.2, 0.8, 1.33, 7};
	else if (material == CHROME)
		mat = (t_mat){.25, 0.4, 0.9, 250.0, 1.0, 0.0, 0.95, 8};
	else if (material == AIR)
		mat = (t_mat){0.1, 0.2, 0.8, 200.0, 0.8, .7, 1.001, 9};
	else
		mat = (t_mat){0.8, 0.9, 0.9, 200, 0.0, 0.0, 1.0, 0};
	return (mat);
}

//pass in get_mat(material) to change the material of the current object

void	change_mat(t_trace *trace, t_on *on, const t_mat mat)
{
	if (on->object == NULL)
		return ;
	if (on->type == SPHERE)
		trace->curr_sp->mat = mat;
	else if (on->type == PLANE)
		trace->curr_pl->mat = mat;
	else if (on->type == CYLINDER)
		trace->curr_cy->mat = mat;
	else if (on->type == CUBE)
		trace->curr_cu->mat = mat;
	else if (on->type == HYPERBOLOID)
		trace->curr_hy->mat = mat;
	else
		return ;
}

//used in photon mapping to select action

int	roulette_interaction(t_mat mat)
{
	double	random;

	random = randf();
	if ((double)random < mat.diff)
		return (1);
	else if ((double)random < mat.diff + mat.spec)
		return (2);
	else
		return (0);
}
