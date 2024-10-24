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

void	filter_intersections(t_csg *csg, t_intersects **intersects)
{
	bool			inl;
	bool			inr;
	bool			lhit;
	int				i;
	int				j;

	inl = false;
	inr = false;
	i = 0;
	while (i < (*intersects)->count)
	{
		lhit = ((*intersects)->hits[i].object == csg->left);
		if (lhit)
			inl = !inl;
		if ((*intersects)->hits[i].object == csg->right)
			inr = !inr;
		if (!hit_allowed(csg->op, lhit, inl, inr))
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

t_csg	*make_new_csg(void *left, void *right, t_csg_op op)
{
	t_csg	*new_csg;

	new_csg = (t_csg*)malloc(sizeof(t_csg));
	if (!new_csg)
		return (NULL); //TODO: handle malloc failure
	new_csg->left = left;
	new_csg->right = right;
	new_csg->op = op;
	return (new_csg);
}

void	check_csg(void *shapes1, void *shapes2, t_intersects *intersects)
{
	t_csg		*test_csg;

	if (shapes1 == NULL)
		return ;
	if (shapes2 == NULL)
		return ;
	test_csg = make_new_csg(shapes1, shapes2, DIFFERENCE);
	filter_intersections(test_csg, &intersects);
	free(test_csg);
}
