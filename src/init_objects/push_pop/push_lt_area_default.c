#include "minirt.h"

static inline void	set_lt_cube_transform2(t_cube *cube)
{
	t_mat4	inv_rot;
	t_mat4	inv_trans;

	inv_rot = rot_to(cube->norm, vec(0, 1, 0, 0));
	inv_trans = translation(-cube->center.x, -cube->center.y, -cube->center.z);
	cube->curr_rottran = mat_mult(inv_rot, inv_trans);
	cube->curr_scale = \
		inv_scaling(cube->h_width, cube->h_height, cube->h_depth);
	cube->transform = (mat_mult(cube->curr_scale, cube->curr_rottran));
}

static inline void	set_arealt_transform2(t_light *lt, double width, \
	double length)
{
	t_mat4	rot;
	t_mat4	trans;

	trans = translation(lt->center.x, lt->center.y, lt->center.z);
	rot = rot_to(vec(0, 1, 0, 0), lt->dir);
	lt->curr_rottran = mat_mult(rot, trans);
	lt->curr_scale = scaling(width / 2, 1.0, length / 2);
	lt->transform = mat_mult(lt->curr_scale, lt->curr_rottran);
}

static inline t_cube	*set_lt_cube2(t_light *new, double wid, double len)
{
	t_cube	*cube;

	cube = (t_cube *)malloc(sizeof(t_cube));
	if (!cube)
		return (NULL);
	cube->center = new->center;
	cube->norm = new->dir;
	cube->h_width = wid / 2.0;
	cube->h_height = 0.05;
	cube->h_depth = len / 2.0;
	cube->color = mult_color(new->brightness * 255.0, new->color);
	cube->bright = new->brightness;
	cube->mat = get_mat(DEFAULT);
	set_lt_cube_transform2(cube);
	cube->shadow = false;
	cube->emitter = true;
	cube->next = cube;
	cube->prev = cube;
	return (cube);
}

static inline bool	make_default_arealt(t_light **start, t_light *new)
{
	new->area = 4;
	new->dir = vec(0, -1, 0, 0);
	new->brightness = 0.75;
	new->center = vec(0, 3, 0, 1);
	new->color = color(1.0, 1.0, 1.0);
	new->usteps = 4;
	new->vsteps = 4;
	set_arealt_transform2(new, 2, 2);
	set_arealt(new);
	new->samples = new->usteps * new->vsteps;
	new->type = AREA;
	new->emitter = set_lt_cube2(new, 2, 2);
	if (!new->emitter)
		return (1);
	new->id = 1;
	new->next = new;
	new->prev = new;
	*start = new;
	return (0);
}

//produce a square area lt.

bool	insert_ltcopy_after2(t_trace *trace, t_light **current)
{
	t_light	*lt_to_copy;
	t_light	*new;

	new = (t_light *)malloc(sizeof(t_light));
	if (!new)
		return (true);
	make_default_arealt(&trace->lights, new);
	trace->al_count++;
	trace->total_ints += 2;
	if (!*current)
	{
		trace->on->object = trace->lights;
		trace->curr_lt = trace->on->object;
		trace->on->type = LIGHT;
		return (false);
	}
	lt_to_copy = *current;
	adj_pntrs_copy2(lt_to_copy, new);
	next_list_ob(trace, trace->on);
	update_light_ids(trace->lights);
	return (false);
}
