#include "minirt.h"

void	set_pl_vals(t_plane *new, char **line)
{
	new->point = get_coordinates(line[1], 1.0);
	new->norm = get_coordinates(line[2], 0.0);
	new->color = get_color(line[3], 1.0);	
}

t_plane	*create_plane(char **line)
{
	t_plane	*new;

	new = (t_plane *)malloc(sizeof(t_plane));
	if (!new)
		return (NULL);
	set_pl_vals(new, line);
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_pl(t_plane **start, char **line)
{
	t_plane	*new;
	t_plane	*last;

	new = create_plane(line);
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