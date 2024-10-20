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
//use the fov horizontal and aspect ratio to set view width, height;

void info_init(t_trace *trace)
{
	trace->width = 1080;
	trace->height = (int)((double)trace->width / ASPECT);
	
	printf("width: %d\n", trace->width);
	printf("height: %d\n", trace->height);
	trace->color_i = 0;
	trace->num_colors = 384;

	trace->curr_sp = trace->spheres;
	trace->curr_le = trace->lenses;
	trace->curr_pl = trace->planes;
	trace->curr_cy = trace->cylinders;
	trace->curr_lt = trace->lights;
	trace->curr_cu = trace->cubes;

	trace->supersample = false;
	trace->layer = false;
	trace->n = 4.0;

	trace->depths.refl = 5;
	trace->depths.refr = 5;

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
	mlx_hook(trace->mlx_win, ButtonPress, \
	ButtonPressMask, mouse_handler, trace);
	/* mlx_loop_hook(trace->mlx_connect, render, trace);
	mlx_loop(trace->mlx_connect); */
}

void init_transforms(t_trace *trace)
{
	set_sp_transforms(trace);
	set_pl_transforms(trace);
	set_cy_transforms(trace);
	set_le_transforms(trace);
	set_cu_transforms(trace);
}
//---------testing sq

double	randf(void)
{
	return ((double)rand() / (double)(RAND_MAX + 1.0));
}

void	init_sqlight(t_trace *trace, t_sqlight *light)
{
	int i;

//	light->center = vec(-7, 4.5, -9, 1);//center
	light->center = vec(0, 5, 0, 1);//center
	light->brightness = 1.0;
	light->color = color(1, 1, 1);
	light->v1 = vec(2, 0, 0, 0);//size of light in each dir
	light->v2 = vec(0, 0, 2, 0);
	light->corner = subtract_vec(light->center, div_vec(2, add_vec(light->v1, light->v2)));
	light->usteps = 8;
	light->vsteps = 8;
	light->uvec = div_vec(light->usteps, light->v1);
	light->vvec = div_vec(light->vsteps, light->v2);
	light->samples = light->usteps * light->vsteps;
	//malloced array of size of samples of randfs
	light->jitter = (double *)malloc((light->samples + 3) * sizeof(double));//this seems to be working, maybe better with one for each thread..in piece
	if (!light->jitter)
	{
		free(trace->sqlt);
		clear_all(trace);
		//free textures
	//protect
	}
	i = -1;
	while (++i < 3 + light->samples)
		light->jitter[i] = randf();
}

void trace_init(t_trace *trace)
{
	info_init(trace);
	trace->mlx_connect = mlx_init();
	if (trace->mlx_connect == NULL)
	{
		free_all_objects(trace);
		free(trace->threads);
		perror("Mlx init() failure\n");
		exit(EXIT_FAILURE);
	}
	trace->mlx_win = mlx_new_window(trace->mlx_connect, trace->width, trace->height, "***MiniRT***");
	if (trace->mlx_win == NULL)
		clear_some(trace);
	if (new_img_init(trace->mlx_connect, &trace->img, trace->width, trace->height) == -1)
		clear_all(trace);
	trace->on = (t_on *)malloc(sizeof(t_on));
	if (!trace->on)
		clear_all(trace);
	trace->w_colors = set_color_wheel(trace->num_colors, 1.0, 0.5, 202);//num colors, sat, lightness, base hue
	if (!trace->w_colors)
		clear_all(trace);//check protect here, frees where needed elsweher and closing already. need in push_object_function safeties...
	trace->on->object = trace->spheres;
	trace->on->type = SPHERE;
	events_init(trace);
	init_transforms(trace);
	

	trace->image1 = import_png(trace->mlx_connect, "image_1.png", &trace->image1_w, &trace->image1_h);//protect and protect elsewhere.
	//make some list of imgs in to manip and perform frees
	//img type specifier for bringing in images
	//mlx_destroy_image(trace->mlx_connect, trace->image1->img_ptr);
	//free(trace->image1);
	trace->sqlt = (t_sqlight *)malloc(sizeof(t_sqlight));
	if (!trace->sqlt)
		clear_all(trace);
		//free all textures
	//pro
	init_sqlight(trace, trace->sqlt);
}