/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_pop_hy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:07:31 by arybarsk          #+#    #+#             */
/*   Updated: 2024/11/18 18:07:33 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//update the id numbers of all hyperboloids in the list

static inline void	update_hyperboloid_ids(t_hyperboloid *hyperboloid)
{
	t_hyperboloid	*start;
	int			id;

	start = hyperboloid;
	id = 1;
	hyperboloid->id = id++;
	hyperboloid = hyperboloid->next;
	while (hyperboloid != start)
	{
		hyperboloid->id = id++;
		hyperboloid = hyperboloid->next;
	}
}

//if list is empty, make a default hyperboloid and set it as the current object

static inline void	make_default_hy(t_hyperboloid **start, t_hyperboloid *new)
{
	t_matrix_4x4	transform;

	new->center = vec(0.0, 0.0, 0.0, 1.0);
	new->rad1 = 1.0;
	new->rad2 = 1.0;
	new->height = 2.0;
	new->half_h = new->height / 2.0;
	new->norm = vec(0.0, 1.0, 0.0, 0.0);
	new->color.r = 0;
	new->color.g = 200;
	new->color.b = 100;
	identity(&transform);
	new->curr_scale = transform;
	new->curr_rottran = transform;
	new->transform = transform;
	new->mat = get_mat(DEFAULT);
	*start = new;
	new->id = 1;
	new->shadow = true;
	new->bump = false;
	new->next = new;
	new->prev = new;
	new->texture = NULL; //if not assigned
}

//copy a hyperboloid and place it immediately after the current hyperboloid in the list

bool	insert_hycopy_after(t_trace *trace, t_hyperboloid **current)
{
	t_hyperboloid	*hy_to_copy;
	t_hyperboloid	*new;

	new = (t_hyperboloid *)malloc(sizeof(t_hyperboloid));
	if (!new)
		return (true);
	if (!*current)
	{
		make_default_hy(&trace->hyperboloids, new);
		trace->on->object = trace->hyperboloids;
		trace->on->type = HYPERBOLOID;
		trace->total_ints += 4;
		return (false);
	}
	hy_to_copy = *current;
	*new = *hy_to_copy;
	new->next = hy_to_copy->next;
	new->prev = hy_to_copy;
	hy_to_copy->next->prev = new;
	hy_to_copy->next = new;
	trace->total_ints += 4;
	update_hyperboloid_ids(trace->hyperboloids);
	return (false);
}

static inline void set_empty_hyperboloids(t_trace *trace)
{
	trace->hyperboloids = NULL;
	trace->curr_hy = NULL;
	trace->on->object = trace->curr_hy;	
}

//remove hyperboloid from list

void	pop_hy(t_trace *trace, t_hyperboloid **current)
{
	t_hyperboloid	*to_destroy;
	t_hyperboloid	*prev_hy;
	t_hyperboloid	*next_hy;

	if (!current || !*current)
		return ;
	to_destroy = *current;
	if (to_destroy->next == to_destroy)
		set_empty_hyperboloids(trace);
	else
	{
		prev_hy = to_destroy->prev;
		next_hy = to_destroy->next;
		prev_hy->next = next_hy;
		next_hy->prev = prev_hy;
		trace->curr_hy = to_destroy->prev;
		if (trace->hyperboloids == to_destroy)
			trace->hyperboloids = prev_hy;
		trace->on->object = trace->curr_hy;
	}
	free(to_destroy);
	to_destroy = NULL;
	trace->total_ints -= 4;
	if (trace->hyperboloids)
		update_hyperboloid_ids(trace->hyperboloids);
}