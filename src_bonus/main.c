#include "minirt.h"

int	ft_3darray_len(char ***array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

int	is_rt_file_valid(char *filename)
{
	int		len;
	char	*dot;

	len = ft_strlen(filename);
	if (len < 4)
		return (0);
	dot = ft_strrchr(filename, '.');
	if (!dot || ft_strcmp(dot, ".rt"))
		return (0);
	return (1);
}

int	main(int ac, char **av)
{
	char		***rt_file;
	t_trace		trace;
	
	if (ac != 2)
	{
		free_exit(NULL, "Error\n: Usage: ./minirt [scene.rt]\n", NULL);
		return (1);
	}
	if (!is_rt_file_valid(av[1]))
	{
		free_exit(NULL, "Error\n Invalid [scene.rt] file\n", NULL);
		return (1);
	}
	rt_file = split_file(av[1]);
	if (!rt_file)
		return (1);

	parse_rt(&trace, rt_file);//PARSING
	free_3d_array_i(rt_file, ft_3darray_len(rt_file));//free parsing tool 3d split rt_file
	//free_all_objects(&trace);

    //return (0);

//----------------------------- multithread stuff here
	trace.num_cols = 1;
	trace.num_rows = get_num_cores();
	//trace.num_rows = 1;

	//printf("num rows: %d\n", trace.num_rows);
	trace.threads = (pthread_t *)malloc(trace.num_rows * trace.num_cols * sizeof(pthread_t));
	if (!trace.threads)
	{
		free_all_objects(&trace);
		printf("Error: Thread Malloc failed\n");
		return (EXIT_FAILURE);
	}

//-----------------------	

	trace_init(&trace);
	render(&trace);
	mlx_loop(trace.mlx_connect);
	return (0);
}
	//print_3d_array(rt_file);

