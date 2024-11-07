#include "minirt.h"

// adds b2 to b1 (b1 expands to contain b2)

void	add_to(t_box *b1, t_box b2)
{
	add_pnt(b1, b2.min);
	add_pnt(b1, b2.max);
}

//true if point is within box

bool	contains_pnt(t_box box, t_point point)
{
	bool in_x;
	bool in_y;
	bool in_z;

	in_x = point.x >= box.min.x && point.x <= box.max.x;
	in_y = point.y >= box.min.y && point.y <= box.max.y;
	in_z = point.z >= box.min.z && point.z <= box.max.z;
	return (in_x && in_y && in_z);
}

//true if b1 contains b2

bool	contains_box(t_box b1, t_box b2)
{
	return (contains_pnt(b1, b2.min) && contains_pnt(b1, b2.max));
}

//return a new box with a transform applied to an existing one

t_box	*transform_box(t_box *box, t_matrix_4x4 transf)
{
	t_point point;
	t_box	*new;

	if (!box)
		return (NULL);
	new = empty_box();
	if (!new)
		return (NULL);
	point = box->min;
	add_pnt(new, mat_vec_mult(transf, point));
	point = vec(box->min.x, box->min.y, box->max.z, 1);
	add_pnt(new, mat_vec_mult(transf, point));
	point = vec(box->min.x, box->max.y, box->min.z, 1);
	add_pnt(new, mat_vec_mult(transf, point));
	point = vec(box->min.x, box->max.y, box->max.z, 1);
	add_pnt(new, mat_vec_mult(transf, point));
	point = vec(box->max.x, box->min.y, box->min.z, 1);
	add_pnt(new, mat_vec_mult(transf, point));
	point = vec(box->max.x, box->min.y, box->max.z, 1);
	add_pnt(new, mat_vec_mult(transf, point));
	point = vec(box->max.x, box->max.y, box->min.z, 1);
	add_pnt(new, mat_vec_mult(transf, point));
	point = box->max;
	add_pnt(new, mat_vec_mult(transf, point));
	return (new);
}

//bops: bounds of object in parent space. 
//TRANSFORM NEEDS TO BE THE REGULAR, NOT THE INVERSE AS IS CURRENTLY SET, add these to prims in set transforms
t_box *bops(t_shape *shape)//ret null on fail
{
	return (transform_box(bounds_of(shape->type), shape->transform));
}

//in progress 
//bounds of group by combining sub-boxen
//traverse the group recursively combining boxes
t_box *bog(t_group *group)//must test....this......
{
	t_shape	*curr;
	t_box	*box;
	t_box	*cbox;

	if (!group->shapes)
		return (NULL);
	box = empty_box();
	if (!box)
		return (NULL);
	curr = group->shapes;
	while (true)
	{
		if (curr->type == GROUP)
			cbox = bog((t_group *)curr->shape);//pro
		else
			cbox = bops(curr);//pro
		if (!cbox)
			free(box);
		if (cbox)
		{
			add_to(box, *cbox);
			free(cbox);
		}
		curr = curr->next;
		if (curr == group->shapes)
			break;
	}
	return (box);
}
