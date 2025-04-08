#include "minirt.h"

//create and return a new group instance

t_group	*group(void)
{
	t_group	*new;

	new = (t_group *)malloc(sizeof(t_group));
	if (!new)
		return (NULL);
	new->shapes = NULL;
	new->type = GROUP;
	new->box = NULL;
	new->tran = identity();
	new->count = 0;
	new->depth = 0;
	return (new);
}

/* take any object primitive, place in shape container and 
add to group(first argument) when adjusting group transform 
it must reapply to all */

int	add_child(t_group *group, void *obj, t_type type, t_mat4 tran, t_mat4 i_tran, t_box *box_in)
{
	t_shape	*new;
	t_shape	*last;

	if (type == GROUP)
	{
		new = (t_group *)obj;
		//printf("depth of added: %d\n", ((t_group *)obj)->depth);
		new->depth = ((t_group *)obj)->depth; 
	}
	else
	{
		new = (t_shape *)malloc(sizeof(t_shape));
		if (!new)
			return (1);
		new->shape = obj;

	}
	new->type = type;
	new->parent = group;
	new->tran = mat_mult(group->tran, tran);
	new->i_tran = i_tran;
	new->box = box_in;
	group->count++;
	if (group->shapes == NULL)
	{
		new->next = new;
		new->prev = new;
		group->shapes = new;
		return (0);
	}
	last = group->shapes->prev;
	new->prev = last;
	new->next = group->shapes;
	last->next = new;
	group->shapes->prev = new;
	return (0);
}

/* recursively free the groups' shapes and the groups themselves,
only for shape containers, not objects actual
if (curr->type == GROUP) // if freeing shaps also
			free_group((t_group *)curr->shape);
		else
			free(curr->shape); */

void	free_group(t_group *group)
{
	t_shape	*curr;
	t_shape	*temp;

	if (!group)
		return ;
	if (!group->shapes)
		return (free(group));
	curr = group->shapes;
	group->shapes->prev->next = NULL;
	while (curr != NULL)
	{
		temp = curr->next;
		if (curr->type == GROUP)
			free_group((t_group *)curr);
		else
		{
			if (curr->box)
				free(curr->box);
			free(curr);
		}
		curr = temp;
	}
	if (group->box)
		free(group->box);
	free(group);
}

//the next 2 fts recursively tests intersections down through group
//area lts not yet handled

void	ray_group_intersect(t_shape *curr, t_ray ray, t_intersects *intersects)
{
	if (curr->type == SPHERE)
		ray_sphere_intersect((t_sphere *)curr->shape, ray, intersects);
	else if (curr->type == PLANE)
		ray_plane_intersect((t_plane *)curr->shape, ray, intersects);
	else if (curr->type == CYLINDER)
		ray_cylinder_intersect((t_cylinder *)curr->shape, ray, intersects);
	else if (curr->type == HYPERBOLOID)
		ray_hype_intersect((t_hyperboloid *)curr->shape, ray, intersects, ((t_hyperboloid *)curr->shape)->single);
	else if (curr->type == CUBE)
		ray_cube_intersect((t_cube *)curr->shape, ray, intersects);
	else if (curr->type == GROUP)
		check_group((t_group *)curr->shape, intersects, ray);
	else
		return ;
}

//function to check the intersections of a group

void	check_group(t_group *group, t_intersects *intersects, t_ray ray)
{
	t_shape	*curr;

	if (!group->shapes)
		return ;
	ray = transform(ray, group->tran);
	curr = group->shapes;
	while (true)
	{
		ray_group_intersect(curr, ray, intersects);
		curr = curr->next;
		if (curr == group->shapes)
			break ;
	}
}

int	append_shape_copy(t_group *in, t_shape *s)
{
	t_shape *new;
	t_shape *last;

	new = (t_shape *)malloc(sizeof(t_shape));
	if (!new)
		return (1);
	new->type = s->type;
	new->tran = s->tran;
	if (s->type != GROUP)
	{
		new->shape = s->shape;
		new->prev = s->prev;
		new->next = s->next;
		new->i_tran = s->i_tran;
	}
	new->box = NULL;
	if (in->shapes == NULL)
	{
		new->next = new;
		new->prev = new;
		in->shapes = new;
		return (0);
	}
	last = in->shapes->prev;
	new->prev = last;
	new->next = in->shapes;
	last->next = new;
	in->shapes->prev = new;
	return (0);
}

t_group *copy_group(t_group *in)
{
	t_group *copy;
	t_shape *curr;

	if (!in || !in->shapes)
		return (NULL);
	copy = group();
	copy->count = in->count;
	copy->depth = in->depth;
	if (!copy)
		return (NULL);
	curr = in->shapes;
	while (true)
	{		
		if (curr->type == GROUP)
			append_shape_copy(copy, copy_group((t_group *)curr));
		else
			append_shape_copy(copy, (t_shape *)curr);
		curr = curr->next;
		if (curr == in->shapes)
			break ;
	}
	return (copy);
}
