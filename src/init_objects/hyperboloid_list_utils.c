#include "minirt.h"

void	set_hy_vals(t_hyperboloid *new, char **line)
{
	char	*rad_str;
	char	*waist_str;
	char	*height_str;	

	rad_str = line[3];
	waist_str = line[4];
	height_str = line[5];
	new->center = get_coordinates(line[1], 1.0);
	new->norm = norm_vec(get_coordinates(line[2], 0.0));
	new->rad = get_double(&rad_str);
	new->waist_val = get_double(&waist_str);
	new->waist3 = new->waist_val * new->waist_val * new->waist_val;
	new->height = get_double(&height_str);
	new->half_h = new->height / 2.0;
	new->color = get_color(line[6], 1.0);
	new->mat = get_mat(ENAMEL);
	new->shadow = true;
	new->bump = false;
	new->w_frost = false;
	new->option = 0;
	new->texture = NULL;
	new->bump_level = DEFAULT_BUMP;
	new->caps = true;
}

t_hyperboloid	*create_hyperboloid(char **line)
{
	t_hyperboloid	*new;

	new = (t_hyperboloid *)malloc(sizeof(t_hyperboloid));
	if (!new)
		return (NULL);
	set_hy_vals(new, line);
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_hy(t_hyperboloid **start, char **line)
{
	t_hyperboloid	*new;
	t_hyperboloid	*last;

	new = create_hyperboloid(line);
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
