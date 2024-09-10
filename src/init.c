#include "minirt.h"
/*-------------VIEWPORT DIAGRAM-----------Best I could manage in ascii chars :(
			 |\
 			 | \				Using Right Handed Coordinates(for everything)
 			 |  \ 
 			 |   \								Y+ (up screen)
 			 |	  \==> viewport width			|
			/|\    \							|	
 		   / | \	\	+-----------------+		| 
 		  /	 |	\	 \	| SCENE GOES HERE |		\--------Z- (into screen is -)
		 /	 |	 \	  \	+-----------------+	     \
(cam){o}----------\	  |		   				 	  \
  		  |   \	   \  |							   \
   		  |    \	\ | 							X+ (rightward along screen)
    	  | 	\	 \|
      focal_len	 \	  |==> viewport height
 (dash from cam)  \	  |
	   			   \  |
					\ |
					 \|

 view_width = 2 * focal_len * tan(horizontal_FOV / 2)// moved focal len adj.
 ----------------------------------------------------------------------------*/

//setting the top left pixel the center of the corresponding view section

static inline void	set_pixel00(t_trace *trace, t_point view_topleft, t_vec3 u_vec, t_vec3 v_vec)
{
	t_vec3	pix_delta_u;
	t_vec3	pix_delta_v;

	//set change in u/v per pixel along view directions
	pix_delta_u = scale_vec(trace->pixel_width, u_vec);
	pix_delta_v = scale_vec(trace->pixel_height, v_vec);

	//the adjust by .5 is for pixel centers
	trace->pixel00 = add_vec(view_topleft, scale_vec(0.5, pix_delta_u));
	trace->pixel00 = add_vec(trace->pixel00, scale_vec(0.5, pix_delta_v));
}

static inline void	set_view_topleft(t_trace *trace, t_vec3 view_center, double view_width, double view_height)
{
	t_point	view_topleft;
	t_vec3	horizontal_move;
	t_vec3	vertical_move;
	t_vec3	u_vec;
	t_vec3	v_vec;
	
	//right and up basis vecs
	u_vec = norm_vec(cross_prod(trace->cam->orient, vec(0, 1, 0)));
	v_vec = norm_vec(cross_prod(u_vec, trace->cam->orient));

	//for set view center
	horizontal_move = scale_vec(view_width / 2.0, u_vec);
	vertical_move = scale_vec(view_height / 2.0, v_vec);

	view_topleft = add_vec(view_center, vertical_move); 
	view_topleft = subtract_vec(view_topleft, horizontal_move);
	//trace->pixel00 = view_topleft;//----------for other super

	set_pixel00(trace, view_topleft, u_vec, v_vec);
}

// recall init_viewing when moving or rotating the cam
// setting up the viewport then mapping pixel00

void	init_viewing(t_trace *trace)
{
	t_point	view_center;
	double	focal_len;
	double	view_width;
	double	view_height;
	
	focal_len = 1.0;
	view_center = add_vec(trace->cam->center, scale_vec(1.0 / focal_len, trace->cam->orient));

	view_width = 2.0 * tan((double)(trace->cam->fov / 2) * DEG_TO_RAD);
	view_height = view_width / ASPECT;
	
	// scale pixels to veiwport
	trace->pixel_width = view_width / (double)trace->width;
	trace->pixel_height = view_height / (double)trace->height;

	set_view_topleft(trace, view_center, view_width, view_height);
}


void info_init(t_trace *trace)
{
	//use the fov horizontal and aspect ratio to set view width, height;
	
	trace->width = 1080;//control screen size by setting width and the aspect_r
	trace->height = (int)((double)trace->width / ASPECT);

	trace->height_orig = trace->height;
	trace->width_orig = trace->width;
	
	trace->curr_sp = trace->spheres;
	trace->curr_pl = trace->planes;
	trace->curr_cy = trace->cylinders;

	trace->supersample = false;
	trace->s_kernel = 5;
	trace->layer = false;//not used...
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
	if (trace->mlx_connect == NULL)//need to free objects here check the clear_all() function.
		clear_all(trace);
	trace->mlx_win = mlx_new_window(trace->mlx_connect, trace->width, trace->height, "***MiniRT***");
	if (trace->mlx_win == NULL)
		clear_all(trace);
	if (new_img_init(trace->mlx_connect, &trace->img, trace->width, trace->height) == -1)
		clear_all(trace);
	trace->on = (t_on *)malloc(sizeof(t_on));
	if (!trace->on)//check all protections
		clear_all(trace);
	trace->on->object = trace->spheres;
	trace->on->type = SPHERE;
	if (pthread_mutex_init(&trace->mutex, NULL) != 0)
		clear_all(trace);
	events_init(trace);
}