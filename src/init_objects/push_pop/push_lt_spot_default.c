#include "minirt.h"

void	make_default_spotlt(t_light **start, t_light *new)
{
	new->type = SPOT;
	new->dir = vec(0, 1, 0, 0);
	new->brightness = 0.75;
	new->center = vec(0, 1, 0, 1);
	new->color = color(1.0, 1.0, 1.0);
	new->inner_cone = cos(15 * DEG_TO_RAD);
	new->outer_cone = cos(22 * DEG_TO_RAD);
	new->in_angle = 15;
	new->out_angle = 22;
	if (new->inner_cone == new->outer_cone)
		new->inv_conediff = 0;
	else
		new->inv_conediff = 1.0 / (new->inner_cone - new->outer_cone);
	new->id = 1;
	new->next = new;
	new->prev = new;
	*start = new;
	set_lt_transforms(new);
}

void	adj_pntrs_copy3(t_light *to_copy, t_light *new)
{
	new->next = to_copy->next;
	new->prev = to_copy;
	to_copy->next->prev = new;
	to_copy->next = new;
}

//produce a spot lt.

bool	insert_ltcopy_after3(t_trace *trace, t_light **current)
{
	t_light	*lt_to_copy;
	t_light	*new;

	new = (t_light *)malloc(sizeof(t_light));
	if (!new)
		return (true);
	make_default_spotlt(&trace->lights, new);
	trace->sl_count++;
	if (!*current)
	{
		trace->on->object = trace->lights;
		trace->curr_lt = trace->on->object;
		trace->on->type = LIGHT;
		return (false);
	}
	lt_to_copy = *current;
	adj_pntrs_copy3(lt_to_copy, new);
	next_list_ob(trace, trace->on);
	update_light_ids(trace->lights);
	return (false);
}
