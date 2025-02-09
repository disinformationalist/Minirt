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

static inline unsigned int	avg_samples1(t_norm_color sum, double n)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	r = clamp_color1(sum.r / n);
	g = clamp_color1(sum.g / n);
	b = clamp_color1(sum.b / n);
	return (r << 16 | g << 8 | b);
}

static inline t_norm_color	sum_rgbs1(t_norm_color sum, t_norm_color to_add)
{
	sum.r += to_add.r;
	sum.g += to_add.g;
	sum.b += to_add.b;
	return (sum);
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
	check_spheres(trace->spheres, intersects, ray);
	check_planes(trace->planes, intersects, ray);
	check_cylinders(trace->cylinders, intersects, ray);
	check_hyperboloids(trace->hyperboloids, intersects, ray);
	check_cubes(trace->cubes, intersects, ray);
	check_arealts(trace->lights, intersects, ray);
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

	if (depths.refl <= 0 && depths.refr <= 0)
		return (color1(0, 0, 0));
	find_closest_s(trace, r, intersects);
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

static inline void	compute_pixels(t_trace *trace, t_piece *piece, \
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
