#include "minirt.h"

void	render(t_trace *trace)
{
	t_track_hits	*closest;	

	closest = (t_track_hits *)malloc(sizeof(t_track_hits));
	if(!closest)
	{
		perror("Malloc error\n");
		clear_all(trace);
	}
	long start = get_time();//test
	if (trace->supersample)
	{
		ft_putstr_color_fd(1, "SUPERSAMPLE IN PROGRESS...\n", BOLD_GREEN);
		compute_pixels_s(trace, closest);
		ft_putstr_color_fd(1, "SUPERSAMPLE COMPLETE!\n", BOLD_BRIGHT_GREEN);
	}
	else
		compute_pixels(trace, closest);
	free(closest);
	mlx_put_image_to_window(trace->mlx_connect,
		trace->mlx_win, trace->img.img_ptr, 0, 0);
	print_times(start, get_time(), "RENDER TIME: %f seconds\n");//test
}
