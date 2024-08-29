#include "minirt.h"

int	ft_3darray_len(char ***array)//ft_matrix len?
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

void	print_3d_array(char ***array)//testing
{
	int	i;
	int	j;

	i = 0;
	while (array[i])
	{
		j = 0;
		while (array[i][j])
		{
			printf("%s\n", array[i][j]);
			j++;
		}
		i++;
	}
}

int	main(int ac, char **av)
{
	char ***rt_file;
	t_trace	trace;
	if (ac != 2)
	{
		printf("Error: Usage: ./minirt [scene.rt]\n");
		return (1);
	}
	rt_file = split_file(av[1]);
	if (!rt_file)
		return (1);
	parse_rt(&trace, rt_file);//PARSING
	free_3d_array_i(rt_file, ft_3darray_len(rt_file));//free parsing tool 3d split rt_file
    
//----------------------------- multithread stuff here
	trace.num_cols = 1;//compare speeds with optisetter
	trace.num_rows = get_num_cores();
	//trace.num_rows = 1;

	//printf("num rows: %d\n", trace.num_rows);
	trace.threads = (pthread_t *)malloc(trace.num_rows * trace.num_cols * sizeof(pthread_t));
	if (!trace.threads)
	{
		printf("Error: Thread Malloc failed\n");
		return (EXIT_FAILURE);
	}
//-----------------------	

	trace.name = "***MiniRT***";
	trace_init(&trace);
	render(&trace);
	mlx_loop(trace.mlx_connect);
	return (0);
}
	//print_3d_array(rt_file);

