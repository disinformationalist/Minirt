/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSG_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:54:47 by arybarsk          #+#    #+#             */
/*   Updated: 2024/12/16 12:54:50 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	filter_intersections_2(t_intersects **intersects, int i)
{
	int	j;

	j = i - 1;
	while (++j < (*intersects)->count - 1)
		(*intersects)->hits[j] = (*intersects)->hits[j + 1];
	(*intersects)->count--;
}

void	filter_intersections(t_csg *csg, t_intersects **intersects,
		bool *inl, bool *inr)
{
	bool			lhit;
	bool			is_in_left;
	bool			is_in_right;
	int				i;

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
			filter_intersections_2(intersects, i);
		else
			i++;
	}
}
