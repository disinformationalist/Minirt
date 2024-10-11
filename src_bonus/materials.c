#include "minirt.h"
//diamond, water, chrome
t_mat	get_mat(t_material material)
{
	t_mat	mat;	

	if (material == DEFAULT)
		mat = (t_mat){1.0, 0.9, 0.9, 200, .8, 0.0, 1.0};//.8 -> 0.0
	else if (material == METAL)
		mat = (t_mat){0.2, 0.3, 1.0, 128.0, 0.8, 0.0, 1.0};
	else if (material == MATTE)
		mat = (t_mat){0.2, 0.7, 0.1, 5.0, 0.05, 0.0, 1.0};
	else if (material == GLASS)
		mat = (t_mat){0.0, 0.1, 0.9, 200.0, 0.9, 1.0, 1.5};
	else if (material == MIRROR)
		mat = (t_mat){0.0, 0.0, 1.0, 1000.0, 1.0, 0.0, 1.0};
	else
		mat = (t_mat){0.1, 0.9, 0.9, 200, 0.0, 0.0, 1.0};
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
	else if (on->type == LENS)
		trace->curr_le->mat = mat;
	else
		return ;
}