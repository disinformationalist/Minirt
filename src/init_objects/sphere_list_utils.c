#include "minirt.h"

void	set_sp_vals(t_sphere *new, char **line)
{
	char *diam_str;

	diam_str = line[2];
	new->center = get_coordinates(line[1]);
	new->radius = get_double(&diam_str) / 2.0;
	new->color = get_color(line[3], 1.0);	
}

t_sphere *create_sphere(char **line)
{
	t_sphere *new;

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
	t_sphere *new;
	t_sphere *last;

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

void	update_sphere_ids(t_sphere *sphere)
{
	while (sphere->id != 1)
	{
		sphere->id++;
		sphere = sphere->next;
	}
}

//if list is empty, make a default sphere and set it as the current object

void make_default_sp(t_sphere **start, t_sphere *new)
{
	new->center = vec(0.000,-0.500,-6.500);
	new->radius = 1.5;
	new->color.r = 200;
	new->color.g = 50;
	new->color.b = 200;
	*start = new;
	new->id = 1;
	new->next = new;
	new->prev = new;
}

//copy a sphere and place it immediately after the current sphere in the list

bool	insert_spcopy_after(t_trace *trace, t_sphere **current)
{
	t_sphere	*sp_to_copy;
	t_sphere	*new;

	new = (t_sphere *)malloc(sizeof(t_sphere));
	if (!new)
		return (true);
	if (!*current)
	{
		make_default_sp(&trace->spheres, new);
		trace->on->object = trace->spheres;
		trace->on->type = SPHERE;
		return (false);
	}
	sp_to_copy = *current;
	*new = *sp_to_copy;//copy contents
	new->next = sp_to_copy->next;
	new->prev = sp_to_copy;
	sp_to_copy->next->prev = new;
	sp_to_copy->next = new;
	new->id = sp_to_copy->id + 1;
	update_sphere_ids(new->next);
	return (false);
}
