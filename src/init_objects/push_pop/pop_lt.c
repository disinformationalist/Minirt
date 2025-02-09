#include "minirt.h"

void	update_light_ids(t_light *light)
{
	t_light	*start;
	int		id;

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

static inline void	set_empty_lights(t_trace *trace)
{
	trace->lights = NULL;
	trace->curr_lt = NULL;
	trace->on->object = trace->curr_lt;
	//trace->on->type = VOID;
}

void	adj_pntrs_destroy(t_trace *trace, t_light *to_destroy, \
	t_light *prev_lt, t_light *next_lt)
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

//remove light from list

void	pop_lt(t_trace *trace, t_light **current)
{
	t_light	*to_destroy;
	t_light	*prev_lt;
	t_light	*next_lt;

	prev_lt = NULL;
	next_lt = NULL;
	if (!current || !*current)
		return ;
	to_destroy = *current;
	if (to_destroy->next == to_destroy)
		set_empty_lights(trace);
	else
		adj_pntrs_destroy(trace, to_destroy, prev_lt, next_lt);
	if (to_destroy->type == SPOT)
		trace->sl_count--;
	if (to_destroy->type == AREA)
	{
		trace->total_ints -= 2;
		trace->al_count--;
		free(to_destroy->emitter);
	}
	free(to_destroy);
	to_destroy = NULL;
	if (trace->lights)
		update_light_ids(trace->lights);
}
