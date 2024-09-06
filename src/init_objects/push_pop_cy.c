#include "minirt.h"

//update the id numbers of all cylinders in the list

static inline void	update_cylinder_ids(t_cylinder *cylinder)
{
	t_cylinder	*start;
	int			id;

	start = cylinder;
	id = 1;

	cylinder->id = id++;
	cylinder = cylinder->next;
	while (cylinder != start)
	{
		cylinder->id = id++;
		cylinder = cylinder->next;
	}
}

//if list is empty, make a default cylinder and set it as the current object

static inline void	make_default_cy(t_cylinder **start, t_cylinder *new)
{
	new->center = vec(0.0,-0.5,-6.5);
	new->radius = 1;
	new->height = 3.0;
	new->norm_vector = vec(0.0,1.0,0.0);
	new->color.r = 0;
	new->color.g = 200;
	new->color.b = 100;
	*start = new;
	new->id = 1;
	new->next = new;
	new->prev = new;
}

//copy a cylinder and place it immediately after the current cylinder in the list

bool	insert_cycopy_after(t_trace *trace, t_cylinder **current)
{
	t_cylinder	*cy_to_copy;
	t_cylinder	*new;

	new = (t_cylinder *)malloc(sizeof(t_cylinder));
	if (!new)
		return (true);
	if (!*current)
	{
		make_default_cy(&trace->cylinders, new);
		trace->on->object = trace->cylinders;
		trace->on->type = CYLINDER;
		return (false);
	}
	cy_to_copy = *current;
	*new = *cy_to_copy;
	new->next = cy_to_copy->next;
	new->prev = cy_to_copy;
	cy_to_copy->next->prev = new;
	cy_to_copy->next = new;
	update_cylinder_ids(trace->cylinders);
	return (false);
}

static inline void set_empty_cylinders(t_trace *trace)
{
	trace->cylinders = NULL;
	trace->curr_cy = NULL;
	trace->on->object = trace->curr_cy;	
}

//remove cylinder from list

void	pop_cy(t_trace *trace, t_cylinder **current)
{
	t_cylinder	*to_destroy;
	t_cylinder	*prev_cy;
	t_cylinder	*next_cy;

	if (!current || !*current)
		return ;
	to_destroy = *current;
	if (to_destroy->next == to_destroy)
		set_empty_cylinders(trace);
	else
	{
		prev_cy = to_destroy->prev;
		next_cy = to_destroy->next;
		prev_cy->next = next_cy;
		next_cy->prev = prev_cy;
		trace->curr_cy = to_destroy->prev;
		if (trace->cylinders == to_destroy)
			trace->cylinders = prev_cy;
		trace->on->object = trace->curr_cy;
	}
	free(to_destroy);
	to_destroy = NULL;
	if (trace->cylinders)
		update_cylinder_ids(trace->cylinders);
}