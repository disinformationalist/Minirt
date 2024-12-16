/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_all_objects2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:01:05 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 14:01:08 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	print_cylinders(t_cylinder *cylinder)
{
	t_cylinder	*curr_cy;

	if (cylinder == NULL)
		return ;
	curr_cy = cylinder;
	printf("\n----------------------CYLINDERS-----------------------\n");
	while (true)
	{
		printf("\n----------------cylinder id: %d ----------- \n", curr_cy->id);
		printf("centerx: %f\n", curr_cy->center.x);
		printf("centery: %f\n", curr_cy->center.y);
		printf("centerz: %f\n\n", curr_cy->center.z);
		printf("norm_vecx: %f\n", curr_cy->norm.x);
		printf("norm_vecy: %f\n", curr_cy->norm.y);
		printf("norm_vecz: %f\n\n", curr_cy->norm.z);
		printf("cylinder diam: %f\n", curr_cy->radius * 2);
		printf("cylinder height: %f\n", curr_cy->height);
		printf("cylinder color rgb: r: %f, g: %f, b %f\n", curr_cy->color.r,
			curr_cy->color.g, curr_cy->color.b);
		curr_cy = curr_cy->next;
		if (curr_cy == cylinder)
			break ;
	}
}

void	print_planes(t_plane *plane)
{
	t_plane	*curr_pl;

	if (plane == NULL)
		return ;
	curr_pl = plane;
	printf("\n----------------------PLANES-----------------------\n");
	while (true)
	{
		printf("\n----------------plane id: %d ----------- \n", curr_pl->id);
		printf("pointx: %f\n", curr_pl->point.x);
		printf("pointy: %f\n", curr_pl->point.y);
		printf("pointz: %f\n", curr_pl->point.z);
		printf("norm_vecx: %f\n", curr_pl->norm.x);
		printf("norm_vecy: %f\n", curr_pl->norm.y);
		printf("norm_vecz: %f\n", curr_pl->norm.z);
		printf("plane color rgb: r: %f, g: %f, b %f\n", curr_pl->color.r,
			curr_pl->color.g, curr_pl->color.b);
		curr_pl = curr_pl->next;
		if (curr_pl == plane)
			break ;
	}
}
