/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:30:30 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 14:30:32 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	print_obj_nums(t_obj_counts *counts)
{
	printf("amb_count: %d\n", counts->amb_count);
	printf("cam_count: %d\n", counts->cam_count);
	printf("light_count: %d\n", counts->light_count);
	printf("sphere_count: %d\n", counts->sphere_count);
	printf("plane_count: %d\n", counts->plane_count);
	printf("cyl_count: %d\n", counts->cyl_count);
}

void	print_3d_array(char ***array)
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

long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_times(long start, long end, char *msg)
{
	double	duration;

	duration = (double)(end - start) / 1000;
	printf(msg, duration);
}
