#include "minirt.h"

//update the id numbers of all cubes in the list

static inline void	update_cube_ids(t_cube *cube)
{
	t_cube	*start;
	int			id;

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

//if list is empty, make a default cube and set it as the current object

static inline void	make_default_cu(t_cube **start, t_cube *new)
{
	t_matrix_4x4	transform;

	new->center = vec(0.0, 0.0, 0.0, 1.0);
	new->h_width = 1.0;
	new->h_height = 1.0;
	new->h_depth = 1.0;
	new->norm = vec(0.0, 1.0, 0.0, 0.0);
	new->color.r = 0;
	new->color.g = 0;
	new->color.b = 200;
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

//copy a cube and place it immediately after the current cube in the list

bool	insert_cucopy_after(t_trace *trace, t_cube **current)
{
	t_cube	*cu_to_copy;
	t_cube	*new;

	new = (t_cube *)malloc(sizeof(t_cube));
	if (!new)
		return (true);
	if (!*current)
	{
		make_default_cu(&trace->cubes, new);
		trace->on->object = trace->cubes;
		trace->on->type = CUBE;
		trace->total_ints += 2;
		return (false);
	}
	cu_to_copy = *current;
	*new = *cu_to_copy;
	new->next = cu_to_copy->next;
	new->prev = cu_to_copy;
	cu_to_copy->next->prev = new;
	cu_to_copy->next = new;
	trace->total_ints += 2;
	update_cube_ids(trace->cubes);
	return (false);
}

static inline void set_empty_cubes(t_trace *trace)
{
	trace->cubes = NULL;
	trace->curr_cu = NULL;
	trace->on->object = trace->curr_cu;	
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