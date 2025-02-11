#include "minirt.h"

void	set_cy_vals(t_cylinder *new, char **line)
{
	char	*diam_str;
	char	*height_str;	

	diam_str = line[3];
	height_str = line[4];
	new->center = get_coordinates(line[1], 1.0);
	new->norm = norm_vec(get_coordinates(line[2], 0.0));
	new->radius = get_double(&diam_str) / 2;
	new->height = get_double(&height_str);
	new->color = get_color(line[5], 1.0);
	new->mat = get_mat(ENAMEL);
	new->shadow = true;
	new->bump = false;
	new->option = 0;
	new->w_frost = false;
	new->texture = NULL;
	new->bump_level = DEFAULT_BUMP;
	new->caps = true;
}

t_cylinder	*create_cylinder(char **line)
{
	t_cylinder	*new;

	new = (t_cylinder *)malloc(sizeof(t_cylinder));
	if (!new)
		return (NULL);
	set_cy_vals(new, line);
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_cy(t_cylinder **start, char **line)
{
	t_cylinder	*new;
	t_cylinder	*last;

	new = create_cylinder(line);
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
