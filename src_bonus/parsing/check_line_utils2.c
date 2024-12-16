/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:47:04 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 14:47:05 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	check_coordinates(char *coord_str)
{
	if (check_double(&coord_str, -999.0, 999.0))
		return (1);
	if (*(coord_str) != ',')
		return (1);
	coord_str++;
	if (check_double(&coord_str, -999.0, 999.0))
		return (1);
	if (*(coord_str) != ',')
		return (1);
	coord_str++;
	if (check_double(&coord_str, -999.0, 999.0))
		return (1);
	if (*(coord_str) != '\0')
		return (1);
	return (0);
}

int	check_orientation(char *orient_str)
{
	if (check_double(&orient_str, -1.0, 1.0))
		return (1);
	if (*(orient_str) != ',')
		return (1);
	orient_str++;
	if (check_double(&orient_str, -1.0, 1.0))
		return (1);
	if (*(orient_str) != ',')
		return (1);
	orient_str++;
	if (check_double(&orient_str, -1.0, 1.0))
		return (1);
	if (*(orient_str) != '\0')
		return (1);
	return (0);
}

int	check_normalization(char *orient_str)
{
	double	coord_x;
	double	coord_y;
	double	coord_z;

	coord_x = get_double(&orient_str);
	orient_str++;
	coord_y = get_double(&orient_str);
	orient_str++;
	coord_z = get_double(&orient_str);
	if (!is_normalized(vec(coord_x, coord_y, coord_z, 0)))
		return (1);
	return (0);
}

int	check_fov(char *fov_str)
{
	int		i;
	char	str[3];	
	int		fov;	

	i = -1;
	while (*fov_str != '\0')
	{
		if (!isdigit(*fov_str))
			return (1);
		str[++i] = *fov_str;
		fov_str++;
	}
	str[++i] = '\0';
	fov = ft_atoi(str);
	if (fov < 0 || fov > 180)
		return (1);
	if (*(fov_str) != '\0')
		return (1);
	return (0);
}

int	check_param_num(char **line, int num)
{
	int	i;

	i = 0;
	while (line[i] != NULL)
		i++;
	if (i != num)
		return (1);
	return (0);
}
