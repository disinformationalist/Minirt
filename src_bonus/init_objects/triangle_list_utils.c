#include "minirt.h"

void	set_tri_vals(t_tri *new, char **line)
{
	new->p1 = get_coordinates(line[1], 1.0);
	new->p2 = get_coordinates(line[2], 1.0);
	new->p3 = get_coordinates(line[3], 1.0);
	new->edge1 = subtract_vec(new->p2, new->p1);
	new->edge2 = subtract_vec(new->p3, new->p1);
	new->norm = norm_vec(cross_prod(new->edge2, new->edge1));
	new->color = get_color(line[4], 1.0);
	new->mat = get_mat(DEFAULT);
	new->shadow = true;
}

t_tri	*create_tri(char **line)
{
	t_tri	*new;

	new = (t_tri *)malloc(sizeof(t_tri));
	if (!new)
		return (NULL);
	set_tri_vals(new, line);
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_tri(t_tri **start, char **line)
{
	t_tri	*new;
	t_tri	*last;

	new = create_tri(line);
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
