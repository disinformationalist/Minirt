#include "minirt.h"

//update the id numbers of all planes in the list

static inline void	update_plane_ids(t_plane *plane)
{
	t_plane	*start;
	int		id;

	start = plane;
	id = 1;
	plane->id = id++;
	plane = plane->next;
	while (plane != start)
	{
		plane->id = id++;
		plane = plane->next;
	}
}

//if list is empty, make a default plane and set it as the current object

static inline void	make_default_pl(t_plane **start, t_plane *new)
{
	new->point = vec(0.0,-0.5,-6.5);
	new->norm_vector = vec(0.0,1.0,0.0);
	new->color.r = 200;
	new->color.g = 0;
	new->color.b = 0;
	*start = new;
	new->id = 1;
	new->next = new;
	new->prev = new;
}

//copy a plane and place it immediately after the current plane in the list

bool	insert_plcopy_after(t_trace *trace, t_plane **current)
{
	t_plane	*pl_to_copy;
	t_plane	*new;

	new = (t_plane *)malloc(sizeof(t_plane));
	if (!new)
		return (true);
	if (!*current)
	{
		make_default_pl(&trace->planes, new);
		trace->on->object = trace->planes;
		trace->on->type = PLANE;
		return (false);
	}
	pl_to_copy = *current;
	*new = *pl_to_copy;
	new->next = pl_to_copy->next;
	new->prev = pl_to_copy;
	pl_to_copy->next->prev = new;
	pl_to_copy->next = new;
	update_plane_ids(trace->planes);
	return (false);
}

static inline void set_empty_planes(t_trace *trace)
{
	trace->planes = NULL;
	trace->curr_pl = NULL;
	trace->on->object = trace->curr_pl;	
}

//remove plane from list

void	pop_pl(t_trace *trace, t_plane **current)
{
	t_plane	*to_destroy;
	t_plane	*prev_pl;
	t_plane	*next_pl;

	if (!current || !*current)
		return ;
	to_destroy = *current;
	if (to_destroy->next == to_destroy)
		set_empty_planes(trace);
	else
	{
		prev_pl = to_destroy->prev;
		next_pl = to_destroy->next;
		prev_pl->next = next_pl;
		next_pl->prev = prev_pl;
		trace->curr_pl = to_destroy->prev;
		if (trace->planes == to_destroy)
			trace->planes = prev_pl;
		trace->on->object = trace->curr_pl;
	}
	free(to_destroy);
	to_destroy = NULL;
	if (trace->planes)
		update_plane_ids(trace->planes);
}