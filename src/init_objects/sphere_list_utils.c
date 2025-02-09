#include "minirt.h"

void	set_sp_vals(t_sphere *new, char **line)
{
	char	*diam_str;

	diam_str = line[2];
	new->center = get_coordinates(line[1], 1.0);
	new->radius = get_double(&diam_str) / 2.0;
	new->color = get_color(line[3], 1.0);
	new->mat = get_mat(DEFAULT);
	new->shadow = true;
	new->bump = false;
	new->option = 0;
	new->w_frost = false;
	new->texture = NULL;
	new->bump_level = DEFAULT_BUMP;
}

t_sphere	*create_sphere(char **line)
{
	t_sphere	*new;

	new = (t_sphere *)malloc(sizeof(t_sphere));
	if (!new)
		return (NULL);
	set_sp_vals(new, line);
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_sp(t_sphere **start, char **line)
{
	t_sphere	*new;
	t_sphere	*last;

	new = create_sphere(line);
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
