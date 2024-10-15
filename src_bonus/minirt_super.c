#include "minirt.h"

static inline void	find_closest_s(t_trace *trace, t_ray ray, t_intersects *intersects)
{
	int i;

	i = 0;
	intersects->closest->t = INFINITY;
	intersects->closest->object = NULL;
	intersects->closest->object_type = -1;
	intersects->count = 0;
	check_spheres(trace->spheres, intersects, ray);
	//check_lenses(trace->lenses, intersects, closest, ray);
	check_planes(trace->planes, intersects, ray);
	check_cylinders(trace->cylinders, intersects, ray);

	while (i < intersects->count && intersects->hits[i].t <= 0)
		i++;
	if (i < intersects->count)
	*(intersects->closest) = intersects->hits[i];
}


static inline t_norm_color	check_intersects_s(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths)
{
	t_norm_color	color_out;

	if (depths.refl <= 0 && depths.refr <= 0)
		return (color(0, 0, 0));
	
	find_closest_s(trace, r, intersects);
	t_track_hits	*closest = intersects->closest;//temp

	if (closest->t != INFINITY && closest->object_type == SPHERE)
		color_out = color_sphere(trace, r, intersects, depths);
	/* else if (closest->t != INFINITY && closest->object_type == LENS)
		color_out = color_lens(trace, r, closest); */
	else if (closest->t != INFINITY && closest->object_type == PLANE)
		color_out = color_plane(trace, r, intersects, depths);
	else if (closest->t != INFINITY && closest->object_type == CYLINDER)
		color_out = color_cylinder(trace, r, intersects, depths);
	else
		return (color(0, 0, 0));
	return (color_out);
}


static inline t_norm_color sum_subpixels(t_trace *trace, t_ray r, t_intersects *intersects, t_vec3 currpix)
{
	int				k;
	int				l;
	t_norm_color	sum;
	t_vec3			subpix;
	t_vec3			row_start;

	sum = color(0, 0, 0);
	row_start = currpix;
	l = -1;
	while (++l < trace->n)
	{
		k = -1;
		subpix = row_start;
		while (++k < trace->n)
		{
			r.dir = norm_vec(subtract_vec(subpix, r.origin));
			sum = sum_rgbs(sum, check_intersects_s(trace, r, intersects, trace->depths));
			subpix = add_vec(subpix, trace->move_x);
		}
		row_start = add_vec(row_start, trace->move_y);
	}
	return (sum);
}

static inline void	compute_pixels(t_trace *trace, t_piece *piece, t_intersects *intersects)
{
	t_ray			r;
	t_point			current_pixel;
	t_position		pos;
	unsigned int	color;
	t_norm_color	sum;

	color = 0;
	r.origin = trace->cam->center;
	pos.j = piece->y_s - 1;
	while (++pos.j < piece->y_e)
	{
		current_pixel = trace->pixel00;
		current_pixel = add_vec(current_pixel, scale_vec(pos.j, trace->pix_delta_down));
		pos.i = piece->x_s - 1;
		while (++pos.i < piece->x_e)
		{
			sum = sum_subpixels(trace, r, intersects, current_pixel);
			color = avg_samples(sum, trace->n2);
			my_pixel_put(pos.i, pos.j, &trace->img, color);
			current_pixel = add_vec(current_pixel, trace->pix_delta_rht);
		}
	}
}

//routine to loop through all pixels and compute.

void	*ray_trace_s(void *arg)
{
	t_piece			*piece;
	t_trace			*trace;

	piece = (t_piece *)arg;
	trace = piece->trace;
	compute_pixels(trace, piece, piece->intersects);
	pthread_exit(NULL);
}