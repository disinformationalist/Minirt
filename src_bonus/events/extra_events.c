#include "minirt.h"

//forge new rt file or export png image for current scene

void	forge_or_export(int keycode, t_trace *trace)
{
	char	*name;

	name = NULL;
	if (keycode == F3)
		name = get_nxt_name("scene_");
	else if (keycode == F1)
		name = get_nxt_name_rt("rt_files/forged_");
	if (!name)
		clear_all(trace);
	if (keycode == F1)
	{
		forge_rt(name, trace);
		ft_putstr_color_fd(1, "RT FILE READY\n", BOLD_BRIGHT_BLUE);
	}
	else if (keycode == F3)
	{
		if (export_png(name, &trace->img, trace->width, trace->height) == -1)
		{
			free(name);
			close_win(trace);
		}
		ft_putstr_color_fd(1, "EXPORT COMPLETE\n", BOLD_BRIGHT_BLUE);
	}
	free(name);
}
