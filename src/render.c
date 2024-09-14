#include "minirt.h"

//set all transforms to account for position and orientation.

void	set_sp_transforms(t_trace *trace)
{
	t_sphere	*curr_sp;

	if (trace->spheres)
	{
		curr_sp = trace->spheres;
		while (true)
		{
		/* 	t_matrix_4x4 inv_scale = inv_scaling(curr_sp->radius, curr_sp->radius, curr_sp->radius);
			t_matrix_4x4 inv_tran = translation(-curr_sp->center.x, -curr_sp->center.y, -curr_sp->center.z);// building the transform working
			curr_sp->transform = (mat_mult(inv_scale, inv_tran)); */
 
			t_matrix_4x4 tran = translation(curr_sp->center.x, curr_sp->center.y, curr_sp->center.z);// building the forward transform and inverse, also works
			t_matrix_4x4 scale = scaling(curr_sp->radius, curr_sp->radius, curr_sp->radius);
			curr_sp->transform = inverse(mat_mult(tran, scale));
			curr_sp = curr_sp->next;
			if (curr_sp == trace->spheres)
				break;
		}
	}
}
// use orientation vec to get the rotations from assumed normal pointing upward.
//building method for getting the right rotation here

bool	veccmp(t_vec3 v1, t_vec3 v2)
{
	if (v1.x != v2.x)
		return (true);
	if (v1.y != v2.y)
		return (true);
	if (v1.z != v2.z)
		return (true);
	return (false);
}
//will need inverse(transform), transpose(inverse(transform)) just assign inverse, maybe both.


/* t_matrix_4x4 get_rot(t_vec3 ori)// must use this + normal, then invert... use rodrigues see chat
{
	t_matrix_4x4	res;
	double			yaw;
	double			pitch;
	t_matrix_4x4	y_rot;
	t_matrix_4x4	x_rot;

	if (!veccmp(ori, vec(0, 1, 0, 0)))
		return (init_identity_matrix(&res), res);
	if (!veccmp(ori, vec(0, -1, 0, 0)))
		return (rot_x(M_PI));//simpler way?
	yaw = atan2(ori.x, ori.z);
	pitch = asin(ori.y);
	y_rot = rot_y(-yaw);
	x_rot = rot_x(-pitch);
	return (mat_mult(x_rot, y_rot));
} */
/* 
void	set_pl_transforms(t_trace *trace)
{
	t_plane	*curr_pl;

	if (trace->planes)
	{
		curr_pl = trace->planes;
		while (true)
		{
			
			//combine these then inverse for the transform.
			t_matrix_4x4 trans = translation(-curr_pl->point.x, -curr_pl->point.y, -curr_pl->point.z);// building the transform working

			curr_pl->transform = mat_mult(trans, get_rot(curr_pl->norm));

			curr_pl = curr_pl->next;
			if (curr_pl == trace->planes)
				break;
		}
	}
} */

/* void	set_cy_transforms(t_trace *trace)
{
	t_plane	*curr_cy;

	if (trace->cylinders)
	{
		curr_cy = trace->cylinders;
		while (true)
		{
			t_matrix_4x4 trans = translation(-curr_cy->center.x, -curr_cy->center.y, -curr_cy->point.z);// building the transform working
			//t_matrix_4x4 sca = inv_scaling(curr_pl->radius, curr_pl->radius, curr_pl->radius);
			curr_pl->transform = mat_mult(sca, trans);
			curr_pl = curr_sp->next;
			if (curr_pl == trace->spheres)
				break;
		}
	}
} */

void	render(t_trace *trace)
{
	//set all transforms here
	set_sp_transforms(trace);
	//set_pl_transforms(trace);
	//set_cy_transforms(trace);

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
