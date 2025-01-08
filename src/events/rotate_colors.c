#include "minirt.h"

static inline double	get_diff(t_norm_color c1, t_norm_color c2)
{
	double	diff_sqr;
	double	diff_sqg;
	double	diff_sqb;

	diff_sqr = (c1.r - c2.r) * (c1.r - c2.r);
	diff_sqg = (c1.g - c2.g) * (c1.g - c2.g);
	diff_sqb = (c1.b - c2.b) * (c1.b - c2.b);
	return (sqrt(diff_sqr + diff_sqg + diff_sqb));
}

static inline int	match_index(int num_colors, t_norm_color *colors, \
	t_norm_color color)
{
	int		i;
	int		best_match;
	double	avg_diff;
	double	least_diff;

	best_match = 0;
	least_diff = INFINITY;
	i = -1;
	while (++i < num_colors)
	{
		avg_diff = get_diff(colors[i], color);
		if (avg_diff < least_diff)
		{
			best_match = i;
			least_diff = avg_diff;
		}
	}
	return (best_match);
}

t_norm_color	get_obj_color(t_on *on)
{
	if (on->type == LIGHT)
		return (((t_light *)on->object)->color);
	else if (on->type == SPHERE)
		return (mult_color(1 / 255.0, ((t_sphere *)on->object)->color));
	else if (on->type == PLANE)
		return (mult_color(1 / 255.0, ((t_plane *)on->object)->color));
	else if (on->type == CYLINDER)
		return (mult_color(1 / 255.0, ((t_cylinder *)on->object)->color));
	else if (on->type == CUBE)
		return (mult_color(1 / 255.0, ((t_cube *)on->object)->color));
	else if (on->type == HYPERBOLOID)
		return (mult_color(1 / 255.0, ((t_hyperboloid *)on->object)->color));
	else
		return (color(0, 0, 0));
}

void	set_obj_color(t_on *on, t_norm_color new_col)
{
	t_light	*lt;

	if (on->type == LIGHT)
	{
		lt = (t_light *)on->object;
		lt->color = new_col;
		if (lt->type == AREA)
			lt->emitter->color = mult_color(255.0, new_col);
	}
	else if (on->type == SPHERE)
		((t_sphere *)on->object)->color = mult_color(255.0, new_col);
	else if (on->type == PLANE)
		((t_plane *)on->object)->color = mult_color(255.0, new_col);
	else if (on->type == CYLINDER)
		((t_cylinder *)on->object)->color = mult_color(255.0, new_col);
	else if (on->type == CUBE)
		((t_cube *)on->object)->color = mult_color(255.0, new_col);
	else if (on->type == HYPERBOLOID)
		((t_hyperboloid *)on->object)->color = mult_color(255.0, new_col);
	else
		return ;
}

//rotate color of lights and objects

void	rotate_colors(t_trace *trace, int button, t_norm_color *curr)
{
	if (!trace->layer)
	{
		*curr = get_obj_color(trace->on);
		if (get_diff(*curr, trace->w_colors[trace->color_i]) > .5)
			trace->color_i = match_index(trace->num_colors, \
			trace->w_colors, *curr);
		if (button == 5)
			trace->color_i = (trace->color_i + 5) % trace->num_colors;
		else if (button == 4)
			trace->color_i = (trace->color_i - 5 + trace->num_colors) \
			% trace->num_colors;
		set_obj_color(trace->on, trace->w_colors[trace->color_i]);
	}
	else
	{
		*curr = get_obj_color(trace->on);
		if (get_diff(*curr, trace->m_colors[trace->color_i % 19]) > .5)
			trace->color_i = match_index(19, trace->m_colors, *curr);
		if (button == 5)
			trace->color_i = (trace->color_i + 1) % 19;
		else if (button == 4)
			trace->color_i = (trace->color_i + 18) % 19;
		set_obj_color(trace->on, trace->m_colors[trace->color_i]);
	}
}
