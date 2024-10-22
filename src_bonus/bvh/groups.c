#include "minirt.h"

/* typedef	struct s_shape
{
	void			*shape;
	void			*parent;
	t_type			type;
	t_matrix_4x4	transform;
}	t_shape;

typedef struct s_group
{
	t_shape			**shapes;//pntrs to objs, or groups
	int				count;
	t_matrix_4x4	transform;
}	t_group; */

//take any object primitive, place in shape container and add to group

void	add_child(t_group *group, void *obj, t_type type, t_matrix_4x4 transform)
{
	t_shape *new;

	new = (t_shape * )malloc(sizeof(t_shape));
	if (!new)
	{
		// perform full free of groups and other frees, exit;
		exit(1);//placeholder
	}
	new->shape = obj;
	new->type = type;
	new->parent = group;
	new->transform = transform;//not sure if i need this as shape will have it
	group->shapes[group->count] = new;//remeber to init, group, group count
	//make group->shapes an ll and add shape


}


//maybe batch together objects by type into new lnkd list  or array, when intersect test comes, mabey...
//maybe switch to individual tesing by type...