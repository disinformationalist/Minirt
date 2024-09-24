#include "minirt.h"
/****KEY HOOK GUIDE****/
/*

1 => 4, 9, 0: switch between object lists (spheres, planes, etc...)

1 = sp list;
2 = pl list;
3 = cy list;
4 = 4th object... todo (prisms?)

9 = lt list;
0 = cam;

F1 => forge rt file from current scene
F3 => save current scene to png

F1	 F3								  NUM PAD	
+-------------------------------+	+---------+
|1 2 3 4(lists)        (l,c)9 0	|	|		- | pad - prev obj on curr list
|Q W E					U I O	|	|		+ | pad + next obj on curr list
|A S D					J K L	|	|		  |
|(rot)					(move)	|	|		  |
|			  SPACE		,	.	|	|		  |
+-------------------------------+	+---------+

SPACE => supersample mode

, => pop current object from current list // second layer scale down axes

. => push new object to current list // second layer scale up all axes

//TRANSLATION(first layer) //SCALE(second layer)  

J,L => x dirs
I,K => y dirs
U,O => z dirs

//ROTATION

J,L => x dirs
I,K => y dirs
U,O => z dirs

-----------------TODO--------------------

//OTHER TRANSFORMATIONS?


//maybe an onscreen control board?
*/


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

int key_press_3(int keycode, t_trace *trace)
{
	if (keycode == A)
		rotate_object(trace, trace->on, rot_x(-M_PI / 6));
	else if (keycode == D)
		rotate_object(trace, trace->on, rot_x(M_PI / 6));
	else if (keycode == W)
		rotate_object(trace, trace->on, rot_y(M_PI / 6));
	else if (keycode == S)
		rotate_object(trace, trace->on, rot_y(-M_PI / 6));
	else if (keycode == Q)
		rotate_object(trace, trace->on, rot_z(M_PI / 6));
	else if (keycode == E)
		rotate_object(trace, trace->on, rot_z(-M_PI / 6));
	else
		supersample_handle(keycode, trace);
	return (0);
}

//arrows can use XK_LEFT, XK_RIGHT, XK_UP, XK_DOWN if needed for smthing

//second layer of key press 2 controls, scale

int	key_press_2layer(int keycode, t_trace *trace)
{
	if (keycode == J)
		scale_object(trace, trace->on, vec(.9, 1, 1, 0));
	else if (keycode == L)
		scale_object(trace, trace->on, vec(1.1, 1, 1, 0));
	else if (keycode == I)
		scale_object(trace, trace->on, vec(1, 1.1, 1, 0));
	else if (keycode == K)
		scale_object(trace, trace->on, vec(1, .9, 1, 0));
	else if (keycode == U)
		scale_object(trace, trace->on, vec(1, 1, .9, 0));
	else if (keycode == O)
		scale_object(trace, trace->on, vec(1, 1, 1.1, 0));
	else if (keycode == PERIOD)
		scale_object(trace, trace->on, vec(1.1, 1.1, 1.1, 0));
	else if (keycode == COMMA)
		scale_object(trace, trace->on, vec(.9, .9, .9, 0));
	else
		key_press_3(keycode, trace);
	render(trace);
	return (0);
}

//translation, push, and pop functions

int	key_press_2(int keycode, t_trace *trace)
{
	if (keycode == J)
		translate_object(trace, trace->on, vec(-.5, 0, 0, 0));
	else if (keycode == L)
		translate_object(trace, trace->on, vec(.5, 0, 0, 0));
	else if (keycode == I)
		translate_object(trace, trace->on, vec(0, .5, 0, 0));
	else if (keycode == K)
		translate_object(trace, trace->on, vec(0, -.5, 0, 0));
	else if (keycode == U)
		translate_object(trace, trace->on, vec(0, 0, .5, 0));
	else if (keycode == O)
		translate_object(trace, trace->on, vec(0, 0, -.5, 0));
	else if (keycode == PERIOD)
		push_new_object(trace, trace->on);
	else if (keycode == COMMA)
		pop_object(trace, trace->on);
	else
		key_press_3(keycode, trace);
	render(trace);
	return (0);
}

int	key_press(int keycode, t_trace *trace)
{
	printf("key: %d\n", keycode);
	if (keycode == XK_Escape)
		close_win(trace);
	else if (keycode == UP_CARET)
		trace->layer = !trace->layer;
	else if (keycode == N_1 || keycode == N_2 || keycode == N_3 ||  keycode == N_9 || keycode == N_0)
		switch_list(keycode, trace, trace->on);
	else if (keycode == PAD_PLUS)
		next_list_ob(trace, trace->on);
	else if (keycode == PAD_MINUS)
		prev_list_ob(trace, trace->on);
	else if ((keycode == F1) | (keycode == F3))//F1 = forge and  F3 = save png. bones
		forge_or_export(keycode, trace);
	else if (trace->layer)
		key_press_2layer(keycode, trace);
	else
		key_press_2(keycode, trace);
	return (0);
}
