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

void	info_init(t_trace *trace)
{
	trace->width = 1080;
	trace->height = (int)((double)trace->width / ASPECT);
	trace->color_i = 0;
	trace->num_colors = 384;
	trace->curr_sp = trace->spheres;
	trace->curr_pl = trace->planes;
	trace->curr_cy = trace->cylinders;
	trace->curr_hy = trace->hyperboloids;
	trace->curr_lt = trace->lights;
	trace->curr_cu = trace->cubes;
	trace->w_colors = NULL;
	trace->m_colors = NULL;
	trace->supersample = false;
	trace->layer = false;
	trace->low_res = false;
	trace->dragging = false;
	trace->shift_on = false;
	trace->menu_open = false;
	trace->n = 4.0;
	trace->low_inc = 50;
	trace->depths.refl = 6;
	trace->depths.refr = 6;
	init_viewing(trace);
}

static void	events_init(t_trace *trace)
{
	
	if (trace->spheres)
	{
		trace->on->object = trace->spheres;
		trace->on->type = SPHERE;
	}
	else if (trace->planes)
	{
		trace->on->object = trace->planes;
		trace->on->type = PLANE;
	}
	else if (trace->cylinders)
	{
		trace->on->object = trace->cylinders;
		trace->on->type = CYLINDER;
	}
	else if (trace->hyperboloids)
	{
		trace->on->object = trace->hyperboloids;
		trace->on->type = HYPERBOLOID;
	}
	else if (trace->cubes)
	{
		trace->on->object = trace->cubes;
		trace->on->type = CUBE;
	}
	else if (trace->lights)
	{
		trace->on->object = trace->lights;
		trace->on->type = LIGHT;
	}
	else
	{
		trace->on->object = NULL;
		trace->on->type = VOID;
	}
	mlx_hook(trace->mlx_win, KeyPress, KeyPressMask, key_press, trace);
	mlx_hook(trace->mlx_win, DestroyNotify, StructureNotifyMask, close_win, trace);
	mlx_hook(trace->mlx_win, ButtonPress, ButtonPressMask, mouse_press, trace);

	mlx_hook(trace->mlx_win, MotionNotify, PointerMotionMask, mouse_move, trace);
	mlx_hook(trace->mlx_win, ButtonRelease, ButtonReleaseMask, mouse_release, trace);
	mlx_hook(trace->mlx_win, KeyRelease, KeyReleaseMask, key_release, trace);
}

void	init_transforms(t_trace *trace)
{
	set_sp_transforms(trace);
	set_pl_transforms(trace);
	set_cy_transforms(trace);
	set_hy_transforms(trace);
	set_cu_transforms(trace);
}

void	trace_init(t_trace *trace)
{
	info_init(trace);
	trace->mlx_connect = mlx_init();
	if (trace->mlx_connect == NULL)
		clear_few(trace);
	trace->mlx_win = mlx_new_window(trace->mlx_connect, \
		trace->width, trace->height, "***MegaRT***");
	if (trace->mlx_win == NULL)
		clear_some(trace);
	if (new_img_init(trace->mlx_connect, &trace->img, \
		trace->width, trace->height) == -1)
		clear_all(trace);
	trace->on = (t_on *)malloc(sizeof(t_on));
	if (!trace->on)
		clear_all(trace);
	trace->w_colors = set_color_wheel(trace->num_colors, 1.0, 0.5, 202);
	if (!trace->w_colors)
		clear_all(trace);
	trace->m_colors = set_metal_colors();
	if (!trace->m_colors)
		clear_all(trace);
	events_init(trace);
	init_transforms(trace);
	if (import_textures(trace->mlx_connect, trace->textures))
		clear_all(trace);
}
