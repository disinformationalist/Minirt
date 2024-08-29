#include "minirt.h"
/*----------------------VIEWPORT DIAGRAM---------------------------
			 |\
 			 | \
 			 |  \ 
 			 |   \
 			 |	  \==> viewport width
			/|\    \
 		   / | \	\				+-----------------+
 		  /	 |	\	 \				| SCENE GOES HERE |
		 /	 |	 \	  \				+-----------------+
(cam){o}----------\	  |				   
  		  |   \	   \  |				
   		  |    \	\ | 
    	  | 	\	 \|
      focal_len	 \	  |==> viewport height
 (dash from cam)  \	  |
	   			   \  |
					\ |
					 \|

 view_width = 2 * focal_len * tan(horizontal_FOV / 2)// moved focal len adj.
 -----------------------------------------------------------------*/


void	set_vectors(t_trace *trace, double focal_len)
{
	t_vec3 up_vec;
	t_vec3 view_center;
	t_vec3 horizontal;
	t_vec3 vertical;

	up_vec.x = 0;//set world up direction
	up_vec.y = 1;
	up_vec.z = 0;

	//right and up basis vecs
	trace->u_vec = normalize_vec(cross_prod(up_vec, trace->cam->orient));
	trace->v_vec = normalize_vec(cross_prod(trace->cam->orient, trace->u_vec));


	//for set view center
	horizontal = scalar_mult_vec(trace->view_width / 2.0, trace->u_vec);
	vertical = scalar_mult_vec(trace->view_height / 2.0, trace->v_vec);

	view_center = subtract_vec(trace->cam->center, scalar_mult_vec(1.0 / focal_len, trace->cam->orient));
	trace->view_topleft = add_vec(view_center, vertical);//move up 
	trace->view_topleft = subtract_vec(trace->view_topleft, horizontal);//move_left

	//set pix00 location
	trace->pix_delta_u = scalar_mult_vec(trace->pixel_width, trace->u_vec);
	trace->pix_delta_v = scalar_mult_vec(trace->pixel_height, trace->v_vec);

	//the adjust by .5 is for pixel centers
	trace->pixel00 = add_vec(trace->view_topleft, scalar_mult_vec(0.5, trace->pix_delta_u));
	trace->pixel00 = add_vec(trace->pixel00, scalar_mult_vec(0.5, trace->pix_delta_v));
}

// recall init_viewing when moving or rotating the cam
void	init_viewing(t_trace *trace)
{
	double focal_len;

	focal_len = 1.0;

	trace->view_width = 2.0 * tan((double)(trace->cam->fov / 2) * trace->deg_to_rad);
	trace->view_height = trace->view_width / trace->aspect_r;
	
	// scale pixels to veiwport
	trace->pixel_width = trace->view_width / (double)trace->width;
	trace->pixel_height = trace->view_height / (double)trace->height;

	set_vectors(trace, focal_len);
}


void info_init(t_trace *trace)
{
	//use the fov horizontal and aspect_r to set view width, height;
	trace->deg_to_rad = M_PI / 180.0;
	trace->aspect_r = 16.0 / 9.0;

	
	trace->width = 1920;//control screen size by setting width and the aspect_r
	trace->height = (int)((double)trace->width / trace->aspect_r);

	trace->height_orig = trace->height;
	trace->width_orig = trace->width;
	
	trace->move_x = 0.0;
	trace->move_y = 0.0;
	trace->move_z = 0.0;

	trace->zoom = 1;
	trace->supersample = false;
	trace->s_kernel = 5;
	trace->layer = false;
	trace->n = 1.0;//for muliple rays per pixel... not using yet...
	init_viewing(trace);
}

//not using any of the commented out events yet, may not need them.

static void events_init(t_trace *trace)
{
	mlx_hook(trace->mlx_win, KeyPress, KeyPressMask, key_press, trace);
	//mlx_hook(trace->mlx_win, KeyRelease, KeyReleaseMask, key_release, trace);
//	mlx_hook(trace->mlx_win, ButtonPress, ButtonPressMask, mouse_press, trace);
	//mlx_hook(trace->mlx_win, ButtonRelease, ButtonReleaseMask, mouse_release, trace);
	//mlx_hook(trace->mlx_win, MotionNotify, PointerMotionMask, mouse_move, trace);
	mlx_hook(trace->mlx_win, DestroyNotify, StructureNotifyMask, close_win, trace);
	/* mlx_loop_hook(trace->mlx_connect, render, trace);
	mlx_loop(trace->mlx_connect); */
}

void trace_init(t_trace *trace)
{
	info_init(trace);
	trace->mlx_connect = mlx_init();
	if (trace->mlx_connect == NULL)
		exit(EXIT_FAILURE);
	trace->mlx_win = mlx_new_window(trace->mlx_connect, trace->width, trace->height, trace->name);
	if (trace->mlx_win == NULL)
		clear_all(trace);
	if (new_img_init(trace->mlx_connect, &trace->img, trace->width, trace->height) == -1)
		clear_all(trace);
	if (pthread_mutex_init(&trace->mutex, NULL) != 0)
		clear_all(trace);
	events_init(trace);
}