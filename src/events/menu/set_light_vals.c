#include "minirt.h"

void	set_lttype(void *con, void *win, t_light *lt)
{
	unsigned int	color;

	color = 0x90C4FF;	
	if (lt == NULL)
		mlx_string_put(con, win, 184, 58, color, "");
	else if (lt->type == POINT)
		mlx_string_put(con, win, 186, 58, color, "POINT");
	else if (lt->type == SPOT)
		mlx_string_put(con, win, 188, 58, color, "SPOT");
	else if (lt->type == AREA)
		mlx_string_put(con, win, 188, 58, color, "AREA");
	else
		mlx_string_put(con, win, 184, 58, color, "");
	mlx_string_put(con, win, 76, 58, color, "TO CUBE");
	mlx_string_put(con, win, 274, 58, color, "TO CAMERA");
}

void	set_vals(void *con, void *win, t_light *lt)
{
	double			bright;
	char			val[10];
	int				ys;
	int				xs;
	unsigned int	color;

	xs = 324;
	ys = 197;
	color = 0x90C4FF;
	set_lttype(con, win, lt);
	if (lt == NULL)
		return ;
	bright = lt->brightness;
	sprintf(val, "%.2f", bright);
	mlx_string_put(con, win, xs, ys, color, val);
	if (lt->type == SPOT)
	{
		sprintf(val, "%.0f", lt->in_angle);
		mlx_string_put(con, win, xs + 7, ys + 115, color, val);
		sprintf(val, "%.0f", lt->out_angle);
		mlx_string_put(con, win, xs + 7, ys + 140, color, val);
	}
	else if (lt->type == AREA)
	{
		double wid = 2 / (lt->emitter->curr_scale.mat[0]);
		double len = 2 / (lt->emitter->curr_scale.mat[10]);
		ys += 31;
		sprintf(val, "%.2f", wid);
		mlx_string_put(con, win, xs, ys + 165, color, val);
		sprintf(val, "%.2f", len);
		mlx_string_put(con, win, xs, ys + 190, color, val);
		sprintf(val, "%d", lt->usteps);
		mlx_string_put(con, win, xs + 7, ys + 215, color, val);
		sprintf(val, "%d", lt->vsteps);
		mlx_string_put(con, win, xs + 7, ys + 240, color, val);
	}
}

//sets the current vals on the light control board

void	set_light_vals(t_trace *trace, t_on *on)
{
	void			*con;
	void			*win;

	con = trace->mlx_connect;
	win = trace->mlx_win;	
	set_vals(con, win, trace->curr_lt);
	set_color(con, win, on);
}