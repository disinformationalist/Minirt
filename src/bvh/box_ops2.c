#include "minirt.h"

/* bops: bounds of object in parent space. 
TRANSFORM NEEDS TO BE THE REGULAR, NOT THE INVERSE AS IS CURRENTLY SET, 
add these to prims in set transforms
ret null on fail */

t_box	*bops(t_shape *shape)
{
	return (transform_box(bounds_of(shape->type), shape->transform));
}

/* in progress 
bounds of group by combining sub-boxes
traverse the group recursively combining boxes
must test , protect functions
t_box *bog(t_group *group)
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
			cbox = bog((t_group *)curr->shape);
		else
			cbox = bops(curr);
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
} */