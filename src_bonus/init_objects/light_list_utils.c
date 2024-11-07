#include "minirt.h"

void	set_sl_vals(t_light *new, char **line)
{
	char	*bright_ratio;
	
	bright_ratio = line[3];
	new->type = SPOT;
	new->center = get_coordinates(line[1], 1.0);
	new->dir = neg(get_coordinates(line[2], 0.0));
	new->brightness = get_double(&bright_ratio);
	if (line[4])
	{
		new->color = get_color(line[4], 255.0);
		new->inner_cone = cos(ft_atoi(line[5]) * DEG_TO_RAD);
		new->outer_cone = cos(ft_atoi(line[6]) * DEG_TO_RAD);
	}
	else 
	{
		new->color = color(1.0, 1.0, 1.0);
		new->inner_cone = cos(ft_atoi(line[4]) * DEG_TO_RAD);
		new->outer_cone = cos(ft_atoi(line[5]) * DEG_TO_RAD);	
	}
	if (new->inner_cone == new->outer_cone)
		new->inv_conediff = 0;
	else
		new->inv_conediff = 1.0 / (new->inner_cone - new->outer_cone);
}

void	set_lt_vals(t_light *new, char **line)
{
	char	*bright_ratio;

	bright_ratio = line[2];
	new->type = POINT;
	new->center = get_coordinates(line[1], 1.0);
	new->brightness = get_double(&bright_ratio);
	new->brightness = 1.0;
	if (line[3])
		new->color = get_color(line[3], 255.0);
	else 
		new->color = color(1.0, 1.0, 1.0);
}

t_light *create_light(t_trace *trace, char **line)
{
	t_light *new;

	new = (t_light *)malloc(sizeof(t_light));
	if (!new)
		return (NULL);
	if (!ft_strcmp(line[0], "L"))
		set_lt_vals(new, line);
	else if (!ft_strcmp(line[0], "SL"))
		set_sl_vals(new, line);
	else if (set_al_vals(trace, new, line))
	{
		free(new);
		return (NULL);
	}
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_light(t_trace *trace, t_light **start, char **line)
{
	t_light *new;
	t_light *last;

	new = create_light(trace, line);
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