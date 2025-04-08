#include "minirt.h"

t_box	*box(t_point min, t_point max)
{
	t_box	*new;

	new = (t_box *)malloc(sizeof(t_box));
	if (!new)
		return (NULL);
	new->min = min;
	new->max = max;
	return (new);
}

t_box	*empty_box(void)
{
	t_box	*new;

	new = (t_box *)malloc(sizeof(t_box));
	if (!new)
		return (NULL);
	new->min = vec(INFINITY, INFINITY, INFINITY, 1);
	new->max = vec(-INFINITY, -INFINITY, -INFINITY, 1);
	return (new);
}

//add new point to box by adjusting mins and maxes

void	add_pnt(t_box *box, t_point to_add)
{
	if (to_add.x < box->min.x)
		box->min.x = to_add.x;
	if (to_add.y < box->min.y)
		box->min.y = to_add.y;
	if (to_add.z < box->min.z)
		box->min.z = to_add.z;
	if (to_add.x > box->max.x)
		box->max.x = to_add.x;
	if (to_add.y > box->max.y)
		box->max.y = to_add.y;
	if (to_add.z > box->max.z)
		box->max.z = to_add.z;
}

//instantiate box with limits for a given shape type

t_box	*bounds_of(t_type type)
{
	t_box	*new;

	if (type == SPHERE || type == CUBE)
		new = box(vec(-1, -1, -1, 1), vec(1, 1, 1, 1));
	else if (type == CYLINDER || type == HYPERBOLOID)
		new = box(vec(-1, -1, -1, 1), vec(1, 1, 1, 1));
	else if (type == PLANE)
		new = box(vec(-INFINITY, 0, -INFINITY, 1), \
		vec(INFINITY, 0, INFINITY, 0));
	else
		return (NULL);
	if (!new)
		return (NULL);
	return (new);
}
