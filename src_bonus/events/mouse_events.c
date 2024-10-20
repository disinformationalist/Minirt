#include "minirt.h"

static inline double get_diff(t_norm_color c1, t_norm_color c2)
{
	double diff_sqr;
	double diff_sqg;
	double diff_sqb;

	diff_sqr = (c1.r - c2.r) * (c1.r - c2.r);
	diff_sqg = (c1.g - c2.g) * (c1.g - c2.g);
	diff_sqb = (c1.b - c2.b) * (c1.b - c2.b);
	return (sqrt(diff_sqr + diff_sqg + diff_sqb));
	//return((fabs(c1.r - c2.r) + fabs(c1.g - c2.g) + fabs(c1.b - c2.b)) / 3);
}

static inline int	match_index(int num_colors, t_norm_color *colors, t_norm_color color)
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

static inline t_norm_color	get_obj_color(t_on *on)
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
	/* else if (on->type == LENS)
		return (mult_color(1 / 255.0, ((t_lens *)on->object)->color)); */
	else
		return (color(0, 0, 0));
}

static inline void	set_obj_color(t_on *on, t_norm_color new_col)
{
	if (on->type == LIGHT)
		((t_light *)on->object)->color = new_col;
	else if (on->type == SPHERE)
		((t_sphere *)on->object)->color = mult_color(255.0, new_col);
	else if (on->type == PLANE)
		((t_plane *)on->object)->color = mult_color(255.0, new_col);
	else if (on->type == CYLINDER)
		((t_cylinder *)on->object)->color = mult_color(255.0, new_col);
	else if (on->type == CUBE)
		((t_cube *)on->object)->color = mult_color(255.0, new_col);
	/* else if (on->type == LENS)
		((t_lens *)on->object)->color = mult_color(255.0, new_col); */
	else
		return ;
}

//handle mouse hooks

int	mouse_handler(int button, int x, int y, t_trace *trace)//grayscale still in progress...
{
	t_norm_color	curr_col;
	/* double			inten;
	t_norm_color	shade; */

	(void)x;
	(void)y;
	if (trace->on->object == NULL)
		return (0);
	curr_col = get_obj_color(trace->on);

	if (!trace->layer)
	{
		if (get_diff(curr_col, trace->w_colors[trace->color_i]) > .5)
			trace->color_i = match_index(trace->num_colors, trace->w_colors, curr_col);	
		if (button == 5)
			trace->color_i = (trace->color_i + 5) % trace->num_colors;
		else if (button == 4)
			trace->color_i = (trace->color_i - 5 + trace->num_colors) % trace->num_colors;
		set_obj_color(trace->on, trace->w_colors[trace->color_i]);
	}
	/* else //scroll through textures. strg bump map on/off
	{
		inten = get_lumin(curr_col) * 255.0;
		shade = color(inten, inten, inten);
		//if (get_diff(curr_col, shade) > .1)
		//	trace->color_i = match_index(trace->num_colors, trace-)
		set_obj_color(trace->on, shade);
	} */

	render(trace);
	return (0);
}
	//CONSIDER MAKING MOUSE BUTTON TO SWITCH BETWEEN COLORS AND GRAYS. or use for two colors. or when up carat shift grays/blacks/whites  //shift textures
	//if (button == 1)
	//else if (button == 3)
		