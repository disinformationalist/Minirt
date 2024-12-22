#include "minirt.h"

void	increment_option(t_trace *trace, int *option)
{
	(*option)++;
	if (*option == 1 && !trace->textures)
		(*option)++;
	if (*option == 3)
		*option = 0;
}

//rotating through opts, color, texture, pattern

void	select_option(t_trace *trace, t_on *on)
{
	if (on->type == PLANE)
		increment_option(trace, &trace->curr_pl->option);
	else if (on->type == SPHERE)
		increment_option(trace, &trace->curr_sp->option);
	else if (on->type == CYLINDER)
		increment_option(trace, &trace->curr_cy->option);
	else if (on->type == CUBE)
		increment_option(trace, &trace->curr_cu->option);
	else if (on->type == HYPERBOLOID)
		increment_option(trace, &trace->curr_hy->option);
	else
		return ;
}

int	get_option(t_on *on)
{
	int	option;

	if (on->type == SPHERE)
		option = ((t_sphere *)on->object)->option;
	else if (on->type == PLANE)
		option = ((t_plane *)on->object)->option;
	else if (on->type == CYLINDER)
		option = ((t_cylinder *)on->object)->option;
	else if (on->type == CUBE)
		option = ((t_cube *)on->object)->option;
	else if (on->type == HYPERBOLOID)
		option = ((t_hyperboloid *)on->object)->option;
	else
		option = 0;
	return (option);
}

//handle mouse hooks,//try making a struct that contains the stuff for hooks sep of trace

int	mouse_press(int button, int x, int y, t_trace *trace)
{
	t_norm_color	curr_col;
	int				option;

	if (button == 1)
		track_object(trace, x, y);
	if (trace->on->object == NULL)
		return (0);
	option = get_option(trace->on);
	if (button == 3)
		select_option(trace, trace->on);
	else if (button == 4 || button == 5)
	{
		if (option == 0)
			rotate_colors(trace, button, &curr_col);
		else if (option == 1)
			set_next_tx(button, trace->textures, trace->on);
	}
	render(trace);
	return (0);
}
