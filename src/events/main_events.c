#include "minirt.h"
/****KEY HOOK GUIDE****/
/*

1 => 4, 9, 0: switch between object lists (spheres, planes, etc...)

1 = sp list;
2 = pl list;
3 = cy list;
9 = lt list;
0 = cam;

F1 => forge rt file from current scene
F3 => save current scene to png

F1	 F3								  NUM PAD	
+-------------------------------+	+---------+
|1 2 3 4                 9 0	|	|		- | pad - prev obj on curr list
|Q W E					U I O	|	|		+ | pad + next obj on curr list
|A S D					J K L	|	|		  |
|(rot)					(move)	|	|		  |
|			  SPACE				|	|		  |
+-------------------------------+	+---------+

SPACE => supersample mode

//-------LIST TRAVERSAL

//TRANSLATION

J,L => x directions
I,K => y directions
U,O => z directions

-----------------TODO--------------------

//ROTATION

J,L => x directions
I,K => y directions
U,O => z directions

//RESIZE

//OTHER TRANSFORMATIONS?


//maybe an onscreen control board?
*/


//makes a vector and returns it
t_vec3	vec(double x, double y, double z)
{
	t_vec3	v;

	v.x = x;
	v.y = y;
	v.z = z;

	return (v);
}

//moves current "on" object in x,y,z

void	translate_object(t_trace *trace, t_on *on, t_vec3 vec)
{
	t_sphere	*sphere;
	t_plane		*plane;
	t_cylinder	*cylinder;
	t_cam		*cam;
	t_light		*light;

	if (on->type == SPHERE)
	{
		sphere = (t_sphere *)on->object;
		sphere->center = add_vec(sphere->center, vec);
	}
	else if (on->type == PLANE)
	{
		plane = (t_plane *)on->object;
		plane->point = add_vec(plane->point, vec);
	}
	else if (on->type == CYLINDER)
	{
		cylinder = (t_cylinder *)on->object;
		cylinder->center = add_vec(cylinder->center, vec);
	}
	else if (on->type == LIGHT)
	{
		light = (t_light *)on->object;
		light->center = add_vec(light->center, vec);
	}
	else if (on->type == CAM)
	{
		cam = (t_cam *)on->object;
		cam->center = add_vec(cam->center, vec);
		init_viewing(trace);
	}
}


int	close_win(t_trace *trace)//valgrind error when using the x to close window, escape key gives no such error...
{	
	pthread_mutex_destroy(&trace->mutex);
	free_all_objects(trace);
	free(trace->on);
	mlx_destroy_image(trace->mlx_connect, trace->img.img_ptr);
	mlx_destroy_window(trace->mlx_connect, trace->mlx_win);
	mlx_destroy_display(trace->mlx_connect);
	free(trace->mlx_connect);
	free(trace->threads);
	exit(EXIT_SUCCESS);
	return (0);
}

int	supersample_handle(int keycode, t_trace *trace)
{
	if (keycode == SPACE)
	{
		if (trace->supersample)
		{
			trace->width = trace->width_orig;
			trace->height = trace->height_orig;
			init_viewing(trace);
		}
		else
		{
			trace->width = trace->width * trace->s_kernel;
			trace->height = trace->height * trace->s_kernel;
			init_viewing(trace);
		}
		trace->supersample = !trace->supersample;
	}
	return (0);
}
//arrows can use XK_LEFT, XK_RIGHT, XK_UP, XK_DOWN if needed for smthing

int	key_press_2(int keycode, t_trace *trace)
{
	
	//translation
	if (keycode == J)
		translate_object(trace, trace->on, vec(-.5, 0, 0));
	else if (keycode == L)
		translate_object(trace, trace->on, vec(.5, 0, 0));
	else if (keycode == I)
		translate_object(trace, trace->on, vec(0, .5, 0));
	else if (keycode == K)
		translate_object(trace, trace->on, vec(0, -.5, 0));
	else if (keycode == U)
		translate_object(trace, trace->on, vec(0, 0, .5));
	else if (keycode == O)
		translate_object(trace, trace->on, vec(0, 0, -.5));
	else
		supersample_handle(keycode, trace);
	render(trace);
	return (0);
}

int	key_press(int keycode, t_trace *trace)//first function is for things that do not rerender the scene. key_press_2 and on will rerender.
{
	//printf("key: %d\n", keycode);//prints key press num for troub shoot
	if (keycode == XK_Escape)
		close_win(trace);
	else if (keycode == N_1 || keycode == N_2 || keycode == N_3 ||  keycode == N_9 || keycode == N_0)
		switch_list(keycode, trace, trace->on);
	else if (keycode == PAD_PLUS)
		next_list_ob(trace, trace->on);
	else if (keycode == PAD_MINUS)
		prev_list_ob(trace, trace->on);
	else if (keycode == UP_CARET)
		trace->layer = !trace->layer;
	else if ((keycode == F1) | (keycode == F3))//F1 = forge and  F3 = save png. bones
	{
		char *name;// easily make name select and function forge or save png

		if (keycode == F3)
			name = get_nxt_name("scene_");
		else if (keycode == F1)
			name = get_nxt_name_rt("forged_");
		if (!name)
			clear_all(trace);
		
		if (keycode == F1)
			forge_rt(name, trace);// bones only, uses snprintf
		else if (keycode == F3)//bones, uses png functions
		{
			if (export_png(name, &trace->img, trace->width_orig, trace->height_orig, NULL) == -1)
			{
				free(name);
				close_win(trace);
			}
			ft_putstr_color_fd(1, "EXPORT COMPLETE\n", BOLD_BRIGHT_BLUE);
		}
		free(name);
	}
	else
		key_press_2(keycode, trace);
	return (0);
}
