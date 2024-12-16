/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSG.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:54:17 by arybarsk          #+#    #+#             */
/*   Updated: 2024/12/16 12:54:30 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_csg	*make_new_csg(t_helper_shape *left, t_helper_shape *right, t_csg_op op)
{
	t_csg	*new_csg;

	new_csg = (t_csg *)malloc(sizeof(t_csg));
	if (!new_csg)
		return (NULL);
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
