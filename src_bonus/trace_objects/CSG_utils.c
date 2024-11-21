#include "minirt.h"

int	get_type(t_helper_shape *shape)
{
	if (shape == NULL)
		return (-1);
	return (shape->type);
}

bool	is_in_csg_left(t_csg *csg, t_intersects **intersects, int i)
{
	t_csg	*left_csg;

	if ((*intersects)->hits[i].object == csg->left)
		return (true);
	if (get_type((t_helper_shape *)csg->left) == CSG)
	{
		left_csg = (t_csg *)csg->left;
		if (is_in_csg_left(left_csg, intersects, i) || is_in_csg_right(left_csg, intersects, i))
			return (true);
	}
	return (false);
}

bool	is_in_csg_right(t_csg *csg, t_intersects **intersects, int i)
{
	t_csg	*right_csg;

	if ((*intersects)->hits[i].object == csg->right)
		return (true);
	if (get_type((t_helper_shape *)csg->right) == CSG)
	{
		right_csg = (t_csg *)csg->right;
		if (is_in_csg_left(right_csg, intersects, i) || is_in_csg_right(right_csg, intersects, i))
			return (true);
	}
	return (false);
}

void	set_left(bool *lhit, bool *inl)
{
	*lhit = true;
	*inl = !*inl;
}

void	set_right(bool *lhit, bool *inr)
{
	*lhit = false;
	*inr = !*inr;
}