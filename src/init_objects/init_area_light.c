#include "minirt.h"

void	set_arealt_transform(t_light *lt, double width, double length)
{
	t_mat4	rot;
	t_mat4	trans;

	trans = translation(lt->center.x, lt->center.y, lt->center.z);
	rot = rot_to(vec(0, 1, 0, 0), lt->dir);
	lt->curr_rottran = mat_mult(rot, trans);
	lt->curr_scale = scaling(width / 2, 1.0, length / 2);
	lt->transform = mat_mult(lt->curr_scale, lt->curr_rottran);
	lt->samples = lt->usteps * lt->vsteps;
	lt->type = AREA;
}

void	set_arealt(t_light *lt)
{
	lt->v1 = mat_vec_mult(lt->transform, vec(2, 0, 0, 0));
	lt->v2 = mat_vec_mult(lt->transform, vec(0, 0, 2, 0));
	lt->corner = subtract_vec(lt->center, div_vec(2, add_vec(lt->v1, lt->v2)));
	lt->uvec = div_vec(lt->usteps, lt->v1);
	lt->vvec = div_vec(lt->vsteps, lt->v2);
	lt->dir = norm_vec(mat_vec_mult(lt->curr_rottran, vec(0, 1, 0, 0)));
}

void	set_lt_cube_transform(t_cube *cube)
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

t_cube	*set_lt_cube(t_light *new, double wid, double len)
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
	set_lt_cube_transform(cube);
	cube->shadow = false;
	cube->emitter = true;
	cube->next = cube;
	cube->prev = cube;
	return (cube);
}

int	set_al_vals(t_trace *trace, t_light *new, char **line)
{
	char	*bright_ratio;
	char	*wid_str;
	char	*len_str;
	double	wid;
	double	len;

	wid_str = line[4];
	len_str = line[5];
	wid = get_double(&wid_str);
	len = get_double(&len_str);
	bright_ratio = line[3];
	new->area = wid * len;
	new->dir = norm_vec(get_coordinates(line[2], 1.0));
	new->brightness = get_double(&bright_ratio);
	new->center = get_coordinates(line[1], 1.0);
	new->color = get_color(line[6], 255.0);
	new->usteps = ft_atoi(line[7]);
	new->vsteps = ft_atoi(line[8]);
	set_arealt_transform(new, wid, len);
	set_arealt(new);
	new->emitter = set_lt_cube(new, wid, len);
	if (!new->emitter)
		return (1);
	trace->total_ints += 2;
	return (0);
}
