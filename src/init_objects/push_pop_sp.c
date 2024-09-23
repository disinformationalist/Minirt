#include "minirt.h"

//update the id numbers of all spheres in the list

static inline void	update_sphere_ids(t_sphere *sphere)
{
	t_sphere	*start;
	int			id;

	start = sphere;
	id = 1;

	sphere->id = id++;
	sphere = sphere->next;
	while (sphere != start)
	{
		sphere->id = id++;
		sphere = sphere->next;
	}
}

//if list is empty, make a default sphere and set it as the current object

static inline void	make_default_sp(t_sphere **start, t_sphere *new)
{
	t_matrix_4x4 transform;

	new->center = vec(0.0, 0.0, 0.0, 1.0);
	new->radius = 1;
	new->color.r = 200;
	new->color.g = 50;
	new->color.b = 200;
	identity(&transform);
	new->transform = transform;
	*start = new;
	new->id = 1;
	new->next = new;
	new->prev = new;
}

//copy a sphere and place it immediately after the current sphere in the list

bool	insert_spcopy_after(t_trace *trace, t_sphere **current)
{
	t_sphere	*sp_to_copy;
	t_sphere	*new;

	new = (t_sphere *)malloc(sizeof(t_sphere));
	if (!new)
		return (true);
	if (!*current)
	{
		make_default_sp(&trace->spheres, new);
		trace->on->object = trace->spheres;
		trace->on->type = SPHERE;
		return (false);
	}
	sp_to_copy = *current;
	*new = *sp_to_copy;
	new->next = sp_to_copy->next;
	new->prev = sp_to_copy;
	sp_to_copy->next->prev = new;
	sp_to_copy->next = new;
	update_sphere_ids(trace->spheres);
	return (false);
}

static inline void set_empty_spheres(t_trace *trace)
{
	trace->spheres = NULL;
	trace->curr_sp = NULL;
	trace->on->object = trace->curr_sp;
}

//remove sphere from list

void	pop_sp(t_trace *trace, t_sphere **current)
{
	t_sphere	*to_destroy;
	t_sphere	*prev_sp;
	t_sphere	*next_sp;

	if (!current || !*current)
		return ;
	to_destroy = *current;
	if (to_destroy->next == to_destroy)
		set_empty_spheres(trace);
	else
	{
		prev_sp = to_destroy->prev;
		next_sp = to_destroy->next;
		prev_sp->next = next_sp;
		next_sp->prev = prev_sp;
		trace->curr_sp = to_destroy->prev;
		if (trace->spheres == to_destroy)
			trace->spheres = prev_sp;
		trace->on->object = trace->curr_sp;
	}
	free(to_destroy);
	to_destroy = NULL;
	if (trace->spheres)
		update_sphere_ids(trace->spheres);
}
