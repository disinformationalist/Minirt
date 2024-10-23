#include "minirt.h"
/* typedef	struct s_shape
{
	void			*shape;
	t_type			type;
	t_matrix_4x4	transform;
	struct s_shape	*next;
	void			*parent;
}	t_shape;

//the group contains ll of shapes... a shape that contains all other shapes...

typedef struct s_group 
{
	t_shape			*shapes;
	t_type			type;
	t_matrix_4x4	transform;//group transform
}	t_group; */

//doubly linked circular group structure. cells? for shorthand

//create and return a new group instance

t_group *group(void)
{
	t_group *new;

	new = (t_group *)malloc(sizeof(t_group));
	if (!new)
		return (NULL);
	new->shapes = NULL;
	new->type = GROUP;
	identity(&new->transform);
	return (new);
}

//take any object primitive, place in shape container and add to group(first argument)

int	add_child(t_group *group, void *obj, t_type type, t_matrix_4x4 transform)
{
	t_shape *new;
	t_shape *last;
	t_shape *curr;

	new = (t_shape *)malloc(sizeof(t_shape));
	if (!new)
		return (1);
	new->shape = obj;
	new->type = type;
	new->parent = group;
	new->transform = transform;
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

//if i group a shape should i remove it from other list?... would have to free group objs themselves here then.. 

//free the groups shapes and the group itself.

void free_group(t_group *group)
{
	t_shape *curr;
	t_shape *temp;

	if (!group->shapes)
	{
		return free(group);
	}
	curr = group->shapes;
	group->shapes->prev->next = NULL;//next of last elem to null to break circle
	while (curr != NULL)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
	free(group);
}

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
		check_group((t_group *)curr->shape, intersects, ray);//recur!
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
		//must type cast and pass into ft based on type//if type is group recur here
		ray_group_intersect(curr, ray, intersects);
		curr = curr->next;
		if (curr == group->shapes)
			break;
	}
}

//IMPLEMENT FIND CHILD NORMAL WITHIN THE GROUP.







//maybe batch together objects by type into new lnkd list  or array, when intersect test comes, mabey...
//maybe switch to individual tesing by type...