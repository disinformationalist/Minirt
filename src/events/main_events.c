#include "minirt.h"
/****KEY HOOK GUIDE****/
/*
1 => 5, 9, 0: switch between object lists (spheres, planes, etc...)

1 = sp list;
2 = pl list;
3 = cy list;
4 = hy list;
5 = cu list;

9 = lt list;
0 = cam;

F1 => forge rt file from current scene
F3 => save current scene to png

Tab => second layer key press

F1	 F3								  NUM PAD	
+-------------------------------+	+---------+
|1 2 3 4(lists)        (l,c)9 0	|	|		- | pad - prev obj on curr list
|Q W E					U I O	|	|		+ | pad + next obj on curr list
|A S D					J K L	|	|		  | //second key layer
|(rot)					(move)	|	|		  | pad - supersample level -1
|			  SPACE		,	.	|	|		  | pad + supersample level +1
+-------------------------------+	+---------+
SPACE => supersample mode

, => pop current object from current list // second layer scale down all \
	axes, for reduce bright
. => push new object to current list // second layer scale up all axes, \
	for light increase bright

//TRANSLATION(first layer) //SCALE(second layer)  
J,L => x dirs
I,K => y dirs
U,O => z dirs

//ROTATION
J,L => x dirs
I,K => y dirs
U,O => z dirs

//shadow toggle strg(right) key
//bump toggle strg(left) key

//COLOR SHIFT
mouse wheel shifts color of on object or light.

NUM PAD 1 - 9 => change material of current object */

//change material of current object

int	transfigure(int keycode, t_trace *trace)
{
	if (keycode == PAD_1)
		change_mat(trace, trace->on, get_mat(DEFAULT));
	else if (keycode == PAD_2)
		change_mat(trace, trace->on, get_mat(METAL));
	else if (keycode == PAD_3)
		change_mat(trace, trace->on, get_mat(MATTE));
	else if (keycode == PAD_4)
		change_mat(trace, trace->on, get_mat(GLASS));
	else if (keycode == PAD_5)
		change_mat(trace, trace->on, get_mat(MIRROR));
	else if (keycode == PAD_6)
		change_mat(trace, trace->on, get_mat(DIAMOND));
	else if (keycode == PAD_7)
		change_mat(trace, trace->on, get_mat(WATER));
	else if (keycode == PAD_8)
		change_mat(trace, trace->on, get_mat(CHROME));
	else if (keycode == PAD_9)
		change_mat(trace, trace->on, get_mat(AIR));
	else
		supersample_handle(keycode, trace);
	return (0);
}

int	key_press_3(int keycode, t_trace *trace)
{
	if (keycode == A)
		rotate_object(trace, trace->on, rot_x(-M_PI / 12));
	else if (keycode == D)
		rotate_object(trace, trace->on, rot_x(M_PI / 12));
	else if (keycode == W)
		rotate_object(trace, trace->on, rot_y(M_PI / 12));
	else if (keycode == S)
		rotate_object(trace, trace->on, rot_y(-M_PI / 12));
	else if (keycode == Q)
		rotate_object(trace, trace->on, rot_z(M_PI / 12));
	else if (keycode == E)
		rotate_object(trace, trace->on, rot_z(-M_PI / 12));
	else if (keycode == PAD_0)
		frost_on(trace, *(trace->on));
	else
		transfigure(keycode, trace);
	return (0);
}

//second layer of key press 2 controls, scale

int	key_press_2layer(int keycode, t_trace *trace)
{
	if (keycode == J)
		scale_object(trace, trace->on, vec(.9, 1, 1, 0), keycode);
	else if (keycode == L)
		scale_object(trace, trace->on, vec(1.1, 1, 1, 0), keycode);
	else if (keycode == I)
		scale_object(trace, trace->on, vec(1, 1.1, 1, 0), keycode);
	else if (keycode == K)
		scale_object(trace, trace->on, vec(1, .9, 1, 0), keycode);
	else if (keycode == U)
		scale_object(trace, trace->on, vec(1, 1, .9, 0), keycode);
	else if (keycode == O)
		scale_object(trace, trace->on, vec(1, 1, 1.1, 0), keycode);
	else if (trace->on->type == HYPERBOLOID && keycode == G)
		scale_hyperboloid(trace, trace->on, vec(1, 1, 1, 0), 0.9);
	else if (trace->on->type == HYPERBOLOID && keycode == H)
		scale_hyperboloid(trace, trace->on, vec(1, 1, 1, 0), 1.1);
	else if (keycode == PERIOD)
		scale_object(trace, trace->on, vec(1.1, 1.1, 1.1, 0), keycode);
	else if (keycode == COMMA)
		scale_object(trace, trace->on, vec(.9, .9, .9, 0), keycode);
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
	else if (keycode == XK_Control_R)
		toggle_shadow(trace, trace->on);
	else if (keycode == XK_Control_L)
		toggle_bump(trace, trace->on);
	else
		key_press_3(keycode, trace);
	render(trace);
	return (0);
}

//top function for keypress events

int	key_press(int keycode, t_trace *trace)
{
	if (keycode == XK_Escape)
		close_win(trace);
	else if (keycode == XK_Tab)
		trace->layer = !trace->layer;
	else if (keycode == XK_1 || keycode == XK_2 || keycode == XK_3 || \
		keycode == XK_4 || keycode == XK_9 || keycode == XK_0 || \
		keycode == XK_5)
		switch_list(keycode, trace, trace->on);
	else if (keycode == PAD_PLUS && trace->layer)
		adjust_super(keycode, trace);
	else if (keycode == PAD_MINUS && trace->layer)
		adjust_super(keycode, trace);
	else if (keycode == PAD_PLUS)
		next_list_ob(trace, trace->on);
	else if (keycode == PAD_MINUS)
		prev_list_ob(trace, trace->on);
	else if ((keycode == F1) | (keycode == F3))
		forge_or_export(keycode, trace);
	else if (trace->layer)
		key_press_2layer(keycode, trace);
	else
		key_press_2(keycode, trace);
	return (0);
}
