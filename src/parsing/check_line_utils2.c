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
