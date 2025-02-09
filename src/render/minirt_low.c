#include "minirt.h"

//***---------------Repeated functions for inline opti-----------------***/

static inline void	my_pixel_put1(int x, int y, t_img *img, unsigned int color)
{
	int	offset;

	offset = (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)(img->pixels_ptr + offset) = color;
}

static inline t_vec3	add_vec1(t_vec3 vec1, t_vec3 vec2)
{
	t_vec3	res;

	res.x = vec1.x + vec2.x;
	res.y = vec1.y + vec2.y;
	res.z = vec1.z + vec2.z;
	res.w = vec1.w + vec2.w;
	return (res);
}

static inline t_vec3	scale_vec1(double scalar, t_vec3 vec)
{
	t_vec3	res;

	res.x = scalar * vec.x;
	res.y = scalar * vec.y;
	res.z = scalar * vec.z;
	res.w = 0;
	return (res);
}

static inline t_vec3	norm_vec1(t_vec3 vec)
{
	t_vec3	normed;
	double	length;

	length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (!length)
		return (vec);
	normed.x = vec.x / length;
	normed.y = vec.y / length;
	normed.z = vec.z / length;
	normed.w = 0;
	return (normed);
}

static inline uint8_t	clamp_color1(double color)
{
	if (color >= 255)
		return (255);
	if (color < 0)
		return (0);
	else
		return ((uint8_t)(color));
}

static inline t_vec3	subtract_vec1(t_vec3 vec1, t_vec3 vec2)
{
	t_vec3	res;

	res.x = vec1.x - vec2.x;
	res.y = vec1.y - vec2.y;
	res.z = vec1.z - vec2.z;
	res.w = vec1.w - vec2.w;
	return (res);
}

static inline t_norm_color	color1(double r, double g, double b)
{
	t_norm_color	col;

	col.r = r;
	col.g = g;
	col.b = b;
	return (col);
}

//***---------------------------End repeats----------------------------***/

//low resolution mode for speed

void	find_closest_l(t_trace *trace, t_ray ray, t_intersects *intersects)
{
	int	i;

	i = 0;
	intersects->closest->t = INFINITY;
	intersects->closest->object = NULL;
	intersects->closest->object_type = VOID;
	intersects->count = 0;
	check_spheres(trace->spheres, intersects, ray);
	check_cylinders(trace->cylinders, intersects, ray);
	check_hyperboloids(trace->hyperboloids, intersects, ray);
	check_cubes(trace->cubes, intersects, ray);
	check_planes(trace->planes, intersects, ray);
	check_arealts(trace->lights, intersects, ray);
	while (i < intersects->count && intersects->hits[i].t <= 0)
		i++;
	if (i < intersects->count)
		*(intersects->closest) = intersects->hits[i];
}

static inline unsigned int	clamped_col_l(t_norm_color col)
{
	t_color	clamped;

	clamped.r = clamp_color1(col.r);
	clamped.g = clamp_color1(col.g);
	clamped.b = clamp_color1(col.b);
	return (clamped.r << 16 | clamped.g << 8 | clamped.b);
}

//checking for the closest intersection and computing color
//if switch obj colors to norm_col, color_out = mult_color(255.0, color_out);

t_norm_color	check_intersects_l(t_trace *trace, t_ray r, \
	t_intersects *intersects, t_depths depths)
{
	t_norm_color	color_out;
	t_track_hits	*closest;

	if (depths.refl <= 0 && depths.refr <= 0)
		return (color1(0, 0, 0));
	find_closest_l(trace, r, intersects);
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
		return (color1(0, 0, 0));
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
	color = clamped_col_l(check_intersects_l(trace, r, intersects, \
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
			color = clamped_col_l(check_intersects_l(trace, r, intersects, trace->depths));
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
		color = clamped_col_l(check_intersects_l(trace, r, intersects, trace->depths));
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