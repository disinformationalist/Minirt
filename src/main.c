#include "minirt.h"

int	ft_3darray_len(char ***array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

int	is_valid_rt(char *filename)
{
	const char	*ext;
	size_t		name_len;
	size_t		ext_len;

	ext = ".rt";
	name_len = ft_strlen(filename);
	ext_len = ft_strlen(ext);
	if (name_len < ext_len + 1)
		return (0);
	if (ft_strcmp(filename + name_len - ext_len, (char *)ext))
		return (0);
	return (1);
}

int	get_num_cores(void)
{
	int	num_cores;

	num_cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (num_cores < 1)
	{
		perror("sysconf error\n");
		return (1);
	}
	return (num_cores);
}

bool	run_trace(t_trace *trace)
{
	trace->num_cols = 1;
	trace->num_rows = get_num_cores();
	//trace->num_rows = 1;

	trace->threads = (pthread_t *)malloc(trace->num_rows \
		* trace->num_cols * sizeof(pthread_t));
	if (!trace->threads)
	{
		free_all_objects(trace);
		printf("Error\n Thread Malloc failed\n");
		return (1);
	}
	trace_init(trace);
	render(trace);
	mlx_loop(trace->mlx_connect);
	return (0);
}

int	main(int ac, char **av)
{
	char		*file;
	char		***rt_file;
	t_trace		trace;

	if (ac != 2)
		return (free_exit(NULL, \
		"Error\n Usage: ./minirt_bonus [scene.rt]\n", NULL), 1);
	if (!is_valid_rt(av[1]))
		return (free_exit(NULL, "Error\n Invalid rt file\n", NULL), 1);
	file = ft_strjoin("rt_files/", av[1]);
	if (!file)
		return (free_exit(NULL, "Error\n ft_strjoin malloc failed", NULL), 1);
	rt_file = split_file(file);
	free(file);
	if (!rt_file)
		return (1);
	parse_rt(&trace, rt_file);
	free_3d_array_i(rt_file, ft_3darray_len(rt_file));
	ft_putstr_color_fd(1, "\n  ***************** PRESS [F2] FOR KEYHOOK GUIDE *****************\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "   ******************* PRESS [F5] FOR GUI GUIDE *****************\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, " **************** PRESS [F4] TO OPEN/CLOSE THE GUI ****************\n\n", BRIGHT_BLUE);
	if (run_trace(&trace))
		return (1);
	return (0);
}
