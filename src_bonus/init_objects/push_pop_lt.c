#include "minirt.h"

//update the id numbers of all lights in the list

static inline void	update_light_ids(t_light *light)
{
	t_light	*start;
	int			id;

	start = light;
	id = 1;

	light->id = id++;
	light = light->next;
	while (light != start)
	{
		light->id = id++;
		light = light->next;
	}
}

//if list is empty, make a default light and set it as the current object

static inline void	make_default_lt(t_light **start, t_light *new)
{
	new->center = vec(0.0, 0.0, 0.0, 1.0);
	new->color = color(1, 1, 1);
	*start = new;
	new->id = 1;
	new->type = POINT;
	new->next = new;
	new->prev = new;
}

//copy a light and place it immediately after the current light in the list

bool	insert_ltcopy_after(t_trace *trace, t_light **current)
{
	t_light	*sp_to_copy;
	t_light	*new;

	new = (t_light *)malloc(sizeof(t_light));
	if (!new)
		return (true);
	if (!*current)
	{
		make_default_lt(&trace->lights, new);
		trace->on->object = trace->lights;
		trace->on->type = LIGHT;
		return (false);
	}
	sp_to_copy = *current;
	*new = *sp_to_copy;
	new->next = sp_to_copy->next;
	new->prev = sp_to_copy;
	sp_to_copy->next->prev = new;
	sp_to_copy->next = new;
	update_light_ids(trace->lights);
	return (false);
}

static inline void set_empty_lights(t_trace *trace)
{
	trace->lights = NULL;
	trace->curr_lt = NULL;
	trace->on->object = trace->curr_lt;
}

//remove light from list

void	pop_lt(t_trace *trace, t_light **current)
{
	t_light	*to_destroy;
	t_light	*prev_lt;
	t_light	*next_lt;

	if (!current || !*current)
		return ;
	to_destroy = *current;
	if (to_destroy->next == to_destroy)
		set_empty_lights(trace);
	else
	{
		prev_lt = to_destroy->prev;
		next_lt = to_destroy->next;
		prev_lt->next = next_lt;
		next_lt->prev = prev_lt;
		trace->curr_lt = to_destroy->prev;
		if (trace->lights == to_destroy)
			trace->lights = prev_lt;
		trace->on->object = trace->curr_lt;
	}
	free(to_destroy);
	to_destroy = NULL;
	if (trace->lights)
		update_light_ids(trace->lights);
}