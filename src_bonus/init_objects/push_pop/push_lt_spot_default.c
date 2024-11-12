#include "minirt.h"

void	make_default_spotlt(t_light **start, t_light *new)
{
	new->type = SPOT;
	new->dir = vec(0, 1, 0, 0);
	new->brightness =  0.75;
	new->center = vec(0, 1, 0, 1);
	new->color = color(1.0, 1.0, 1.0);
	new->inner_cone = cos(15 * DEG_TO_RAD);
	new->outer_cone = cos(22 * DEG_TO_RAD);
	if (new->inner_cone == new->outer_cone)
		new->inv_conediff = 0;
	else
		new->inv_conediff = 1.0 / (new->inner_cone - new->outer_cone);
	new->id = 1;
	new->next = new;
	new->prev = new;
	*start = new;
}

//if the list is empty, produce a spot lt.

bool	insert_ltcopy_after3(t_trace *trace)
{
	t_light	*new;

	new = (t_light *)malloc(sizeof(t_light));
	if (!new)
		return (true);
	make_default_spotlt(&trace->lights, new);
	trace->on->object = trace->lights;
	trace->curr_lt = trace->on->object;
	trace->on->type = LIGHT;
	return (false);
}