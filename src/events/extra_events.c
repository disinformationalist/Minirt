#include "minirt.h"

//forge file or export png image

void	forge_or_export(int keycode, t_trace *trace)
{
	char	*name;

	name = NULL;
	if (keycode == F3)
		name = get_nxt_name("scene_");
	else if (keycode == F1)
		name = get_nxt_name_rt("forged_");
	if (!name)
		clear_all(trace);
	if (keycode == F1)
		forge_rt(name, trace);// bones only, uses snprintf
	else if (keycode == F3)//bones, uses png functions
	{
		if (export_png(name, &trace->img, trace->width_orig, trace->height_orig, NULL) == -1)
		{
			free(name);
			close_win(trace);
		}
		ft_putstr_color_fd(1, "EXPORT COMPLETE\n", BOLD_BRIGHT_BLUE);
	}
	free(name);
}

//super on/off

int	supersample_handle(int keycode, t_trace *trace)
{
	if (keycode == SPACE)
	{
		if (trace->supersample)
		{
			trace->width = trace->width_orig;
			trace->height = trace->height_orig;
			init_viewing(trace);
		}
		else
		{
			trace->width = trace->width * trace->s_kernel;
			trace->height = trace->height * trace->s_kernel;
			init_viewing(trace);
		}
		trace->supersample = !trace->supersample;
	}
	return (0);
}