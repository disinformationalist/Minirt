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

int	new_img_init(void *mlx_con, t_img *img, int width, int height)
{
	img->img_ptr = mlx_new_image(mlx_con,
			width, height);
	if (img->img_ptr == NULL)
		return (-1);
	img->pixels_ptr = mlx_get_data_addr(img->img_ptr,
			&img->bpp, &img->line_len,
			&img->endian);
	return (0);
}

void info_init(t_trace *trace)
{
	trace->width = 860;
	trace->height = (int)((double)trace->width / ASPECT);
	trace->curr_sp = trace->spheres;
	trace->curr_pl = trace->planes;
	trace->curr_cy = trace->cylinders;
	trace->supersample = false;
	trace->layer = false;
	trace->n = 3.0;
	init_viewing(trace);
}

static void events_init(t_trace *trace)
{
	mlx_hook(trace->mlx_win, KeyPress, KeyPressMask, key_press, trace);
	mlx_hook(trace->mlx_win, DestroyNotify, StructureNotifyMask, close_win, trace);
}

void trace_init(t_trace *trace)
{
	info_init(trace);
	trace->mlx_connect = mlx_init();
	if (trace->mlx_connect == NULL)
	{
		free_all_objects(trace);
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
	trace->on->object = trace->spheres;
	trace->on->type = SPHERE;
	events_init(trace);
	set_sp_transforms(trace);
	set_pl_transforms(trace);
	set_cy_transforms(trace);
}
