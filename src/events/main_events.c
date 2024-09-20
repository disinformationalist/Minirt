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

, => pop current object from current list

. => push new object to current list

//TRANSLATION

J,L => x dirs
I,K => y dirs
U,O => z dirs

-----------------TODO--------------------

//ROTATION

J,L => x dirs
I,K => y dirs
U,O => z dirs

//RESIZE OBJECT, + -

//OTHER TRANSFORMATIONS?


//maybe an onscreen control board?
*/

//moves current "on" object in x,y,z

void	translate_object(t_trace *trace, t_on *on, t_vec3 vec)
{
	
	if (on->object == NULL)
		return ;
	if (on->type == SPHERE)
		trace->curr_sp->center = add_vec(trace->curr_sp->center, vec);
	else if (on->type == PLANE)
		trace->curr_pl->point = add_vec(trace->curr_pl->point, vec);
	else if (on->type == CYLINDER)
		trace->curr_cy->center = add_vec(trace->curr_cy->center, vec);
	else if (on->type == LIGHT)
		trace->lights->center = add_vec(trace->lights->center, vec);
	else if (on->type == CAM)
	{
		trace->cam->center = add_vec(trace->cam->center, vec);
		init_viewing(trace);
	}
}

void	rotate_object(t_trace *trace, t_on *on, t_matrix_4x4 rot)
{
	
	if (on->object == NULL)
		return ;
	else if (on->type == PLANE)
		trace->curr_pl->norm = mat_vec_mult(rot, trace->curr_pl->norm);
	else if (on->type == CYLINDER)
		trace->curr_cy->norm = mat_vec_mult(rot, trace->curr_cy->norm);
	else if (on->type == CAM)
	{
		//broken does scaling
		trace->cam->orient = norm_vec(mat_vec_mult(rot, norm_vec(trace->cam->orient)));
		print_vec(trace->cam->orient);
		init_viewing(trace);
	}
	else
		return ;
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

//rotations will go in key_press_3 TODO

int key_press_3(int keycode, t_trace *trace)
{
	if (keycode == A)//along x
		rotate_object(trace, trace->on, rot_x(-M_PI / 4));
	else if (keycode == D)
		rotate_object(trace, trace->on, rot_x(M_PI / 4));
	else if (keycode == W)//along y
		rotate_object(trace, trace->on, rot_y(M_PI / 4));
	else if (keycode == S)
		rotate_object(trace, trace->on, rot_y(-M_PI / 4));
	else if (keycode == Q)//along z
		rotate_object(trace, trace->on, rot_z(M_PI / 4));
	else if (keycode == E)
		rotate_object(trace, trace->on, rot_z(-M_PI / 4));
	else
		supersample_handle(keycode, trace);
	return (0);
}

//arrows can use XK_LEFT, XK_RIGHT, XK_UP, XK_DOWN if needed for smthing

void	push_new_object(t_trace *trace, t_on *on)
{
	if (on->type == SPHERE)
	{
		if (insert_spcopy_after(trace, &trace->curr_sp))
			close_win(trace);
	}
	else if (on->type == PLANE)
	{
		if (insert_plcopy_after(trace, &trace->curr_pl))
			close_win(trace);	
	}
	else if (on->type == CYLINDER)
	{
		if (insert_cycopy_after(trace, &trace->curr_cy))
			close_win(trace);	
	}
	else
		return ;
	/* else if (on->type == LIGHT)////---------------
	{
		if (insert_ltcopy_after(trace, &trace->lights))
			close_win(trace);	
	} */
	next_list_ob(trace, trace->on);
}

void	pop_object(t_trace *trace, t_on *on)
{
	if (on->type == SPHERE)
		pop_sp(trace, &trace->curr_sp);
	else if (on->type == PLANE)
		pop_pl(trace, &trace->curr_pl);
	else if (on->type == CYLINDER)
		pop_cy(trace, &trace->curr_cy);
	else
		return ;
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
		//supersample_handle(keycode, trace);
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
		forge_or_export(keycode, trace);
	else
		key_press_2(keycode, trace);
	return (0);
}
