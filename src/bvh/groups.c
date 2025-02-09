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
	new->transform = identity();
	return (new);
}

/* take any object primitive, place in shape container and 
add to group(first argument) when adjusting group transform 
it must reapply to all */

int	add_child(t_group *group, void *obj, t_type type, t_mat4 transform)
{
	t_shape	*new;
	t_shape	*last;

	new = (t_shape *)malloc(sizeof(t_shape));
	if (!new)
		return (1);
	new->shape = obj;
	new->type = type;
	new->parent = group;
	new->transform = mat_mult(group->transform, transform);
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
if (curr->type == GROUP)
			free_group((t_group *)curr->shape);
		else
			free(curr->shape); */

void	free_group(t_group *group)
{
	t_shape	*curr;
	t_shape	*temp;

	if (!group->shapes)
		return (free(group));
	curr = group->shapes;
	group->shapes->prev->next = NULL;
	while (curr != NULL)
	{
		temp = curr->next;
		if (curr->type == GROUP)
			free_group((t_group *)curr->shape);
		free(curr);
		curr = temp;
	}
	free(group);
}

//the next 2 fts recursively tests intersections down through group

void	ray_group_intersect(t_shape *curr, t_ray ray, t_intersects *intersects)
{
	if (curr->type == SPHERE)
		ray_sphere_intersect((t_sphere *)curr->shape, ray, intersects);
	else if (curr->type == PLANE)
		ray_plane_intersect((t_plane *)curr->shape, ray, intersects);
	else if (curr->type == CYLINDER)
		ray_cylinder_intersect((t_cylinder *)curr->shape, ray, intersects);
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
	ray = transform(ray, group->transform);
	curr = group->shapes;
	while (true)
	{
		ray_group_intersect(curr, ray, intersects);
		curr = curr->next;
		if (curr == group->shapes)
			break ;
	}
}
