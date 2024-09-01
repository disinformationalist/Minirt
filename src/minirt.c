#include "minirt.h"

void	put_pixel(int x, int y, t_trace *trace, int color)
{
	if (trace->supersample)
		trace->pixels_xl[y][x] = color;
	else
		my_pixel_put(x, y, &trace->img, color);
}

void	find_closest(t_trace *trace, t_ray ray, t_track_hits *closest)
{
	double			t;

	closest->t = INFINITY;
	closest->object = NULL;
	closest->object_type =  -1;

	check_spheres(trace->spheres, closest, ray, &t); //see trace_spheres.c
	check_planes(trace->planes, closest, ray, &t); //see trace_planes.c
	//check_cylinders();//todo

}

void	check_intersects(t_trace *trace, t_ray r, int i, int j, t_track_hits *closest)
{
	unsigned int	final_color;
	
	find_closest(trace, r, closest);

	if (closest->t != INFINITY && closest->object_type == SPHERE)
		final_color = color_sphere(trace, r, closest);
	else if (closest->t != INFINITY && closest->object_type == PLANE)
		final_color = color_plane(trace, r, closest);
	/* 
	else if (closest->t != INFINITY && closest->object_type == CYLINDER)
		final_color = color_cylinder(trace, r, closest);//todo */
	else
		final_color = 0x000000;//background color here
	put_pixel(i, j, trace, final_color);
}


//routine to loop through all pixels and compute.

void	*ray_trace(void *arg)
{
	t_piece	*piece;
	t_trace	*trace;
	int		i;
	int		j;
	t_vec3	current_pixel;
	t_ray			r;

	piece = (t_piece *)arg;
	trace = piece->trace;

	t_track_hits *closest;
	closest = (t_track_hits *)malloc(sizeof(t_track_hits));//maybe move into piece set proper frees for each thread.
	if (!closest)
		clear_all(trace);

	r.origin = trace->cam->center;
	j = piece->y_s - 1;//each thread is working within its limits in piece structs
	while (++j < piece->y_e)
	{
		current_pixel = trace->pixel00;
		current_pixel.y -= j * trace->pixel_height;
		i = piece->x_s - 1;
		while (++i < piece->x_e)
		{
			r.direction = subtract_vec(current_pixel, r.origin);
			check_intersects(trace, r, i, j, closest);
			current_pixel.x += trace->pixel_width;
		}
	}
	free(closest);
	pthread_exit(NULL);
}

		//current_pixel = subtract_vec(current_pixel, scalar_mult_vec(j * trace->pixel_height, trace->v_vec));//old y shift

			//current_pixel = add_vec(current_pixel, trace->pix_delta_u);//move along width to next pixel//old x shift



//sphere
/* (bx^2 +by^2 + bz^2)t^2 +  (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
a = ray origin
b = ray direction
r = radius
t = hit distance

double a = dot(raydir, raydir);
double b = 2.0 * dot (rayorigin, raydirection)
double c = dot(rayorigin, rayorigin) - radius*radius

*/