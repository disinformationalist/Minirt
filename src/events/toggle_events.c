#include "minirt.h"

int	close_win(t_trace *trace)
{
	free_all_objects(trace);
	free(trace->on);
	free(trace->w_colors);
	free(trace->m_colors);
	mlx_destroy_image(trace->mlx_connect, trace->img.img_ptr);
	mlx_destroy_window(trace->mlx_connect, trace->mlx_win);
	mlx_destroy_display(trace->mlx_connect);
	free(trace->mlx_connect);
	free(trace->threads);
	exit(EXIT_SUCCESS);
	return (0);
}

//radial sine normal perturbation on/off

void	toggle_sine(t_trace *trace)
{
	if (trace->on->type == PLANE)
		trace->curr_pl->sine = !trace->curr_pl->sine;
	else if (trace->on->type == CUBE)
		trace->curr_cu->sine = !trace->curr_cu->sine;
}

//random normal perturb "frost" on/off

void	frost_on(t_trace *trace, t_on on)
{
	if (on.object == NULL)
		return ;
	if (on.type == PLANE)
		trace->curr_pl->w_frost = !trace->curr_pl->w_frost;
	else if (on.type == SPHERE)
		trace->curr_sp->w_frost = !trace->curr_sp->w_frost;
	else if (on.type == CYLINDER)
		trace->curr_cy->w_frost = !trace->curr_cy->w_frost;
	else if (on.type == CUBE)
		trace->curr_cu->w_frost = !trace->curr_cu->w_frost;
	else if (on.type == HYPERBOLOID)
		trace->curr_hy->w_frost = !trace->curr_hy->w_frost;
	else
		return ;
}

//shadows cast by on object on/off

void	toggle_shadow(t_trace *trace, t_on *on)
{
	if (on->object == NULL)
		return ;
	if (on->type == SPHERE)
		trace->curr_sp->shadow = !trace->curr_sp->shadow;
	else if (on->type == PLANE)
		trace->curr_pl->shadow = !trace->curr_pl->shadow;
	else if (on->type == CYLINDER)
		trace->curr_cy->shadow = !trace->curr_cy->shadow;
	else if (on->type == HYPERBOLOID)
		trace->curr_hy->shadow = !trace->curr_hy->shadow;
	else if (on->type == CUBE)
		trace->curr_cu->shadow = !trace->curr_cu->shadow;
	else
		return ;
}

//bump mapping on/off

void	toggle_bump(t_trace *trace, t_on *on)
{
	if (on->object == NULL)
		return ;
	if (on->type == SPHERE)
		trace->curr_sp->bump = !trace->curr_sp->bump;
	else if (on->type == PLANE)
		trace->curr_pl->bump = !trace->curr_pl->bump;
	else if (on->type == CYLINDER)
		trace->curr_cy->bump = !trace->curr_cy->bump;
	else if (on->type == CUBE)
		trace->curr_cu->bump = !trace->curr_cu->bump;
	else if (on->type == HYPERBOLOID)
		trace->curr_hy->bump = !trace->curr_hy->bump;
	else
		return ;
}

void	toggle_caps(t_trace *trace)
{
	t_type type;

	type = trace->on->type;
	if (trace->on->object == NULL)
		return ;
	else if (type == CYLINDER)
		trace->curr_cy->caps = !trace->curr_cy->caps;
	else if (type == HYPERBOLOID)
		trace->curr_hy->caps = !trace->curr_hy->caps;
}

void	toggle_lowres(t_trace *trace)
{
	trace->low_res = !trace->low_res;
	//if (!trace->on_menu)
		trace->low_flag = !trace->low_flag;
	reset_con(trace);
}
