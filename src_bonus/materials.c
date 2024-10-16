#include "minirt.h"
//diamond, water, chrome
t_mat	get_mat(t_material material)
{
	t_mat	mat;	

	if (material == DEFAULT)
		mat = (t_mat){1.0, 0.9, 0.9, 200, 0.0, 0.0, 1.0};//.8 -> 0.0
	else if (material == METAL)
		mat = (t_mat){0.2, 0.3, 1.0, 128.0, 0.8, 0.0, 1.0};
	else if (material == MATTE)
		mat = (t_mat){0.2, 0.7, 0.1, 5.0, 0.05, 0.0, 1.0};
	else if (material == GLASS)
		mat = (t_mat){0.8, 0.2, 0.9, 200.0, 0.8, 1.0, 1.52}; //increase reflect later
	else if (material == MIRROR)
		mat = (t_mat){0.0, 0.0, 1.0, 1000.0, 1.0, 0.0, 1.0};
	else if (material == DIAMOND)
		mat = (t_mat){0.0, 0.1, 1.0, 300.0, 0.9, 2.42, 0.9}; // Diamond: high specularity and reflectance, high IOR
	else if (material == WATER)
		mat = (t_mat){0.0, 0.3, 0.7, 50.0, 0.5, 1.33, 0.9};  // Water: moderate reflectance, lower IOR
	else if (material == CHROME)
		mat = (t_mat){0.25, 0.4, 0.9, 250.0, 1.0, 0.0, 0.95}; // Chrom
	else if (material == AIR)
		mat = (t_mat){0.01, 0.0, 0.0, 0.0, 0.01, 1.0, 1.0}; 
	else
		mat = (t_mat){0.1, 0.9, 0.9, 200, 0.0, 0.0, 1.0};
	return (mat);

	/* double	amb;
	double	diff;
	double	spec;
	double	shine;

	double	ref;//reflect
	double	transp;
	double	refract; */
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
	else if (on->type == LENS)
		trace->curr_le->mat = mat;
	else
		return ;
}