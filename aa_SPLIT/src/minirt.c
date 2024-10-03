#include "minirt.h"

//find closest object hit by ray

static inline void	find_closest(t_trace *trace, t_ray ray, t_track_hits *closest)
{
	double	t;

	closest->t = INFINITY;
	closest->object = NULL;
	closest->object_type = -1;
	t = INFINITY;
	check_spheres(trace->spheres, closest, ray, &t);
	t = INFINITY;
	check_planes(trace->planes, closest, ray, &t);
	t = INFINITY;
	check_cylinders(trace->cylinders, closest, ray, &t);
}

//checking for the closest intersection and computing color

static inline unsigned int	check_intersects(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_norm_color	color;
	t_color			clamped;
	
	find_closest(trace, r, closest);

	if (closest->t != INFINITY && closest->object_type == SPHERE)
		color = color_sphere(trace, r, closest);
	else if (closest->t != INFINITY && closest->object_type == PLANE)
		color = color_plane(trace, r, closest);
	else if (closest->t != INFINITY && closest->object_type == CYLINDER)
		color = color_cylinder(trace, r, closest);
	else
		return (0);//backround
	clamped.r = clamp_color(color.r);
	clamped.g = clamp_color(color.g);
	clamped.b = clamp_color(color.b);
	return (clamped.r << 16 | clamped.g << 8 | clamped.b);
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
			color = check_intersects(trace, r, closest);
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
