#include "minirt.h"

void	render(t_trace *trace)
{
	//set all transforms here maybe move to outside render later... must adjust hooks
	/* set_sp_transforms(trace);
	set_pl_transforms(trace);
	set_cy_transforms(trace); */
	/* t_vec3 cam_cen;
	t_vec3 cam_ori;
	cam_cen = trace->cam->center;
	cam_ori = trace->cam->orient;
	trace->cam->transform = view_transform(cam_cen, cam_ori, vec(0, 1, 0, 0));
 	//trace->cam->transform = view_transform(vec(0, 0, -6, 1), vec(0, 0, 1, 1), vec(0, 1, 0, 0)); 
	print_matrix_4(&trace->cam->transform);
 */
	//print_vec((trace->cam->orient));

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
