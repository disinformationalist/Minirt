#include "minirt.h"

//if list is empty, make a default cube and set it as the current object

static inline void	make_links(t_cube **start, t_cube *new)
{
	*start = new;
	new->next = new;
	new->prev = new;
}

void	make_default_cu(t_cube **start, t_cube *new)
{
	t_mat4	transform;

	new->center = vec(0.0, 0.0, 0.0, 1.0);
	new->h_width = 1.0;
	new->h_height = 1.0;
	new->h_depth = 1.0;
	new->norm = vec(0.0, 1.0, 0.0, 0.0);
	new->color = color(0, 174, 255);
	transform = identity();
	new->curr_scale = transform;
	new->curr_rottran = transform;
	new->transform = transform;
	new->t_transform = transform;
	new->i_transform = transform;
	new->mat = get_mat(DEFAULT);
	new->id = 1;
	new->shadow = true;
	new->bump = false;
	new->sine = false;
	new->w_frost = false;
	new->option = 0;
	new->emitter = false;
	new->pattern = uv_checker(6, 6, color(30, 30, 30), \
		color(255, 255, 255));
	new->bump_level = DEFAULT_BUMP;
	new->rots = vec(0, 0, 0, 0);
	make_links(start, new);
}
