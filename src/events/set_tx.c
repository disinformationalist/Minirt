#include "minirt.h"

void	set_new_tx(t_on *on, t_tx *new)
{
	if (on->type == PLANE)
		((t_plane *)on->object)->texture = new;
	else if (on->type == SPHERE)
		((t_sphere *)on->object)->texture = new;
	else if (on->type == CYLINDER)
		((t_cylinder *)on->object)->texture = new;
	else if (on->type == CUBE)
		((t_cube *)on->object)->texture = new;
	else if (on->type == HYPERBOLOID)
		((t_hyperboloid *)on->object)->texture = new;
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
	else if (on->type == CYLINDER)
		curr_tx = ((t_cylinder *)on->object)->texture;
	else if (on->type == CUBE)
		curr_tx = ((t_cube *)on->object)->texture;
	else if (on->type == HYPERBOLOID)
		curr_tx = ((t_hyperboloid *)on->object)->texture;
	else
		return ;
	while (tx != curr_tx && tx->next != textures)
		tx = tx->next;
	if (button == 4)
		set_new_tx(on, tx->next);
	else if (button == 5)
		set_new_tx(on, tx->prev);
}
