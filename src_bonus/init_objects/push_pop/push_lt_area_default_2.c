#include "minirt.h"

void	adj_pntrs_copy2(t_light *to_copy, t_light *new)
{
	new->next = to_copy->next;
	new->prev = to_copy;
	to_copy->next->prev = new;
	to_copy->next = new;
}
