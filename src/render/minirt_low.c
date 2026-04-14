#include "minirt.h"
#include "mainloop_fts.h"

//low resolution mode for speed

void	find_closest_l(t_trace *trace, t_ray ray, t_intersects *intersects)
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


//checking for the closest intersection and computing color
//if switch obj colors to norm_col, color_out = mult_color(255.0, color_out);

t_norm_color	check_intersects_l(t_trace *trace, t_ray r, \
	t_intersects *intersects, t_depths depths)
{
	t_norm_color	color_out;
	t_track_hits	*closest;

/* 	if (depths.refl <= 0 && depths.refr <= 0)
		return (color1(0, 0, 0)); */
	find_closest_l(trace, r, intersects);
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
/* 	if (closest->t != INFINITY && closest->object_type == SPHERE)
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
		return (color1(0, 0, 0)); */
	return (color_out);
}

static inline void	set_pixels(t_trace *trace, t_position pos, 
								unsigned int color, int xlim, int ylim)
{
	int	k;
	int	l;

	k = -1;
	while (++k < ylim)
	{
		l = -1;
		while (++l < xlim)
			my_pixel_put1(pos.i + l, pos.j + k, &trace->img, color);
	}
}

static inline void	set_lastx_pixels(t_trace *trace, t_position pos, t_ray r, t_intersects *intersects, t_point curr_pix, unsigned int color, int xlim, int ylim)
{
	r.dir = norm_vec1(subtract_vec1(curr_pix, r.origin));
	color = clamped_col1(check_intersects_l(trace, r, intersects, \
	trace->depths));
	set_pixels(trace, pos, color, xlim, ylim);
}


static inline void	compute_pixels_l(t_trace *trace, t_piece *piece, \
									t_intersects *intersects)
{
	//unsigned int	color = piece->thread_color;//for thread testing
	t_ray			r;
	unsigned int	color;
	t_point			current_pixel;
	t_point			row_start;
	t_position		pos;
	int				incx = trace->m_lowinc;//can set these 2 to any
	int				incy = trace->m_lowinc;
	t_vec3			pix_delta_rh = scale_vec1(incx, trace->pix_delta_rht);
	t_vec3			pix_delta_dn = scale_vec1(incy, trace->pix_delta_down);
	int				xs = piece->x_s;
	int				xe = piece->x_e;
	int				ys = piece->y_s;
	int				ye = piece->y_e;
	int				x_blocks = (xe - xs) / incx;
	int				y_blocks = (ye - ys) / incy;
	int				x_remain = (xe - xs) % incx;
	int				y_remain = (ye - ys) % incy;
	int 			k, l;

	pos.j = ys;
	r.origin = trace->cam->center;
	row_start = add_vec1(trace->pixel00, scale_vec1(xs, trace->pix_delta_rht));
	row_start = add_vec1(row_start, scale_vec1(ys, trace->pix_delta_down));
	k = -1;
	while (++k < y_blocks)
	{
		current_pixel = row_start;
		pos.i = xs;
		l = -1;
		while (++l < x_blocks)
		{
			r.dir = norm_vec1(subtract_vec1(current_pixel, r.origin));
			color = clamped_col1(check_intersects_l(trace, r, intersects, trace->depths));
			set_pixels(trace, pos, color, incx, incy);
			current_pixel = add_vec1(current_pixel, pix_delta_rh);
			pos.i += incx;
		}
		set_lastx_pixels(trace, pos, r, intersects, current_pixel, color, x_remain, incy);
		row_start = add_vec1(row_start, pix_delta_dn);
		pos.j += incy;
	}
	//last row for piece
	current_pixel = row_start;
	pos.i = xs;
	l = -1;
	while (++l < x_blocks)
	{
		r.dir = norm_vec1(subtract_vec1(current_pixel, r.origin));
		color = clamped_col1(check_intersects_l(trace, r, intersects, trace->depths));
		set_pixels(trace, pos, color, incx, y_remain);
		current_pixel = add_vec1(current_pixel, pix_delta_rh);
		pos.i += incx;
	}
	set_lastx_pixels(trace, pos, r, intersects, current_pixel, color, x_remain, y_remain);

	//account for bridge piece....single recursive call when applicable
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
		compute_pixels_l(trace, &piece2, piece2.intersects);
	}
}

//routine to loop through all pixels and compute.

void	*ray_trace_l(void *arg)
{
	t_piece			*piece;
	t_trace			*trace;

	piece = (t_piece *)arg;
	trace = piece->trace;
	compute_pixels_l(trace, piece, piece->intersects);
	pthread_exit(NULL);
}