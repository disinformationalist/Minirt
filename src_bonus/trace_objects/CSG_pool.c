#include "minirt.h"

void	check_csg_sphere_list(t_trace *trace, t_csg_op op, t_intersects *intersects)
{
	bool			inl;
	bool			inr;
	t_helper_shape	*temp_shape;
	t_csg			*temp_csg;
	t_sphere		*current;
	t_sphere		*start;

	inl = false;
	inr = false;
	current = trace->spheres;
	if (!current)
		return ;
	start = current;
	while (current && current->next)
	{
		temp_shape = (t_helper_shape *)current;
		temp_csg = make_new_csg(temp_shape, (t_helper_shape *)current->next, op);
		filter_intersections(temp_csg, &intersects, &inl, &inr);
		current = current->next;
		if (current == start)
			break ;
	}
	free(temp_csg);
}

void	check_csg_cyl_list(t_trace *trace, t_csg_op op, t_intersects *intersects)
{
	bool			inl;
	bool			inr;
	t_helper_shape	*temp_shape;
	t_csg			*temp_csg;
	t_cylinder		*current;
	t_cylinder		*start;

	inl = false;
	inr = false;
	current = trace->cylinders;
	if (!current)
		return ;
	start = current;
	while (current && current->next)
	{
		temp_shape = (t_helper_shape *)current;
		temp_csg = make_new_csg(temp_shape, (t_helper_shape *)current->next, op);
		filter_intersections(temp_csg, &intersects, &inl, &inr);
		current = current->next;
		if (current == start)
			break ;
	}
	free(temp_csg);
}

void	check_csg_hyp_list(t_trace *trace, t_csg_op op, t_intersects *intersects)
{
	bool			inl;
	bool			inr;
	t_helper_shape	*temp_shape;
	t_csg			*temp_csg;
	t_hyperboloid		*current;
	t_hyperboloid		*start;

	inl = false;
	inr = false;
	current = trace->hyperboloids;
	if (!current)
		return ;
	start = current;
	while (current && current->next)
	{
		temp_shape = (t_helper_shape *)current;
		temp_csg = make_new_csg(temp_shape, (t_helper_shape *)current->next, op);
		filter_intersections(temp_csg, &intersects, &inl, &inr);
		current = current->next;
		if (current == start)
			break ;
	}
	free(temp_csg);
}

void	check_csg_cube_list(t_trace *trace, t_csg_op op, t_intersects *intersects)
{
	bool			inl;
	bool			inr;
	t_helper_shape	*temp_shape;
	t_csg			*temp_csg;
	t_cube			*current;
	t_cube			*start;

	inl = false;
	inr = false;
	current = trace->cubes;
	if (!current)
		return ;
	start = current;
	while (current && current->next)
	{
		temp_shape = (t_helper_shape *)current;
		temp_csg = make_new_csg(temp_shape, (t_helper_shape *)current->next, op);
		filter_intersections(temp_csg, &intersects, &inl, &inr);
		current = current->next;
		if (current == start)
			break ;
	}
	free(temp_csg);
}