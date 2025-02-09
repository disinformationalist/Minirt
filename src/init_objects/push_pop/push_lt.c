#include "minirt.h"

//update the id numbers of all lights in the list

void	update_light_ids2(t_light *light, t_light *lt_to_copy, t_trace *trace)
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
	if (lt_to_copy->type == SPOT)
		trace->sl_count++;
	if (lt_to_copy->type == AREA)
		trace->al_count++;
}

//if list is empty, make a default light and set it as the current object

static inline void	make_default_lt(t_light **start, t_light *new)
{
	new->center = vec(0.0, 1.0, 0.0, 1.0);
	new->color = color(1, 1, 1);
	new->brightness = 0.75;
	*start = new;
	new->id = 1;
	new->type = POINT;
	new->next = new;
	new->prev = new;
	set_lt_transforms(new);
}

int	copy_arealt_cube(t_light *to_copy, t_light *new)
{
	t_cube	*cube;

	cube = (t_cube *)malloc(sizeof(t_cube));
	if (!cube)
		return (1);
	new->emitter = cube;
	*(new->emitter) = *(to_copy->emitter);
	return (0);
}

void	adj_pntrs_copy(t_light *to_copy, t_light *new)
{
	new->next = to_copy->next;
	new->prev = to_copy;
	to_copy->next->prev = new;
	to_copy->next = new;
}

//copy a light and place it immediately after the current light in the list

bool	insert_ltcopy_after(t_trace *trace, t_light **current)
{
	t_light	*lt_to_copy;
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
	lt_to_copy = *current;
	*new = *lt_to_copy;
	if (lt_to_copy->type == AREA)
	{
		if (copy_arealt_cube(lt_to_copy, new))
			return (free(new), true);
		trace->total_ints += 2;
	}
	adj_pntrs_copy(lt_to_copy, new);
	update_light_ids2(trace->lights, lt_to_copy, trace);
	return (false);
}

bool	insert_ltpoint(t_trace *trace, t_light **current)
{
	t_light	*lt_to_copy;
	t_light	*new;

	new = (t_light *)malloc(sizeof(t_light));
	if (!new)
		return (true);
	make_default_lt(&trace->lights, new);
	if (!*current)
	{
		trace->on->object = trace->lights;
		trace->curr_lt = trace->on->object;
		trace->on->type = LIGHT;
		return (false);
	}
	lt_to_copy = *current;
	adj_pntrs_copy(lt_to_copy, new);
	next_list_ob(trace, trace->on);
	update_light_ids2(trace->lights, lt_to_copy, trace);
	return (false);
}
