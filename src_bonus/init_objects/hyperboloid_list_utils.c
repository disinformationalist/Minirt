/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hyperboloid_list_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:02:26 by arybarsk          #+#    #+#             */
/*   Updated: 2024/11/18 18:02:28 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_hy_vals(t_hyperboloid *new, char **line)
{
	char *rad1_str;
	char *rad2_str;
	char *height_str;	

	rad1_str = line[3];
	rad2_str = line[4];
	height_str = line[5];
	new->center = get_coordinates(line[1], 1.0);
	new->norm = get_coordinates(line[2], 0.0);
	new->rad1 = get_double(&rad1_str);
	new->rad2 = get_double(&rad2_str);
	new->height = get_double(&height_str);
	new->half_h = new->height / 2.0;
	new->color = get_color(line[5], 1.0);
	new->mat = get_mat(DEFAULT);
	new->shadow = true;
	new->bump = false;
	new->texture = NULL; //if not assigned
}

t_hyperboloid *create_hyperboloid(char **line)
{
	t_hyperboloid *new;

	new = (t_hyperboloid *)malloc(sizeof(t_hyperboloid));
	if (!new)
		return (NULL);
	set_hy_vals(new, line);
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_hy(t_hyperboloid **start, char **line)
{
	t_hyperboloid *new;
	t_hyperboloid *last;

	new = create_hyperboloid(line);
	if (!new)
		return (true);
	if (*start == NULL)
	{
		*start = new;
		new->id = 1;
		return (false);
	}
	last = (*start)->prev;
	new->id = last->id + 1;
	new->next = *start;
	(*start)->prev = new;
	new->prev = last;
	last->next = new;
	return (false);
}
