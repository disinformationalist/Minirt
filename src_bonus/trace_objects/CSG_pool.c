/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSG_pool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:55:09 by arybarsk          #+#    #+#             */
/*   Updated: 2024/12/16 12:55:14 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	check_csg_sphere_list(t_trace *trace, t_csg_op op,
		t_intersects *intersects)
{
	bool			inl;
	bool			inr;
	t_helper_shape	*temp_shape;
	t_csg			*temp_csg;
	t_sphere		*current;

	inl = false;
	inr = false;
	current = trace->spheres;
	if (!current)
		return ;
	while (current && current->next)
	{
		temp_shape = (t_helper_shape *)current;
		temp_csg = make_new_csg(temp_shape,
				(t_helper_shape *)current->next, op);
		filter_intersections(temp_csg, &intersects, &inl, &inr);
		current = current->next;
		if (current == trace->spheres)
			break ;
	}
}
