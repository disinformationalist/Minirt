#include "minirt.h"
#include "mainloop_fts.h"

void	find_closest(t_trace *trace, t_ray ray, t_intersects *intersects)
{
	int	i;

	i = 0;
	intersects->closest->t = INFINITY;
	intersects->closest->object = NULL;
	intersects->closest->object_type = VOID;
	intersects->count = 0;
/* 	check_spheres(trace->spheres, intersects, ray);
	check_cylinders(trace->cylinders, intersects, ray);
	check_hyperboloids(trace->hyperboloids, intersects, ray);
	check_cubes(trace->cubes, intersects, ray);
	check_planes(trace->planes, intersects, ray); */
/* 	if (ray_box_intersect(trace->box, trace->group->transform, ray))//initial sp test good, doesnt work for planes.
		check_group(trace->group, intersects, ray); */
	
	check_hierarchy(trace->bvh, intersects, ray);
	check_planes(trace->planes, intersects, ray);
	//check_arealts(trace->lights, intersects, ray);//checking in cubes heirarchy
	if (trace->sp_box)
		ray_sphere_intersect(trace->spheres, ray, intersects);
	while (i < intersects->count && intersects->hits[i].t <= 0)
		i++;
	if (i < intersects->count)
		*(intersects->closest) = intersects->hits[i];
}

//checking for the closest intersection and computing color

t_norm_color	check_intersects(t_trace *trace, t_ray r, \
	t_intersects *intersects, t_depths depths)
{
	t_norm_color	color_out;
	t_track_hits	*closest;

	/* if (depths.refl <= 0 && depths.refr <= 0)
		return (color1(0, 0, 0));//can just return a color here for a nonadjustable sp at infinity */
	find_closest(trace, r, intersects);
	closest = intersects->closest;
	if (closest->t == INFINITY)
		return (color1(0, 0, 0));
	if (closest->object_type == SPHERE)
		color_out = color_sphere(trace, r, intersects, depths);
	else if (closest->object_type == PLANE)
		color_out = color_plane(trace, r, intersects, depths);
	else if (closest->object_type == CYLINDER)
		color_out = color_cylinder(trace, r, intersects, depths);
	else if (closest->object_type == HYPERBOLOID)
		color_out = color_hyperboloid(trace, r, intersects, depths);
	else if (closest->object_type == CUBE)
		color_out = color_cube(trace, r, intersects, depths);
	else
		return (color1(0, 0, 0));
	return (color_out);
}

static inline void	compute_pixels(t_trace *trace, t_piece *piece, \
	t_intersects *intersects)
{
	t_ray			r;
	unsigned int	color;
	t_point			current_pixel;
	t_point			row_start;
	t_position		pos;

	r.origin = trace->cam->center;
	pos.j = piece->y_s - 1;
	row_start = add_vec1(trace->pixel00, scale_vec1(piece->x_s, trace->pix_delta_rht));
	while (++pos.j < piece->y_e)
	{
		current_pixel = add_vec1(row_start, scale_vec1(pos.j, trace->pix_delta_down));
		pos.i = piece->x_s - 1;
		while (++pos.i < piece->x_e)
		{
			r.dir = norm_vec1(subtract_vec1(current_pixel, r.origin));
			color = clamped_col1(check_intersects(trace, r, intersects, \
				trace->depths));
			my_pixel_put1(pos.i, pos.j, &trace->img, color);
			current_pixel = add_vec1(current_pixel, trace->pix_delta_rht);
		}
	}
	if (piece->bridge)
	{
		t_piece	piece2;

		piece2.x_s = piece->x_s2;
		piece2.x_e = piece->x_e2;
		piece2.y_s = piece->y_s2;
		piece2.y_e = piece->y_e2;
		piece2.bridge = false;
		piece2.thread_color = piece->thread_color;
		piece2.intersects = piece->intersects;
		compute_pixels(trace, &piece2, piece2.intersects);
	}
}


void	find_closest_testing(t_trace *trace, t_ray ray, t_intersects *intersects)
{
	int	i;

	i = 0;
	intersects->closest->t = INFINITY;
	intersects->closest->object = NULL;
	intersects->closest->object_type = VOID;
	intersects->count = 0;
	
	check_hierarchy_testing(trace->bvh, intersects, ray);
	check_planes(trace->planes, intersects, ray);
	//check_arealts(trace->lights, intersects, ray);//todo optional show physical light
	if (trace->sp_box)
		ray_sphere_intersect(trace->spheres, ray, intersects);
	while (i < intersects->count && intersects->hits[i].t <= 0)
		i++;
	if (i < intersects->count)
		*(intersects->closest) = intersects->hits[i];
}

t_norm_color	check_intersects_testing(t_trace *trace, t_ray r, \
	t_intersects *intersects, t_depths depths)
{
	t_norm_color	color_out;
	t_track_hits	*closest;

	find_closest_testing(trace, r, intersects);
	closest = intersects->closest;
	if (closest->t == INFINITY)
		return (color1(0, 0, 0));
	if (closest->object_type == SPHERE)
		color_out = color_sphere_testing(trace, r, intersects, depths);
	else if (closest->object_type == PLANE)
		color_out = color_plane_testing(trace, r, intersects, depths);
	else if (closest->object_type == CYLINDER)
		color_out = color_cylinder_testing(trace, r, intersects, depths);
	else if (closest->object_type == HYPERBOLOID)
		color_out = color_hyperboloid_testing(trace, r, intersects, depths);
	else if (closest->object_type == CUBE)
		color_out = color_cube_testing(trace, r, intersects, depths);
	else
		return (color1(0, 0, 0));
	return (color_out);
}

static inline void	compute_pixels_testing(t_trace *trace, t_piece *piece, \
	t_intersects *intersects)
{
	t_ray			r;
	unsigned int	color;
	t_point			current_pixel;
	t_point			row_start;
	t_position		pos;

	r.origin = trace->cam->center;
	pos.j = piece->y_s - 1;
	row_start = add_vec1(trace->pixel00, scale_vec1(piece->x_s, trace->pix_delta_rht));
	while (++pos.j < piece->y_e)
	{
		current_pixel = add_vec1(row_start, scale_vec1(pos.j, trace->pix_delta_down));
		pos.i = piece->x_s - 1;
		while (++pos.i < piece->x_e)
		{
			intersects->stats.camera_rays++;//bvh tracking
			r.dir = norm_vec1(subtract_vec1(current_pixel, r.origin));
			color = clamped_col1(check_intersects_testing(trace, r, intersects, \
				trace->depths));
			my_pixel_put1(pos.i, pos.j, &trace->img, color);
			current_pixel = add_vec1(current_pixel, trace->pix_delta_rht);
		}
	}
	if (piece->bridge)
	{
		t_piece	piece2;

		piece2.x_s = piece->x_s2;
		piece2.x_e = piece->x_e2;
		piece2.y_s = piece->y_s2;
		piece2.y_e = piece->y_e2;
		piece2.bridge = false;
		piece2.thread_color = piece->thread_color;
		piece2.intersects = piece->intersects;
		compute_pixels_testing(trace, &piece2, piece2.intersects);
	}
}


//routine to loop through all pixels and compute.

void	*ray_trace(void *arg)
{
	t_piece			*piece;
	t_trace			*trace;

	piece = (t_piece *)arg;
	trace = piece->trace;
	if (trace->bvh_testing)
		compute_pixels_testing(trace, piece, piece->intersects);
	else
		compute_pixels(trace, piece, piece->intersects);
	pthread_exit(NULL);
}
