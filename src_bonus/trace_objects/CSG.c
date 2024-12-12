#include "minirt.h"

void	filter_intersections(t_csg *csg, t_intersects **intersects,
			bool *inl, bool *inr);

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

void	filter_child_intersections(t_csg *csg, t_intersects **intersects,
		bool *inl, bool *inr)
{
	bool			inl_left;
	bool			inl_right;
	bool			inr_left;
	bool			inr_right;

	inl_left = false;
	inl_right = false;
	inr_left = false;
	inr_right = false;
	if (get_type((t_helper_shape *)csg->left) == CSG)
		filter_intersections((t_csg *)csg->left, intersects,
			&inl_left, &inr_left);
	if (get_type((t_helper_shape *)csg->right) == CSG)
		filter_intersections((t_csg *)csg->right, intersects,
			&inl_right, &inr_right);
	if (csg->op == UNION)
	{
		*inl = inl_left || inr_left || inl_right || inr_right;
		*inr = inl_left || inr_left || inl_right || inr_right;
	}
	else
	{
		*inl = inl_left || inr_left;
		*inr = inl_right || inr_right;
	}
}

void	filter_intersections(t_csg *csg, t_intersects **intersects,
		bool *inl, bool *inr)
{
	bool			lhit;
	bool			is_in_left;
	bool			is_in_right;
	int				i;
	int				j;

	filter_child_intersections(csg, intersects, inl, inr);
	i = 0;
	lhit = false;
	while (i < (*intersects)->count)
	{
		is_in_left = is_in_csg_left(csg, intersects, i);
		is_in_right = is_in_csg_right(csg, intersects, i);
		if (is_in_left)
			set_left(&lhit, inl);
		if (is_in_right)
			set_right(&lhit, inr);
		if ((is_in_left || is_in_right)
			&& !hit_allowed(csg->op, lhit, *inl, *inr))
		{
			j = i - 1;
			while (++j < (*intersects)->count - 1)
				(*intersects)->hits[j] = (*intersects)->hits[j + 1];
			(*intersects)->count--;
		}
		else
			i++;
	}
}

t_csg	*make_new_csg(t_helper_shape *left, t_helper_shape *right, t_csg_op op)
{
	t_csg	*new_csg;

	new_csg = (t_csg *)malloc(sizeof(t_csg));
	if (!new_csg)
		return (NULL); //TODO: handle malloc failure
	new_csg->type = CSG;
	new_csg->left = left;
	new_csg->right = right;
	new_csg->op = op;
	return (new_csg);
}

void	check_csg(t_helper_shape *shapes1, t_helper_shape *shapes2,
		t_helper_shape *shapes3, t_intersects *intersects)
{
	t_csg		*test_csg;
	t_csg		*secondary_csg;
	bool		inl;
	bool		inr;

	test_csg = NULL;
	secondary_csg = NULL;
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

void	check_simple_csg(t_helper_shape *shapes1, t_helper_shape *shapes2,
			t_intersects *intersects, t_csg_op op)
{
	t_csg		*test_csg;
	bool		inl;
	bool		inr;

	test_csg = NULL;
	inl = false;
	inr = false;
	if (!shapes1 || !shapes2)
		return ;
	test_csg = make_new_csg(shapes1, shapes2, op);
	filter_intersections(test_csg, &intersects, &inl, &inr);
	free(test_csg);
}
