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
		new = box(vec(-1, -1, -1, 0), vec(1, 1, 1, 0));
	if (type == PLANE)
		new = box(vec(-INFINITY, 0, -INFINITY, 0), vec(INFINITY, 0, INFINITY, 0));
	if (type == CYLINDER)//going to try to bound with half_h at 1..
		new = box(vec(-1, -1, -1, 0), vec(1, 1, 1, 0));
	/* if (type == CONE)//?
		new = box(vec(-1, -1, -1, 0), vec(1, 1, 1, 0)); */
	/* if (type == TRI)//easy way not opti, still gotta make these
	{
		t_tri *tri;
		tri = (t_tri *)shape;
		new = empty_box();
		if (!new)
			return (NULL);
		add_pnt(new, tri->p1);// add each vert, must change fun to accept shape *
		add_pnt(new, tri->p2);
		add_pnt(new, tri->p3);
	} */
	else
		return (NULL);
	if (!new)
		return (NULL);
	return (new);
}