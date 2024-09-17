#include "minirt.h"

//set all transforms to account for position and orientation.

void	set_sp_transforms(t_trace *trace)
{
	t_sphere		*curr_sp;
	t_matrix_4x4	inv_scale;
	t_matrix_4x4	inv_tran;

	if (trace->spheres)
	{
		curr_sp = trace->spheres;
		while (true)
		{
			inv_scale = inv_scaling(curr_sp->radius, curr_sp->radius, curr_sp->radius);
			inv_tran = translation(-curr_sp->center.x, -curr_sp->center.y, -curr_sp->center.z);
			curr_sp->transform = (mat_mult(inv_scale, inv_tran));
			curr_sp = curr_sp->next;
			if (curr_sp == trace->spheres)
				break;
		}
	}
}

void	set_pl_transforms(t_trace *trace)
{
	t_plane			*curr_pl;
	t_matrix_4x4	inv_trans;

	if (trace->planes)
	{
		curr_pl = trace->planes;
		while (true)
		{
			inv_trans = translation(-curr_pl->point.x, -curr_pl->point.y, -curr_pl->point.z);
			curr_pl->transform = mat_mult(rot_up(curr_pl->norm), inv_trans);	
			curr_pl = curr_pl->next;
			if (curr_pl == trace->planes)
				break;
		}
	}
}

void	set_cy_transforms(t_trace *trace)
{
	t_cylinder		*curr_cy;
	t_matrix_4x4	inv_trans;
	t_matrix_4x4	scale;

	if (trace->cylinders)
	{
		curr_cy = trace->cylinders;
		while (true)
		{
			inv_trans = translation(-curr_cy->center.x, -curr_cy->center.y, -curr_cy->center.z);
			scale = inv_scaling(curr_cy->radius, curr_cy->radius, curr_cy->radius);
			curr_cy->transform = mat_mult(rot_up(curr_cy->norm), mat_mult(scale, inv_trans));
			curr_cy = curr_cy->next;
			if (curr_cy == trace->cylinders)
				break;
		}
	}
}

void	render(t_trace *trace)
{
	//set all transforms here maybe move to outside render later... must adjust hooks
	set_sp_transforms(trace);
	set_pl_transforms(trace);
	set_cy_transforms(trace);

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
	
	long start = get_time();//test
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
	print_times(start, get_time(), "RENDER TIME: %f seconds\n");//test
}
