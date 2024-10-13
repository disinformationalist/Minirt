#include "minirt.h"

void	set_le_vals(t_lens *new, char **line)
{
	char *diam_str_1;
	char *diam_str_2;

	diam_str_1 = line[2];
	new->sphere_1.center = get_coordinates(line[1], 1.0);
	new->sphere_1.radius = get_double(&diam_str_1) / 2.0;
	new->sphere_1.color = get_color(line[3], 1.0);
	new->sphere_1.mat = get_mat(DEFAULT);

	diam_str_2 = line[5];
	new->sphere_2.center = get_coordinates(line[4], 1.0);
	new->sphere_2.radius = get_double(&diam_str_2) / 2.0;
	new->sphere_2.color = get_color(line[6], 1.0);
	new->sphere_2.mat = get_mat(DEFAULT);


}
void	calculate_more_vals(t_lens *new)
{
	double	d1; // distance from C1 to the lens center
	t_vec3	center_offset;
	double	r1_sq;
	double	r2_sq;
	t_vec3	axis_vec;

	new->d = fabs(magnitude(subtract_vec(new->sphere_1.center, new->sphere_2.center)));
	d1 = (pow(new->sphere_1.radius, 2) - pow(new->sphere_2.radius, 2) + pow(new->d, 2)) / (2 * new->d);
	axis_vec = subtract_vec(new->sphere_2.center, new->sphere_1.center);
	center_offset = scale_vec(d1 / new->d, axis_vec);
	new->center = add_vec(new->sphere_1.center, center_offset);
	r1_sq = pow(new->sphere_1.radius, 2);
	r2_sq = pow(new->sphere_2.radius, 2);
	new->radius = sqrt(r1_sq - pow((r1_sq - r2_sq + pow(new->d, 2)) / (2 * new->d), 2));
	new->axis = norm_vec(axis_vec);
}

t_lens *create_lens(char **line)
{
	t_lens *new;

	new = (t_lens *)malloc(sizeof(t_lens));
	if (!new)
		return (NULL);
	set_le_vals(new, line);
	calculate_more_vals(new); 
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
