#include "minirt.h"

void	put_pixel(int x, int y, t_trace *trace, int color)
{
	if (trace->supersample)
		trace->pixels_xl[y][x] = color;
	else
		my_pixel_put(x, y, &trace->img, color);
}

//find closest object hit by ray

void	find_closest(t_trace *trace, t_ray ray, t_track_hits *closest)
{
	double			t;

	closest->t = INFINITY;
	closest->object = NULL;
	closest->object_type =  -1;

	check_spheres(trace->spheres, closest, ray, &t); //see trace_spheres.c
	check_planes(trace->planes, closest, ray, &t); //see trace_planes.c
	check_cylinders(trace->cylinders, closest, ray, &t);//see trace_cylinders.c

}

void	check_intersects(t_trace *trace, t_ray r, t_position pos, t_track_hits *closest)
{
	unsigned int	final_color;
	
	find_closest(trace, r, closest);

	if (closest->t != INFINITY && closest->object_type == SPHERE)
		final_color = color_sphere(trace, r, closest);
	else if (closest->t != INFINITY && closest->object_type == PLANE)
		final_color = color_plane(trace, r, closest);
	else if (closest->t != INFINITY && closest->object_type == CYLINDER)
		final_color = color_cylinder(trace, r, closest);
	else
		final_color = 0x000000;//background color here
	put_pixel(pos.i, pos.j, trace, final_color);
}


//routine to loop through all pixels and compute.

void	*ray_trace(void *arg)
{
	t_piece		*piece;
	t_trace		*trace;
	t_position	pos;
	t_point		current_pixel;
	t_ray		r;

	piece = (t_piece *)arg;
	trace = piece->trace;

	t_track_hits *closest;
	closest = (t_track_hits *)malloc(sizeof(t_track_hits));//maybe move into piece set proper frees for each thread.
	if (!closest)
		clear_all(trace);

	r.origin = trace->cam->center;
	pos.j = piece->y_s - 1;//each thread is working within its limits in piece structs
	while (++pos.j < piece->y_e)
	{
		current_pixel = trace->pixel00;
		current_pixel.y -= pos.j * trace->pixel_height;
		pos.i = piece->x_s - 1;
		while (++pos.i < piece->x_e)
		{
			r.direction = subtract_vec(current_pixel, r.origin);
			//r.direction = normalize_vec(subtract_vec(current_pixel, r.origin));//faster w/o norm, see if needed later
			check_intersects(trace, r, pos, closest);
			current_pixel.x += trace->pixel_width;
		}
	}
	free(closest);
	pthread_exit(NULL);
}

		//current_pixel = subtract_vec(current_pixel, scalar_mult_vec(j * trace->pixel_height, trace->v_vec));//old y shift

			//current_pixel = add_vec(current_pixel, trace->pix_delta_u);//move along width to next pixel//old x shift