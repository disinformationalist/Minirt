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
	t_mat4	transform;

	new->center = vec(0.0, 0.0, 0.0, 1.0);
	new->radius = 1;
	new->height = 2.0;
	new->norm = vec(0.0, 1.0, 0.0, 0.0);
	new->color = color(79, 0, 173);
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
	new->w_frost = false;
	new->option = 0;
	new->next = new;
	new->prev = new;
	new->pattern = uv_checker(18, 9 / M_PI, \
	color(40, 40, 40), color(255, 255, 255));
	new->bump_level = DEFAULT_BUMP;
	new->fuzz_lev = new->bump_level / 1000;
	new->caps = true;
	new->rots = vec(0, 0, 0, 0);
}

//copy a cylinder and place it immediately after the current cyl in the list

bool	insert_cycopy_after(t_trace *trace, t_cylinder **current, bool flag)
{
	t_cylinder	*cy_to_copy;
	t_cylinder	*new;

	new = (t_cylinder *)malloc(sizeof(t_cylinder));
	if (!new)
		return (true);
	cy_to_copy = *current;
	if (!*current)
	{
		make_default_cy(&trace->cylinders, new);
		trace->on->object = trace->cylinders;
		new->texture = trace->textures;
		trace->on->type = CYLINDER;
		trace->total_ints += 4;
		return (false);
	}
	else if (!flag)
	{
		make_default_cy(&trace->cylinders, new);
		new->texture = trace->textures;
	}
	else
		*new = *cy_to_copy;
	new->next = cy_to_copy->next;
	new->prev = cy_to_copy;
	cy_to_copy->next->prev = new;
	cy_to_copy->next = new;
	trace->total_ints += 4;
	update_cylinder_ids(trace->cylinders);
	return (false);
}

static inline void	set_empty_cylinders(t_trace *trace)
{
	trace->cylinders = NULL;
	trace->curr_cy = NULL;
	trace->on->object = trace->curr_cy;
	//trace->on->type = VOID;
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
	trace->total_ints -= 4;
	if (trace->cylinders)
		update_cylinder_ids(trace->cylinders);
}
