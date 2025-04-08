#include "minirt.h"

void	build_group(t_trace *trace)
{
	t_sphere		*curr_sp;
	t_cylinder		*curr_cy;
	t_hyperboloid	*curr_hy;
	t_cube			*curr_cu;

	trace->bvh = group();
	if (trace->spheres)
	{
		curr_sp = trace->spheres;
		while (true)
		{
			if (!curr_sp->is_box)
				add_child(trace->bvh, curr_sp, SPHERE, curr_sp->transform, curr_sp->i_transform, NULL);
			curr_sp = curr_sp->next;
			if (curr_sp == trace->spheres)
				break ;
		}
	}
	if (trace->cylinders)
	{
		curr_cy = trace->cylinders;
		while (true)
		{
			add_child(trace->bvh, curr_cy, CYLINDER, curr_cy->transform, curr_cy->i_transform, NULL);
			curr_cy = curr_cy->next;
			if (curr_cy == trace->cylinders)
				break ;
		}
	}
	if (trace->hyperboloids)
	{
		curr_hy = trace->hyperboloids;
		while (true)
		{
			add_child(trace->bvh, curr_hy, HYPERBOLOID, curr_hy->transform, curr_hy->i_transform, NULL);
			curr_hy = curr_hy->next;
			if (curr_hy == trace->hyperboloids)
				break ;
		}
	}
	if (trace->cubes)
	{
		curr_cu = trace->cubes;
		while (true)
		{
			add_child(trace->bvh, curr_cu, CUBE, curr_cu->transform, curr_cu->i_transform, NULL);
			curr_cu = curr_cu->next;
			if (curr_cu == trace->cubes)
				break ;
		}
	}
}

//make a check print depth, count, types
void	print_hierarchy(t_group *top)
{
	t_shape	*curr;

	if (!top->shapes)
		return ;
	printf("\nDepth: %d\n---------------------------\n", top->depth);
	curr = top->shapes;
	while (true)
	{
		if (curr->type == GROUP)
			print_hierarchy((t_group *)curr);
		else
			printf("Type: %d\n", curr->type);
		curr = curr->next;
		if (curr == top->shapes)
			break;
	}
}

void	build_hierarchy(t_group *top)
{
	if (!top || !top->shapes)
		return ;
	top->box = bog(top);//assigns boxes to each shape and to top group
	divide(top, 4);//5 is just a try
	//print_hierarchy(top);
}

void	rebuild_hierarchy(t_trace *trace)
{
	free_group(trace->bvh);
	build_group(trace);//sets init group for trace->bvh before structuring
	build_hierarchy(trace->bvh);
	//trace->bvh = copy_group(trace->group);
}

void	ray_obj_intersect(t_shape *curr, t_ray ray, t_intersects *intersects)
{
	if (curr->type == SPHERE)
		ray_sphere_intersect((t_sphere *)curr->shape, ray, intersects);
	else if (curr->type == CYLINDER)
		ray_cylinder_intersect((t_cylinder *)curr->shape, ray, intersects);
	else if (curr->type == HYPERBOLOID)
		ray_hype_intersect((t_hyperboloid *)curr->shape, ray, intersects, ((t_hyperboloid *)curr->shape)->single);
	else if (curr->type == CUBE)
		ray_cube_intersect((t_cube *)curr->shape, ray, intersects);
	else if (curr->type == PLANE)
		ray_plane_intersect((t_plane *)curr->shape, ray, intersects);
	else
		return ;
}

//check_hierarchy() recursively perfroms intersection checks on a bvh.

void	check_hierarchy(t_group *top, t_intersects *intersects, t_ray ray)
{
	t_shape	*curr;

	if (!top || !top->shapes)
		return ;
	if (!ray_box_intersect(top->box, top->tran, ray))
		return ;
	curr = top->shapes;
	while (true)
	{
		if (curr->type == GROUP)
			check_hierarchy((t_group *)curr, intersects, ray);
		else
			ray_obj_intersect(curr, ray, intersects);
		curr = curr->next;
		if (curr == top->shapes)
			break ;
	}
}