#include "minirt.h"

void	set_cu_vals(t_cube *new, char **line)
{
	char	*width_str;
	char	*height_str;	
	char	*depth_str;	

	width_str = line[3];
	height_str = line[4];
	depth_str = line[5];
	new->center = get_coordinates(line[1], 1.0);
	new->norm = norm_vec(get_coordinates(line[2], 0.0));
	new->h_width = get_double(&width_str) / 2.0;
	new->h_height = get_double(&height_str) / 2.0;
	new->h_depth = get_double(&depth_str) / 2.0;
	new->color = get_color(line[6], 1.0);
	new->mat = get_mat(ENAMEL);
//	new->mat = get_mat(DEFAULT);

	new->shadow = true;
	new->bump = false;
	new->sine = false;
	new->option = 0;
	new->w_frost = false;
	new->texture = NULL;
	new->emitter = false;
	new->bump_level = DEFAULT_BUMP;
	new->amp = new->bump_level / 1000;
	new->fuzz_lev = new->bump_level / 1000;
}

t_cube	*create_cube(char **line)
{
	t_cube	*new;

	new = (t_cube *)malloc(sizeof(t_cube));
	if (!new)
		return (NULL);
	set_cu_vals(new, line);
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_cu(t_cube **start, char **line)
{
	t_cube	*new;
	t_cube	*last;

	new = create_cube(line);
	if (!new)
		return (true);
	if (*start == NULL)
	{
		*start = new;
		new->id = 1;
		return (false);
	}
	last = (*start)->prev;
	new->id = last->id + 1;
	new->next = *start;
	(*start)->prev = new;
	new->prev = last;
	last->next = new;
	return (false);
}
