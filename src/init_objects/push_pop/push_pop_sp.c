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
	t_mat4	transform;

	new->center = vec(0.0, 0.0, 0.0, 1.0);
	new->radius = 1;
	new->color = color(220, 155, 43);
	transform = identity();
	new->t_transform = transform;
	new->i_transform = transform;
	new->curr_scale = transform;
	new->curr_rottran = transform;
	new->transform = transform;
	new->mat = get_mat(ENAMEL);
	*start = new;
	new->id = 1;
	new->shadow = true;
	new->bump = false;
	new->next = new;
	new->prev = new;
	new->option = 0;
	new->w_frost = false;
	new->pattern = uv_checker(20, 10, color(40, 40, 40), color(255, 255, 255));
	new->bump_level = DEFAULT_BUMP;
	new->rots = vec(0, 0, 0, 0);
}

//copy a sphere and place it immediately after the current sphere in the list

bool	insert_spcopy_after(t_trace *trace, t_sphere **current, bool flag)
{
	t_sphere	*sp_to_copy;
	t_sphere	*new;

	new = (t_sphere *)malloc(sizeof(t_sphere));
	if (!new)
		return (true);
	sp_to_copy = *current;
	if (!*current)
	{
		make_default_sp(&trace->spheres, new);
		trace->on->object = trace->spheres;
		new->texture = trace->textures;
		trace->on->type = SPHERE;
		trace->total_ints += 2;
		return (false);
	}
	else if (!flag)
	{
		make_default_sp(&trace->spheres, new);
		new->texture = trace->textures;
	}
	else
		*new = *sp_to_copy;
	new->next = sp_to_copy->next;
	new->prev = sp_to_copy;
	sp_to_copy->next->prev = new;
	sp_to_copy->next = new;
	update_sphere_ids(trace->spheres);
	trace->total_ints += 2;
	return (false);
}

static inline void	set_empty_spheres(t_trace *trace)
{
	trace->spheres = NULL;
	trace->curr_sp = NULL;
	trace->on->object = trace->curr_sp;
	//trace->on->type = VOID;
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
	trace->total_ints -= 2;
	if (trace->spheres)
		update_sphere_ids(trace->spheres);
}
