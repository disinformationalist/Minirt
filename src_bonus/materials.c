#include "minirt.h"
/* 	t_mat	mat;	

	if (material == DEFAULT)
		mat = (t_mat){1.0, 0.9, 0.9, 200, 0.0, 0.0, 1.0};//.8 -> 0.0
	else if (material == METAL)
		mat = (t_mat){0.2, 0.3, 1.0, 128.0, 0.8, 0.0, 1.0};
	else if (material == MATTE)
		mat = (t_mat){0.2, 0.7, 0.1, 5.0, 0.05, 0.0, 1.0};
	else if (material == GLASS)
		mat = (t_mat){0.1, 0.2, 0.9, 200.0, 0.8, 1.0, 1.52};
	else if (material == MIRROR)
		mat = (t_mat){0.0, 0.0, 1.0, 1000.0, 1.0, 0.0, 1.0};
	else if (material == DIAMOND)
		mat = (t_mat){0.0, 0.1, 1.0, 300.0, 0.9, 2.42, 0.9}; // Diamond: high specularity and reflectance, high IOR
	else if (material == WATER)
		mat = (t_mat){0.0, 0.3, 0.7, 50.0, 0.5, 1.33, 0.9};  // Water: moderate reflectance, lower IOR
	else if (material == CHROME)
		mat = (t_mat){0.25, 0.4, 0.9, 250.0, 1.0, 0.0, 0.95}; // Chrom
	else if (material == AIR)
		mat = (t_mat){0.1, 0.2, 0.8, 200, 0.8, 1.0, 1.0}; 
	else
		mat = (t_mat){0.1, 0.9, 0.9, 200, 0.0, 0.0, 1.0};
	return (mat);*/

	/* 	double	amb;
	double	diff;
	double	diffref;
	double	spec;
	double	shine;
	double	ref;
	double	transp;
	double	refract; */
	

t_mat	get_mat(t_material material)//diffuse_ref?  absorb?
{
	t_mat mat;    
		//{amb, diff, spec, shine, reflect, transp, refract}
    if (material == DEFAULT)
        mat = (t_mat){0.8, 0.9, 0.9, 200.0, 0.0, 0.0, 1.0}; // Balanced diffuse, minimal specular
    else if (material == METAL)
        mat = (t_mat){0.2, 0.3, 1.0, 128.0, 0.8, 0.0, 1.0}; // High specular, reflective
    else if (material == MATTE)
        mat = (t_mat){0.2, 0.7, 0.1, 5.0, 0.05, 0.0, 1.0}; // Mostly diffuse, very low specular
    else if (material == GLASS)//0.1, 0.2, 0.9, 200.0, 0.8, 1.0, 1.52}mat = (t_mat){0.1, 0.2, 0.9, 200.0, 0.8, 1.0, 1.52};
        mat = (t_mat){0.1, 0.2, 0.9, 200.0, 0.1, 1.0, 1.52}; // Low diffuse/spec, mostly transparent
    else if (material == MIRROR)
        mat = (t_mat){0.0, 0.0, 1.0, 1000.0, 1.0, 0.0, 1.0}; // Pure reflection
    else if (material == DIAMOND)
        mat = (t_mat){0.0, 0.1, 1.0, 300.0, 0.5, 0.9, 2.417}; // High refraction, partial reflection
    else if (material == WATER)
        mat = (t_mat){0.0, 0.3, 0.7, 50.0, 0.5, 0.9, 1.33}; // Mostly transparent with slight reflection
    else if (material == CHROME)
        mat = (t_mat){.25, 0.4, 0.9, 250.0, 1.0, 0.0, 0.95}; // High specular, reflective
    else if (material == AIR)
        mat = (t_mat){0.1, 0.2, 0.8, 100.0, 0.8, 1.0, 1.0}; // Full transparency, refractive index ~1.0
    else
        mat = (t_mat){0.1, 0.9, 0.9, 200, 0.0, 0.0, 1.0};
    return mat;
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
	else if (on->type == CUBE)
		trace->curr_cu->mat = mat;
	else
		return ;
}

int	roulette_interaction(t_mat mat)
{
	double random;
	//random = drand48();
	random = randf();
	if ((double)random < mat.diff)//add the pho
		return (1);//diff
	else if ((double)random < mat.diff + mat.spec)
		return (2);// spec
	else
		return (0);//absorbed.
}
	/* else if ((double)random <= mat.diff + mat.spec + mat.transp)
		return (3);//trans */
