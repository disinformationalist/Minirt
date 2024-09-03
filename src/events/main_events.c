#include "minirt.h"
/****KEY HOOK GUIDE****/

/*
F1 => forge rt file from current scene
F3 => save current scene to png


//list traversal

1 => 3: switch between object lists (spheres, planes, etc...)

1 = sp list;
2 = pl list;
3 = cy list;

num pad + => next object on current list
num pad - => prev object on current list


*/


//dummy test, later pass vector translation in place of double move. apply vector move op

void	translate_object(t_on *on, double move)
{
	t_sphere	*sphere;
	t_plane		*plane;

	if (on->type == SPHERE)
	{
		//later simply apply translation vector with translation function
		sphere = (t_sphere *)on->object;
		sphere->center.x += move;
		printf("%f\n", sphere->center.x);

	}
	else if (on->type == PLANE)
	{
		plane = (t_plane *)on->object;
		plane->point.x += move;
		printf("%f\n", plane->point.x);
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

int	key_press_2(int keycode, t_trace *trace)
{
	
	//testing dummy move obj
	if (keycode == XK_Left)//need move z, change these to uiojkl
	{
		//will pass in apropriate translation function later
		translate_object(trace->on, -.5);
	}
	else if (keycode == XK_Right)
		translate_object(trace->on, .5);
	/* else if (keycode == XK_Up)
		trace->move_y -= .5;
	else if (keycode == XK_Down)
		trace->move_y += .5; */
	else
		supersample_handle(keycode, trace);
	render(trace);
	return (0);
}

int	key_press(int keycode, t_trace *trace)//first function is for things that do not rerender the scene. key_press_2 and on will rerender.
{
	printf("key: %d\n", keycode);
	if (keycode == XK_Escape)
		close_win(trace);
	else if (keycode == N_1 || keycode == N_2 || keycode == N_3)
		switch_list(keycode, trace, trace->on);
	else if (keycode == PAD_PLUS)
		next_list_ob(trace, trace->on);
	else if (keycode == PAD_MINUS)
		prev_list_ob(trace, trace->on);
	else if (keycode == UP_CARET)
		trace->layer = !trace->layer;
	else if ((keycode == F1) | (keycode == F3))//forge and save png. bones
	{
		char *name;// easily make name select function for norm

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
