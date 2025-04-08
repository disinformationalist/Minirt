#include "minirt.h"

/* bops: bounds of object in parent space. 
i_transform is normal transform in this implement, 
transform is technically inverse */

t_box	*bops(t_shape *shape)
{
	return (transform_box(bounds_of(shape->type), shape->i_tran));
}

/* 
bounds of group by combining sub-boxes
traverse the group recursively assigning and combining boxes */

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
		{
			cbox = bops(curr);
		}
		if (!cbox)
			free(box);
		if (cbox)
		{
			add_to(box, *cbox);
			if (curr->box)
				free(curr->box);
			curr->box = cbox;
		}
		curr = curr->next;
		if (curr == group->shapes)
			break;
	}
	return (box);
}

t_boxes	*boxes(t_box *left, t_box *right)
{
	t_boxes	*new;

	new = (t_boxes *)malloc(sizeof(t_boxes));
	if (!new)
		return (NULL);
	new->left = left;
	new->right = right;
	return (new);
}
