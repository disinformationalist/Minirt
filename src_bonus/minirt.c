#include "minirt.h"

//find closest object hit by ray

void	find_closest(t_trace *trace, t_ray ray, t_track_hits *closest)
{
	double	t;

	closest->t = INFINITY;
	closest->object = NULL;
	closest->object_type = -1;
	t = INFINITY;
	check_spheres(trace->spheres, closest, ray, &t);
	t = INFINITY;
		check_lenses(trace->lenses, closest, ray, &t);
	t = INFINITY;
	check_planes(trace->planes, closest, ray, &t);
	t = INFINITY;
	check_cylinders(trace->cylinders, closest, ray, &t);
}


unsigned int	clamped_col(t_norm_color col)
{
	t_color clamped;
	
	clamped.r = clamp_color(col.r);
	clamped.g = clamp_color(col.g);
	clamped.b = clamp_color(col.b);

	return (clamped.r << 16 | clamped.g << 8 | clamped.b);
}

//checking for the closest intersection and computing color

t_norm_color	check_intersects(t_trace *trace, t_ray r, t_track_hits *closest, int depth)
{
	t_norm_color	color_out;

	if (depth <= 0)
		return (color(0, 0, 0));
	
	find_closest(trace, r, closest);

	if (closest->t != INFINITY && closest->object_type == SPHERE)
		color_out = color_sphere(trace, r, closest, depth);
	else if (closest->t != INFINITY && closest->object_type == LENS)
		color_out = color_lens(trace, r, closest);
	else if (closest->t != INFINITY && closest->object_type == PLANE)
		color_out = color_plane(trace, r, closest, depth);
	else if (closest->t != INFINITY && closest->object_type == CYLINDER)
		color_out = color_cylinder(trace, r, closest);
	else
		return (color(0, 0, 0));
	return (color_out);
}

static inline void	compute_pixels(t_trace *trace, t_piece *piece, t_track_hits *closest)
{
	t_ray			r;
	t_point			current_pixel;
	t_position		pos;
	unsigned int	color;

	r.origin = trace->cam->center;
	pos.j = piece->y_s - 1;
	while (++pos.j < piece->y_e)
	{
		current_pixel = trace->pixel00;
		current_pixel = add_vec(current_pixel, scale_vec(pos.j, trace->pix_delta_down));
		pos.i = piece->x_s - 1;
		while (++pos.i < piece->x_e)
		{
			r.dir = norm_vec(subtract_vec(current_pixel, r.origin));
			color = clamped_col(check_intersects(trace, r, closest, trace->depth));
			//color = check_intersects(trace, r, closest);
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
	compute_pixels(trace, piece, piece->closest);
	pthread_exit(NULL);
}
