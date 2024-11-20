#include "minirt.h"

//break off into set_tx.c

void	set_new_tx(t_on *on, t_tx *new)
{
	if (on->type == PLANE)
		((t_plane *)on->object)->texture = new;
	else if (on->type == SPHERE)
		((t_sphere *)on->object)->texture = new;
	/*else if (on->type == CYLINDER)
		((t_cylinder *)on->object)->texture = new;
	else if (on->type == HYPERBOLOID)
		((t_hyperboloid *)on->object)->texture = new;
	else if (on->type == CUBE)
		((t_cube *)on->object)->texture = new;*/
	else
		return ;
}

//rotate through textures

void	set_next_tx(int button, t_tx *textures, t_on *on)
{
	t_tx		*curr_tx;
	t_tx		*tx;

	if (textures == NULL || on == NULL)
		return ;
	tx = textures;
	if (on->type == PLANE)
		curr_tx = ((t_plane *)on->object)->texture;
	else if (on->type == SPHERE)
		curr_tx = ((t_sphere *)on->object)->texture;
	/* else if (on->type == CYLINDER)
		curr_tx = ((t_cylinder *)on->object)->texture;
	else if (on->type == HYPERBOLOID)
		curr_tx = ((t_hyperboloid *)on->object)->texture;
	else if (on->type == CUBE)
		curr_tx = ((t_cube *)on->object)->texture; */
	else
		return ;
	while (tx != curr_tx && tx->next != textures)
		tx = tx->next;
	if (button == 4)
		set_new_tx(on, tx->next);
	else if (button == 5)
		set_new_tx(on, tx->prev);
}

void	increment_option(t_trace *trace, int *option)
{
	(*option)++;
	if (*option == 1 && !trace->textures)
		(*option)++;
	if (*option == 3)
		*option = 0;
}

//only for shapes with textures/patterns/bumps, allows rotating through opts

void	select_option(t_trace *trace, t_on *on)
{
	if (on->type == PLANE)
		increment_option(trace, &trace->curr_pl->option);
 	else if (on->type == SPHERE)
		increment_option(trace, &trace->curr_sp->option);
	/* else if (on->type == CYLINDER)
		increment_option(&trace->curr_cy->option);
	else if (on->type == CUBE)
		increment_option(&trace->curr_cu->option);  */
	else
		return ;
}

int	get_option(t_on *on)
{
	int option;

	if (on->type == SPHERE)
		option = ((t_sphere *)on->object)->option;
	else if (on->type == PLANE)
		option = ((t_plane *)on->object)->option;
	else if (on->type == CYLINDER)
		option = ((t_cylinder *)on->object)->option;
	else if (on->type == CUBE)
		option = ((t_cube *)on->object)->option;
	else if (on->type == HYPERBOLOID)
		option = ((t_cube *)on->object)->option;
	else
		option = 0;
	return (option);
}

//handle mouse hooks

int	mouse_handler(int button, int x, int y, t_trace *trace)
{
	t_norm_color	curr_col;
	int				option;

	(void)x;
	(void)y;
	if (trace->on->object == NULL)
		return (0);
	option = get_option(trace->on);
	if (button == 1)
		select_option(trace, trace->on);
	else if (button == 4 || button == 5)
	{
		if (option == 0)
			rotate_colors(trace, button, &curr_col);
		else if (option == 1)
			set_next_tx(button, trace->textures, trace->on);
	}
	if (button == 3)
		if (trace->on->type == PLANE)
			trace->curr_pl->sine = !trace->curr_pl->sine;
	render(trace);
	return (0);
}
	//make a general bright in the minirt.c / super to multiply final color by up down arrow keys to adjust between 0 and 1., 20 steps possible 16steps?
	