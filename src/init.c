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

//setting the top left pixel the center of the corresponding view section

	/* left = cross_prod(ori_vec, norm_vec(up));
	true_up = cross_prod(left, ori_vec);
	tuple_to_row(&orient, vec(left.x, left.y, left.z, 0), 0);
	tuple_to_row(&orient, vec(true_up.x, true_up.y, true_up.z, 0), 1);
	tuple_to_row(&orient, vec(-ori_vec.x, -ori_vec.y, -ori_vec.z, 0), 2);
	tuple_to_row(&orient, vec(0, 0, 0, 1), 3);
	return (mat_mult(orient, translation(-from.x, -from.y, -from.z)));
 */


static inline void	set_pixel00(t_trace *trace, t_point view_topleft, t_vec3 right, t_vec3 true_up)
{
	//set change in /v per pixel along view directions
	trace->pix_delta_rht = scale_vec(trace->pixel_width, right);
	trace->pix_delta_down = scale_vec(trace->pixel_height, neg(true_up));
	trace->pixel00 = add_vec(view_topleft, scale_vec(0.5, add_vec(trace->pix_delta_down, trace->pix_delta_rht)));
}

//for z rot, rotate the viewwindow ( right and up)
//add rotZ to struct and rotate, that much on the right and up. init to zero
static inline void	set_view_topleft(t_trace *trace, t_vec3 view_center, double view_width, double view_height)
{
	t_point	view_topleft;
	t_vec3	horizontal_move;
	t_vec3	vertical_move;
	t_vec3	right;
	t_vec3	true_up;
	
	//right and up basis vecs
	if (!veccmp(trace->cam->orient, vec(0, 1, 0, 0)))
	{
		right = vec(1, 0, 0, 0);
		true_up = vec(0, 1, 0, 0);
	}
	else if (!veccmp(trace->cam->orient, vec(0, -1, 0, 0)))
	{
		right = vec(-1, 0, 0, 0);
		true_up = vec(0, -1, 0, 0);
	}
	else
	{
		right = norm_vec(cross_prod(vec(0, 1, 0, 0), trace->cam->orient));
		true_up = norm_vec(cross_prod(trace->cam->orient, right));
	}
	//if (dot_product(trace->cam->orient, vec(0, 0, 1, 0)) < 0)this works for vert but not horiz. 
	//	right = neg(right);
	//for set view center
	horizontal_move = scale_vec(view_width / 2.0, right);
	vertical_move = scale_vec(view_height / 2.0, true_up);
	view_topleft = add_vec(view_center, vertical_move);
	view_topleft = subtract_vec(view_topleft, horizontal_move);
	set_pixel00(trace, view_topleft, right, true_up);
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
	// scale pixels to veiwport
	trace->pixel_width = view_width / (double)trace->width;
	trace->pixel_height = view_height / (double)trace->height;
	
	set_view_topleft(trace, view_center, view_width, view_height);
}
	/* printf("h_move: %.3f\n", horizontal_move);
	printf("v_move: %.3f\n", vertical_move); */
	/* print_vec(horizontal_move);
	print_vec(vertical_move); */

//setup for transform way(very slow seems to have no big advantage keep until sure)
/* 
void	init_viewing(t_trace *trace)
{
	double	half_view;
	double	aspect;

	half_view = tan(((double)(trace->cam->fov) / 2.0) * DEG_TO_RAD);
	aspect = (double)trace->width / (double)trace->height;
	
	if (aspect >= 1)
	{
		trace->cam->half_width = half_view;
		trace->cam->half_height = half_view / aspect;	
	}
	else
	{
		trace->cam->half_width = half_view * aspect;
		trace->cam->half_height = half_view;
	}
	
	// scale pixels to veiwport
	trace->cam->pixel_size = 2.0 * trace->cam->half_width / trace->cam->width;
	//set_view_topleft(trace, view_center, view_width, view_height);
} */

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
	//append_sl(trace->spotlights);///------------------
	events_init(trace);

	set_sp_transforms(trace);
	set_pl_transforms(trace);
	set_cy_transforms(trace);
}