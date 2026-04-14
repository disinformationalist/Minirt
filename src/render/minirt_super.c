#include "minirt.h"
#include "mainloop_fts.h"


//super sample version in separate for optimization

static inline void	find_closest_s(t_trace *trace, t_ray ray, \
	t_intersects *intersects)
{
	int	i;

	i = 0;
	intersects->closest->t = INFINITY;
	intersects->closest->object = NULL;
	intersects->closest->object_type = VOID;
	intersects->count = 0;

	check_hierarchy(trace->bvh, intersects, ray);
	check_planes(trace->planes, intersects, ray);
	//check_arealts(trace->lights, intersects, ray);
	if (trace->sp_box)
		ray_sphere_intersect(trace->spheres, ray, intersects);
	while (i < intersects->count && intersects->hits[i].t <= 0)
		i++;
	if (i < intersects->count)
		*(intersects->closest) = intersects->hits[i];
}

static inline t_norm_color	check_intersects_s(t_trace *trace, t_ray r, \
	t_intersects *intersects, t_depths depths)
{
	t_norm_color	color_out;
	t_track_hits	*closest;

	/* if (depths.refl <= 0 && depths.refr <= 0)
		return (color1(0, 0, 0)); */
	find_closest_s(trace, r, intersects);
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

//sampling each pixel multiple times, summing results

static inline t_norm_color	sum_subpixels(t_trace *trace, t_ray r, \
	t_intersects *intersects, t_vec3 currpix)
{
	int				k;
	int				l;
	t_norm_color	sum;
	t_vec3			subpix;
	t_vec3			row_start;

	sum = color1(0, 0, 0);
	row_start = currpix;
	l = -1;
	while (++l < trace->n)
	{
		k = -1;
		subpix = row_start;
		while (++k < trace->n)
		{
			r.dir = norm_vec1(subtract_vec1(subpix, r.origin));
			sum = sum_rgbs1(sum, check_intersects_s(trace, \
			r, intersects, trace->depths));
			subpix = add_vec1(subpix, trace->move_x);
		}
		row_start = add_vec1(row_start, trace->move_y);
	}
	return (sum);
}

//threshold comparison tool

static inline float get_diff(t_norm_color f, t_norm_color s)
{
	return (fabs(f.r - s.r) + fabs(f.g - s.g) + fabs(f.b - s.b));
}

//basic adaptive version, use centers from previous pass, run check and super where needed. working

static inline void	compute_pixels(t_trace *trace, t_piece *piece,
	t_intersects *intersects, t_norm_color *centers)
{
	t_ray			r;
	t_point			current_pixel;
	t_point			row_start;
	t_position		pos;
	unsigned int	color;
	t_norm_color	sum;
	t_norm_color	c;
	int				w;
	int				h;
	int				idx;
	float			thresh;
	
	//recompute start inside of pix00
	t_vec3	scale;
	t_point	pixel00;

	scale = scale_vec1(1.0 / (2.0 * trace->n), trace->move);
	pixel00 = add_vec1(trace->view_topleft, scale);


	w = trace->width;
	h = trace->height;
	thresh = 0.03f;

	color = 0;
	r.origin = trace->cam->center;
	pos.j = piece->y_s - 1;
	row_start = add_vec1(pixel00, scale_vec1(piece->x_s, trace->pix_delta_rht));
	while (++pos.j < piece->y_e)
	{
		current_pixel = add_vec1(row_start,
				scale_vec1(pos.j, trace->pix_delta_down));
		pos.i = piece->x_s - 1;
		while (++pos.i < piece->x_e)
		{
			idx = pos.j * w + pos.i;
			c = centers[idx];

			if ((pos.i > 0 && get_diff(c, centers[idx - 1]) > thresh)
				|| (pos.i + 1 < w && get_diff(c, centers[idx + 1]) > thresh)
				|| (pos.j > 0 && get_diff(c, centers[idx - w]) > thresh)
				|| (pos.j + 1 < h && get_diff(c, centers[idx + w]) > thresh))
			{
				sum = sum_subpixels(trace, r, intersects, current_pixel);
				color = avg_samples1(sum, trace->n2);
			}
			else
				color = clamped_col1(c);
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
		compute_pixels(trace, &piece2, piece2.intersects, centers);
	}
}

static inline void	compute_pixel_centers(t_trace *trace, t_piece *piece, \
	t_intersects *intersects, t_norm_color *centers)
{
	t_ray			r;
	t_point			current_pixel;
	t_point			row_start;
	t_position		pos;
	int				w;

	w = trace->width;
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
			centers[pos.j * w + pos.i] = check_intersects_s(trace, r, intersects, \
				trace->depths);
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
		compute_pixel_centers(trace, &piece2, piece2.intersects, centers);
	}
}

//routine to loop through all pixels and compute.

void	*ray_trace_s(void *arg)
{
	t_piece			*piece;
	t_trace			*trace;

	piece = (t_piece *)arg;
	trace = piece->trace;
	compute_pixels(trace, piece, piece->intersects, trace->centers);
	pthread_exit(NULL);
}

//routine to loop through all pixels and compute center color struct.

void	*ray_trace_centers(void *arg)
{
	t_piece			*piece;
	t_trace			*trace;

	piece = (t_piece *)arg;
	trace = piece->trace;
	compute_pixel_centers(trace, piece, piece->intersects, trace->centers);
	pthread_exit(NULL);
}


//normal super, non adaptive. todo: recursive adaptive

/* static inline void	compute_pixels(t_trace *trace, t_piece *piece, \
	t_intersects *intersects)
{
	t_ray			r;
	t_point			current_pixel;
	t_point			row_start;
	t_position		pos;
	unsigned int	color;
	t_norm_color	sum;

	color = 0;
	r.origin = trace->cam->center;
	pos.j = piece->y_s - 1;
	row_start = add_vec1(trace->pixel00, scale_vec1(piece->x_s, trace->pix_delta_rht));
	while (++pos.j < piece->y_e)
	{
		current_pixel = add_vec1(row_start, scale_vec1(pos.j, trace->pix_delta_down));
		pos.i = piece->x_s - 1;
		while (++pos.i < piece->x_e)
		{	
			sum = sum_subpixels(trace, r, intersects, current_pixel);
			color = avg_samples1(sum, trace->n2);


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
} */