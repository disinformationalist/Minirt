#include "minirt.h"

/* t_color	apply_amb(t_amb *amb, t_color obj_color)
{
	t_color res;
	int 	color;

	color = (int)obj_color.r + (int)amb->amb_color.r;
	if (color > 255)//clamps to prevent overflow...
		color = 255;
	res.r = color;	
	color = (int)obj_color.g + (int)amb->amb_color.g;
	if (color > 255)
		color = 255;
	res.g = color;
	color = (int)obj_color.b + (int)amb->amb_color.b;
	if (color > 255)
		color = 255;
	res.b = color;
	return (res);
} */

void	apply_ambient_light(t_trace *trace)
{
	t_sphere	*curr_sp;
	t_plane		*curr_pl;
	//t_cylinder	*curr_cy;

	if (trace->spheres)
	{
		curr_sp = trace->spheres;
		while (true)
		{
			curr_sp->color = apply_amb(trace->amb, curr_sp->color);
			curr_sp = curr_sp->next;
			if (curr_sp == trace->spheres)
				break ;
		}
	}
	if (trace->planes)
	{
		curr_pl = trace->planes;
		while (true)
		{
			curr_pl->color = apply_amb(trace->amb, curr_pl->color);
			curr_pl = curr_pl->next;
			if (curr_pl == trace->planes)
				break ;
		}
	}
}

void	render(t_trace *trace)
{
	//handle adjust for ambient light here. works here, but not with supersample on. will use for no bonus version.
	/* if (trace->amb)
		apply_ambient_light(trace); */
	if (trace->supersample)
	{
		ft_putstr_color_fd(1, "SUPERSAMPLE IN PROGRESS...\n", BOLD_GREEN);
		trace->pixels_xl = malloc_ui_matrix(trace->width_orig * trace->s_kernel, trace->height_orig * trace->s_kernel);
		if (!trace->pixels_xl)
		{
			printf(RED"The super malloc has failed\n"RESET);
			close_win(trace);
		}
		zero_ui_matrix(trace->pixels_xl, trace->width, trace->height);
	}
	render_scene(trace);
	if (trace->supersample)
	{
		downsample_xl(trace->width, trace->height, &trace->img, trace->pixels_xl, trace->s_kernel);
		zero_ui_matrix(trace->pixels_xl, trace->width, trace->height);
		free_ui_matrix(trace->pixels_xl, trace->height);
		ft_putstr_color_fd(1, "SUPERSAMPLE COMPLETE!\n", BOLD_BRIGHT_GREEN);
	}
	mlx_put_image_to_window(trace->mlx_connect,
		trace->mlx_win, trace->img.img_ptr, 0, 0);
}
