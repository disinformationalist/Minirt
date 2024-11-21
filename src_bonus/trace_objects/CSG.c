#include "minirt.h"

bool	hit_allowed(t_csg_op op, bool lhit, bool inl, bool inr)
{
	if (op == UNION)
		return ((lhit && !inr) || (!lhit && !inl));
	else if (op == INTERSECTION)
		return ((lhit && inr) || (!lhit && inl));
	else if (op == DIFFERENCE)
		return ((lhit && !inr) || (!lhit && inl));
	return (false);
}

int	get_type(t_helper_shape *shape)
{
	if (shape == NULL)
		return (-1);
	return (shape->type);
}

bool	is_in_csg_left(t_csg *csg, t_intersects **intersects, int i)
{
	if ((*intersects)->hits[i].object == csg->left
		|| (*intersects)->hits[i].object == ((t_helper_shape *)csg->left)->left
		|| (*intersects)->hits[i].object == ((t_helper_shape *)csg->left)->right)
		return (1);
	return (0);
}

bool	is_in_csg_right(t_csg *csg, t_intersects **intersects, int i)
{
	if ((*intersects)->hits[i].object == csg->right
		|| (*intersects)->hits[i].object == ((t_helper_shape *)csg->right)->left
		|| (*intersects)->hits[i].object == ((t_helper_shape *)csg->right)->right)
		return (1);
	return (0);
}

void	filter_intersections(t_csg *csg, t_intersects **intersects, bool *inl, bool *inr)
{
	bool			lhit;
	int				i;
	int				j;
	
	i = 0;
	lhit = false;
	*inl = false;
	*inr = false;
	if (get_type((t_helper_shape *)csg->left) == CSG)
		filter_intersections((t_csg *)csg->left, intersects, inl, inr);
	if (get_type((t_helper_shape *)csg->right) == CSG)
		filter_intersections((t_csg *)csg->right, intersects, inl, inr);
	while (i < (*intersects)->count)
	{
		if (is_in_csg_left(csg, intersects, i))
		{
			lhit = true;
			*inl = !*inl;
		}
		if (is_in_csg_right(csg, intersects, i))
		{
			lhit = false;
			*inr = !*inr;
		}
		//if (((*intersects)->hits[i].object == csg->left || (*intersects)->hits[i].object == csg->right)
		//		&& !hit_allowed(csg->op, lhit, *inl, *inr))
		if ((is_in_csg_left(csg, intersects, i) || is_in_csg_right(csg, intersects, i))
			&& !hit_allowed(csg->op, lhit, *inl, *inr))
		{
			j = i - 1;
			while (++j < (*intersects)->count - 1)
				(*intersects)->hits[j] = (*intersects)->hits[j+1];
			(*intersects)->count--;
		}
		else
			i++;
	}
}

t_csg	*make_new_csg(t_helper_shape *left, t_helper_shape *right, t_csg_op op)
{
	t_csg	*new_csg;

	new_csg = (t_csg*)malloc(sizeof(t_csg));
	if (!new_csg)
		return (NULL); //TODO: handle malloc failure
	new_csg->type = CSG;
	new_csg->left = left;
	new_csg->right = right;
	new_csg->op = op;
	return (new_csg);
}

void	check_csg(t_helper_shape *shapes1, t_helper_shape *shapes2, t_helper_shape *shapes3, t_intersects *intersects)
{
	t_csg		*test_csg;
	t_csg		*secondary_csg;
	bool			inl;
	bool			inr;
	//(void) intersects;

	inl = false;
	inr = false;
	if (!shapes1 || !shapes2 || !shapes3)
		return ;
	test_csg = make_new_csg(shapes1, shapes2, UNION);
	secondary_csg = make_new_csg((t_helper_shape *)test_csg, shapes3, UNION);
	filter_intersections(secondary_csg, &intersects, &inl, &inr);
	free(test_csg);
	free(secondary_csg);
}
