#include "minirt.h"

/* void	set_sl_vals(t_light *new, char **line)//not made yet
{
	char	*bright_ratio;
	
	bright_ratio = line[2];
	new->center = get_coordinates(line[1]);
	new->brightness = get_double(&bright_ratio);
	if (line[3])
		new->color = get_color(line[3], 255.0);//used in bonus
	return (0);
} */

void	set_lt_vals(t_light *new, char **line)
{
	char	*bright_ratio;
	
	bright_ratio = line[2];
	new->center = get_coordinates(line[1]);
	new->brightness = get_double(&bright_ratio);
	if (line[3])
		new->color = get_color(line[3], 255.0);//used in bonus
}

t_light *create_light(char **line)
{
	t_light *new;

	new = (t_light *)malloc(sizeof(t_light));
	if (!new)
		return (NULL);
	//if ()
	set_lt_vals(new, line);
	//else ()
	//set_sl_vals(new, line);//for spotlight type

	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_light(t_light **start, char **line)
{
	t_light *new;
	t_light *last;

	new = create_light(line);
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