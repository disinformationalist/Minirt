#include "minirt.h"

static inline void	find_closest_s(t_trace *trace, t_ray ray, t_track_hits *closest)
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

static inline t_norm_color	check_intersects_s(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_norm_color	final_color;
	
	find_closest_s(trace, r, closest);

	if (closest->t != INFINITY && closest->object_type == SPHERE)
		final_color = color_sphere(trace, r, closest);
	else if (closest->t != INFINITY && closest->object_type == PLANE)
		final_color = color_plane(trace, r, closest);
	else if (closest->t != INFINITY && closest->object_type == CYLINDER)
		final_color = color_cylinder(trace, r, closest);
	else
	{
		final_color.r = 0;
		final_color.g = 0;
		final_color.b = 0;
	}
	return (final_color);
}

static inline t_norm_color sum_subpixels(t_trace *trace, t_ray r, t_track_hits *closest, t_vec3 currpix)
{
	int				k;
	int				l;
	t_norm_color	sum;
	t_vec3			subpix;
	t_vec3			row_start;

	sum.r = 0;
	sum.g = 0;
	sum.b = 0;
	l = -1;
	row_start = currpix;
	while (++l < trace->n)
	{
		k = -1;
		subpix = row_start;
		while (++k < trace->n)
		{
			r.dir = norm_vec(subtract_vec(subpix, r.origin));
			sum = sum_rgbs(sum, check_intersects_s(trace, r, closest));
			subpix = add_vec(subpix, trace->move_x);
		}
		row_start = add_vec(row_start, trace->move_y);
	}
	return (sum);
}

void	compute_pixels_s(t_trace *trace, t_track_hits *closest)
{
	t_ray			r;
	t_point			current_pixel;
	t_position		pos;
	unsigned int	color;
	t_norm_color	sum;

	color = 0;
	r.origin = trace->cam->center;
	pos.j = -1;
	while (++pos.j < trace->height)
	{
		current_pixel = trace->pixel00;
		current_pixel = add_vec(current_pixel, scale_vec(pos.j, trace->pix_delta_down));
		pos.i = -1;
		while (++pos.i < trace->width)
		{
			sum = sum_subpixels(trace, r, closest, current_pixel);
			color = avg_samples(sum, trace->n2);
			my_pixel_put(pos.i, pos.j, &trace->img, color);
			current_pixel = add_vec(current_pixel, trace->pix_delta_rht);
		}
	}
}
