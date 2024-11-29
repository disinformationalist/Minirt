#include "minirt.h"

void	find_closest(t_trace *trace, t_ray ray, t_intersects *intersects)
{
	int	i;

	i = 0;
	intersects->closest->t = INFINITY;
	intersects->closest->object = NULL;
	intersects->closest->object_type = -1;
	intersects->count = 0;
	check_spheres(trace->spheres, intersects, ray);
	check_cylinders(trace->cylinders, intersects, ray);
	check_hyperboloids(trace->hyperboloids, intersects, ray);
	check_cubes(trace->cubes, intersects, ray);
	check_planes(trace->planes, intersects, ray);
	check_arealts(trace->lights, intersects, ray);
	//check_csg((t_helper_shape *)trace->spheres, (t_helper_shape *)trace->hyperboloids, (t_helper_shape *)trace->spheres->next, intersects);
	//check_csg((t_helper_shape *)trace->spheres->next->next, (t_helper_shape *)trace->hyperboloids->next, (t_helper_shape *)trace->spheres->next->next->next, intersects);
	//check_csg(trace->spheres, trace->cylinders, intersects);
	while (i < intersects->count && intersects->hits[i].t <= 0)
		i++;
	if (i < intersects->count)
		*(intersects->closest) = intersects->hits[i];
}

unsigned int	clamped_col(t_norm_color col)
{
	t_color	clamped;

	clamped.r = clamp_color(col.r);
	clamped.g = clamp_color(col.g);
	clamped.b = clamp_color(col.b);
	return (clamped.r << 16 | clamped.g << 8 | clamped.b);
}

//checking for the closest intersection and computing color

t_norm_color	check_intersects(t_trace *trace, t_ray r, \
	t_intersects *intersects, t_depths depths)
{
	t_norm_color	color_out;
	t_track_hits	*closest;

	if (depths.refl <= 0 && depths.refr <= 0)
		return (color(0, 0, 0));
	find_closest(trace, r, intersects);
	closest = intersects->closest;
	if (closest->t != INFINITY && closest->object_type == SPHERE)
		color_out = color_sphere(trace, r, intersects, depths);
	else if (closest->t != INFINITY && closest->object_type == PLANE)
		color_out = color_plane(trace, r, intersects, depths);
	else if (closest->t != INFINITY && closest->object_type == CYLINDER)
		color_out = color_cylinder(trace, r, intersects, depths);
	else if (closest->t != INFINITY && closest->object_type == HYPERBOLOID)
		color_out = color_hyperboloid(trace, r, intersects, depths);
	else if (closest->t != INFINITY && closest->object_type == CUBE)
		color_out = color_cube(trace, r, intersects, depths);
	else
		return (color(0, 0, 0));
	return (color_out);
}

static inline void	compute_pixels(t_trace *trace, t_piece *piece, \
	t_intersects *intersects)
{
	t_ray			r;
	unsigned int	color;
	t_point			current_pixel;
	t_position		pos;

	r.origin = trace->cam->center;
	pos.j = piece->y_s - 1;
	while (++pos.j < piece->y_e)
	{
		current_pixel = trace->pixel00;
		current_pixel = add_vec(current_pixel, scale_vec(pos.j, \
			trace->pix_delta_down));
		pos.i = piece->x_s - 1;
		while (++pos.i < piece->x_e)
		{
			r.dir = norm_vec(subtract_vec(current_pixel, r.origin));
			color = clamped_col(check_intersects(trace, r, intersects, \
				trace->depths));
			my_pixel_put(pos.i, pos.j, &trace->img, color);
			current_pixel = add_vec(current_pixel, trace->pix_delta_rht);
		}
	}
}

//routine to loop through all pixels and compute.

void	*ray_trace(void *arg)
{
	t_piece			*piece;
	t_trace			*trace;

	piece = (t_piece *)arg;
	trace = piece->trace;
	compute_pixels(trace, piece, piece->intersects);
	pthread_exit(NULL);
}
