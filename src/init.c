#include "minirt.h"
/*-------------VIEWPORT DIAGRAM-----------Best I could manage in ascii chars :(
			 |\
 			 | \				Using LEFT Handed Coordinates(for everything)
 			 |  \ 
 			 |   \								Y+ (up screen)
 			 |	  \==> viewport width			|
			/|\    \							|	
 		   / | \	\	+-----------------+		| 
 		  /	 |	\	 \	| SCENE GOES HERE |		\--------Z+ (into screen)
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

void	set_pixel00(t_trace *trace, t_point view_topleft, t_vec3 right, t_vec3 true_up)
{
	trace->pix_delta_rht = scale_vec(trace->pixel_width, right);
	trace->pix_delta_down = scale_vec(trace->pixel_height, neg(true_up));
	trace->pixel00 = add_vec(view_topleft, scale_vec(0.5, add_vec(trace->pix_delta_down, trace->pix_delta_rht)));
	trace->cam->transform_up = rot_to(vec(0, 1, 0, 0), trace->cam->true_up);
}

static inline void	set_view_topleft(t_trace *trace, t_vec3 view_center, double view_width, double view_height)
{
	t_point	view_topleft;
	t_vec3	horizontal_move;
	t_vec3	vertical_move;
	t_vec3	right;
	
	if (!veccmp(trace->cam->orient, vec(0, 1, 0, 0)))
	{
		right = vec(1, 0, 0, 0);
		trace->cam->true_up = vec(0, 0, -1, 0);
	}
	else if (!veccmp(trace->cam->orient, vec(0, -1, 0, 0)))
	{
		right = vec(1, 0, 0, 0);
		trace->cam->true_up = vec(0, 0, 1, 0);
	}
	else
	{
		right = norm_vec(cross_prod(vec(0, 1, 0, 0), trace->cam->orient));
		trace->cam->true_up = norm_vec(cross_prod(trace->cam->orient, right));
	}
	horizontal_move = scale_vec(view_width / 2.0, right);
	vertical_move = scale_vec(view_height / 2.0, trace->cam->true_up);
	view_topleft = add_vec(view_center, vertical_move);
	view_topleft = subtract_vec(view_topleft, horizontal_move);
	set_pixel00(trace, view_topleft, right, trace->cam->true_up);
}

void	init_viewing(t_trace *trace)
{
	t_point	view_center;
	double	focal_len;
	double	view_width;
	double	view_height;
	
	trace->cam->orient = norm_vec(trace->cam->orient);
	focal_len = 1.0;
	view_center = add_vec(trace->cam->center, scale_vec(1.0 / focal_len, trace->cam->orient));
	view_width = 2.0 * tan((double)((trace->cam->fov) / 2.0) * DEG_TO_RAD);
	view_height = view_width / ASPECT;
	trace->pixel_width = view_width / (double)trace->width;
	trace->pixel_height = view_height / (double)trace->height;	
	set_view_topleft(trace, view_center, view_width, view_height);
}

void info_init(t_trace *trace)
{
	//use the fov horizontal and aspect ratio to set view width, height;
	
	trace->width = 1080;//control screen size by setting width and the aspect_r
	//trace->height = 640;
	
	/* trace->cam->width = 1080;//adding to cam, eventually just need this one.
	trace->cam->height = 640; */
	trace->height = (int)((double)trace->width / ASPECT);

	trace->height_orig = trace->height;
	trace->width_orig = trace->width;
	
	trace->curr_sp = trace->spheres;
	trace->curr_pl = trace->planes;
	trace->curr_cy = trace->cylinders;

	trace->supersample = false;
	trace->s_kernel = 5;
	trace->layer = false;
	trace->n = 1.0;//for muliple rays per pixel... not using yet...
	trace->cam->transform = rot_to(vec(0, 0, 1, 0), trace->cam->orient);


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
		clear_all(trace);
	trace->mlx_win = mlx_new_window(trace->mlx_connect, trace->width, trace->height, "***MiniRT***");
	if (trace->mlx_win == NULL)
		clear_all(trace);
	if (new_img_init(trace->mlx_connect, &trace->img, trace->width, trace->height) == -1)
		clear_all(trace);
	trace->on = (t_on *)malloc(sizeof(t_on));
	if (!trace->on)
		clear_all(trace);
	trace->on->object = trace->spheres;
	trace->on->type = SPHERE;
	if (pthread_mutex_init(&trace->mutex, NULL) != 0)
		clear_all(trace);
	events_init(trace);
	set_sp_transforms(trace);
	set_pl_transforms(trace);
	set_cy_transforms(trace);
	//cam_transform(trace->cam);
}