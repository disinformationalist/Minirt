#include "minirt.h"

static inline void	put_pixel(int x, int y, t_trace *trace, int color)
{
	if (trace->supersample)
		trace->pixels_xl[y][x] = color;
	else
		my_pixel_put(x, y, &trace->img, color);
}

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
		check_lenses(trace->lenses, closest, ray, &t);
	t = INFINITY;
	check_planes(trace->planes, closest, ray, &t);
	t = INFINITY;
	check_cylinders(trace->cylinders, closest, ray, &t);
	
}

//checking for the closest intersection and computing color

static inline void	check_intersects(t_trace *trace, t_ray r, t_position pos, t_track_hits *closest)
{
	unsigned int	final_color;
	
	find_closest(trace, r, closest);
	if (closest->t != INFINITY && closest->object_type == SPHERE)
		final_color = color_sphere(trace, r, closest);
	else if (closest->t != INFINITY && closest->object_type == LENS)
		final_color = color_lens(trace, r, closest);
	else if (closest->t != INFINITY && closest->object_type == PLANE)
		final_color = color_plane(trace, r, closest);
	else if (closest->t != INFINITY && closest->object_type == CYLINDER)
		final_color = color_cylinder(trace, r, closest);
	else
		final_color = 0x000000;//background color here
	put_pixel(pos.i, pos.j, trace, final_color);
}

static inline void	compute_pixels(t_trace *trace, t_piece *piece, t_track_hits *closest)
{
	t_ray		r;
	t_point		current_pixel;
	t_position	pos;

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
			check_intersects(trace, r, pos, closest);
			current_pixel = add_vec(current_pixel, trace->pix_delta_rht);
		}
	}
}

//routine to loop through all pixels and compute.

void	*ray_trace(void *arg)
{
	t_piece			*piece;
	t_trace			*trace;
	//t_track_hits 	*closest;

	piece = (t_piece *)arg;
	trace = piece->trace;
	/* closest = (t_track_hits *)malloc(sizeof(t_track_hits));
	if (!closest)
		clear_all(trace); */
	compute_pixels(trace, piece, piece->closest);
	pthread_exit(NULL);
}

//attempting to set up other super sample method, not done or working yet....
/* 
static inline unsigned int	check_intersects(t_trace *trace, t_ray r, t_position pos, t_track_hits *closest)
{
	unsigned int	final_color;
	
	(void)pos;
	find_closest(trace, r, closest);

	if (closest->t != INFINITY && closest->object_type == SPHERE)
		final_color = color_sphere(trace, r, closest);
	else if (closest->t != INFINITY && closest->object_type == PLANE)
		final_color = color_plane(trace, r, closest);
	else if (closest->t != INFINITY && closest->object_type == CYLINDER)
		final_color = color_cylinder(trace, r, closest);
	else
		final_color = 0x000000;//background color here
	return (final_color);
	//put_pixel(pos.i, pos.j, trace, final_color);
}


static inline void	compute_pixels(t_trace *trace, t_piece *piece, t_track_hits *closest, t_position pos)
{
	t_ray		r;
	t_point		current_pixel;
	int			n = 1;
	int			k = -1;
	int			l = -1;
	unsigned int color = 0;
	t_vec_2 offset;

	offset.x = trace->pixel_width / n;
	offset.y = trace->pixel_height / n;

	
	r.origin = trace->cam->center;
	pos.j = piece->y_s - 1;
	while (++pos.j < piece->y_e)
	{
		current_pixel = trace->pixel00;
		current_pixel.y -= pos.j * trace->pixel_height;
		pos.i = piece->x_s - 1;
		while (++pos.i < piece->x_e)
		{
			while(++l < n)
			{
				current_pixel.y += offset.y;
				while (++k < n)
				{
				current_pixel.x += offset.x;
			
				r.dir = norm_vec(subtract_vec(current_pixel, r.origin));//normed now
				//r.dir.x = .5 * r.dir.x; //to toy with transform
				//r.dir = subtract_vec(current_pixel, r.origin);
				color += check_intersects(trace, r, pos, closest);
				current_pixel.x += trace->pixel_width;
				}
			}
			color = color / (n * n);
			my_pixel_put(pos.i, pos.j, &trace->img, color);
		}
	}
} */