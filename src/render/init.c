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
	//trace->height = 1080;
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
	trace->low_flag = false;
	trace->menu_open = false;
	trace->on_menu = false;
	trace->n = 4.0;
	trace->low_inc = 10;
	trace->stash = false;
	trace->depths.refl = 6;
	trace->depths.refr = 6;
	trace->m_lowinc = 16;
	trace->knob = -1;
	init_viewing(trace);
}

static inline void	set_on(t_trace *trace, void *obj, t_type type)
{
	trace->on->object = obj;
	trace->on->type = type;
}

static void	events_init(t_trace *trace)
{
	if (trace->spheres)
		set_on(trace, trace->spheres, SPHERE);
	else if (trace->planes)
		set_on(trace, trace->planes, PLANE);
	else if (trace->cylinders)
		set_on(trace, trace->cylinders, CYLINDER);
	else if (trace->hyperboloids)
		set_on(trace, trace->hyperboloids, HYPERBOLOID);
	else if (trace->cubes)
		set_on(trace, trace->cubes, CUBE);
	else if (trace->lights)
		set_on(trace, trace->lights, LIGHT);
	else
		set_on(trace, NULL, VOID);
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
	if (import_textures(trace->mlx_connect, trace->textures))
		clear_all(trace);
	trace->obj_control = make_controls(trace->mlx_connect);
	if (!trace->obj_control)
		clear_all(trace);
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
}
