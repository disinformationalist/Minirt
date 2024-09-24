#include "minirt.h"

void	set_le_vals(t_lens *new, char **line)
{
	char *diam_str_1;
	char *diam_str_2;

	diam_str_1 = line[2];
	new->sphere_1.center = get_coordinates(line[1], 1.0);
	new->sphere_1.radius = get_double(&diam_str_1) / 2.0;
	new->sphere_1.color = get_color(line[3], 1.0);	
	diam_str_2 = line[5];
	new->sphere_2.center = get_coordinates(line[4], 1.0);
	new->sphere_2.radius = get_double(&diam_str_2) / 2.0;
	new->sphere_2.color = get_color(line[6], 1.0);	
}

t_lens *create_lens(char **line)
{
	t_lens *new;

	new = (t_lens *)malloc(sizeof(t_lens));
	if (!new)
		return (NULL);
	set_le_vals(new, line);
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_le(t_lens **start, char **line)
{
	t_lens *new;
	t_lens *last;

	new = create_lens(line);
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
