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
//this duplicates a sphere, placing a new duplicate immediately after in the list.NOT DONE
/* bool	insert_sp_dup_after(t_sphere **current)
{
	t_sphere	*sphere;
	t_sphere	*new;

	sphere = *current;
	//build_sp_line here.

	new = create_sphere(line);
	if (!new)
		return (true);
	//for empty list
	if (!sphere)
	{
		new->id = 1;
		sphere = new;
		return (false);
	}
	//else
	new->prev = sphere;
	new->next = sphere->next;
	sphere->next->prev = new;
	if (sphere->next == sphere)
		sphere->prev = new;
	sphere->next = new;
	if (sphere == *current)///
		*current = new;///
	new->id = sphere->id + 1;
	update_sphere_ids(new->next);
	return (false);
} */

//make a get_object_line function for insert(need one for each object type), and rt file creation.
//this needs to be able to get the values for the object after being moved, changed, etc. WAIT TO DO THIS

/* char **build_sp_line(t_sphere *sp)
{
	int		line_len;
	char 	**line;

	//convert all params to str
	char *cen_str;
	cen_str
	//count len for malloc..
	

	return (line);
}

typedef struct s_sphere
{
	int				id;
	t_vec3			center;
	double			radius;
	t_color 		color;
	struct s_sphere	*next;
	struct s_sphere *prev;
}	t_sphere; */