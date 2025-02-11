#include "minirt.h"

//update the id numbers of all cubes in the list

static inline void	update_cube_ids(t_cube *cube)
{
	t_cube	*start;
	int		id;

	start = cube;
	id = 1;
	cube->id = id++;
	cube = cube->next;
	while (cube != start)
	{
		cube->id = id++;
		cube = cube->next;
	}
}

//copy a cube and place it immediately after the current cube in the list

bool	insert_cucopy_after(t_trace *trace, t_cube **current, bool flag)
{
	t_cube	*cu_to_copy;
	t_cube	*new;

	new = (t_cube *)malloc(sizeof(t_cube));
	if (!new)
		return (true);
	cu_to_copy = *current;
	if (!*current)
	{
		make_default_cu(&trace->cubes, new);
		trace->on->object = trace->cubes;
		new->texture = trace->textures;
		trace->on->type = CUBE;
		trace->total_ints += 2;
		return (false);
	}
	else if (!flag)
	{
		make_default_cu(&trace->cubes, new);
		new->texture = trace->textures;
	}
	else
		*new = *cu_to_copy;
	new->next = cu_to_copy->next;
	new->prev = cu_to_copy;
	cu_to_copy->next->prev = new;
	cu_to_copy->next = new;
	trace->total_ints += 2;
	update_cube_ids(trace->cubes);
	return (false);
}

static inline void	set_empty_cubes(t_trace *trace)
{
	trace->cubes = NULL;
	trace->curr_cu = NULL;
	trace->on->object = trace->curr_cu;
	//trace->on->type = VOID;
}

//remove cube from list

void	pop_cu(t_trace *trace, t_cube **current)
{
	t_cube	*to_destroy;
	t_cube	*prev_cu;
	t_cube	*next_cu;

	if (!current || !*current)
		return ;
	to_destroy = *current;
	if (to_destroy->next == to_destroy)
		set_empty_cubes(trace);
	else
	{
		prev_cu = to_destroy->prev;
		next_cu = to_destroy->next;
		prev_cu->next = next_cu;
		next_cu->prev = prev_cu;
		trace->curr_cu = to_destroy->prev;
		if (trace->cubes == to_destroy)
			trace->cubes = prev_cu;
		trace->on->object = trace->curr_cu;
	}
	free(to_destroy);
	to_destroy = NULL;
	trace->total_ints -= 2;
	if (trace->cubes)
		update_cube_ids(trace->cubes);
}
