#include "minirt.h"

//update the id numbers of all spheres in the list

static inline void	update_lens_ids(t_lens *lens)
{
	t_lens	*start;
	int			id;

	start = lens;
	id = 1;

	lens->id = id++;
	lens = lens->next;
	while (lens != start)
	{
		lens->id = id++;
		lens = lens->next;
	}
}

//if list is empty, make a default sphere and set it as the current object

static inline void	make_default_le(t_lens **start, t_lens *new)
{
	new->sphere_1.center = vec(0.0,-0.5,-6.5, 1.0);
	new->sphere_1.radius = 1.5;
	new->sphere_1.color.r = 200;
	new->sphere_1.color.g = 50;
	new->sphere_1.color.b = 200;
	new->sphere_2.center = vec(0.0,-0.0,-5.5, 1.0);
	new->sphere_2.radius = 1.5;
	new->sphere_2.color.r = 50;
	new->sphere_2.color.g = 200;
	new->sphere_2.color.b = 200;
	*start = new;
	new->id = 1;
	new->next = new;
	new->prev = new;
}

//copy a sphere and place it immediately after the current sphere in the list

bool	insert_lecopy_after(t_trace *trace, t_lens **current)
{
	t_lens	*le_to_copy;
	t_lens	*new;

	new = (t_lens *)malloc(sizeof(t_lens));
	if (!new)
		return (true);
	if (!*current)
	{
		make_default_le(&trace->lenses, new);
		trace->on->object = trace->lenses;
		trace->on->type = LENS;
		trace->total_ints += 4;
		return (false);
	}
	le_to_copy = *current;
	*new = *le_to_copy;
	new->next = le_to_copy->next;
	new->prev = le_to_copy;
	le_to_copy->next->prev = new;
	le_to_copy->next = new;
	trace->total_ints += 4;
	update_lens_ids(trace->lenses);
	return (false);
}

static inline void set_empty_lenses(t_trace *trace)
{
	trace->lenses = NULL;
	trace->curr_le = NULL;
	trace->on->object = trace->curr_le;
}

//remove sphere from list

void	pop_le(t_trace *trace, t_lens **current)
{
	t_lens	*to_destroy;
	t_lens	*prev_le;
	t_lens	*next_le;

	if (!current || !*current)
		return ;
	to_destroy = *current;
	if (to_destroy->next == to_destroy)
		set_empty_lenses(trace);
	else
	{
		prev_le = to_destroy->prev;
		next_le = to_destroy->next;
		prev_le->next = next_le;
		next_le->prev = prev_le;
		trace->curr_le = to_destroy->prev;
		if (trace->lenses == to_destroy)
			trace->lenses = prev_le;
		trace->on->object = trace->curr_le;
	}
	free(to_destroy);
	to_destroy = NULL;
	trace->total_ints -= 4;
	if (trace->lenses)
		update_lens_ids(trace->lenses);
}
