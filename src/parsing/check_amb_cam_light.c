#include "minirt.h"

void	check_amb(char **line, char ***rt_file)
{
	char	*ratio_str;	

	ratio_str = line[1];
	if (check_param_num(line, 3))
		free_exit(rt_file, "Error\n Invalid ambient lighting parameters\n", \
	" Ambient lighting must be in the format <type id> <light_ratio> <rgb>\n");
	if (check_double(&ratio_str, 0.0, 1.0))
		free_exit(rt_file, "Error\n Invalid ambient lighting ratio\n", \
	" Ambient Lighting ratio must be between 0.0 and 1.0\n");
	if (check_color(line[2]))
		free_exit(rt_file, "Error\n Invalid ambient color value\n", \
	" Ambient color channel values must be between 0 and 255" \
	" in the format r,g,b\n");
}

void	check_cam(char **line, char ***rt_file)
{
	if (check_param_num(line, 4))
		free_exit(rt_file, "Error\n Invalid camera parameters\n", \
	" Camera must be in the format <type id> <x,y,z> <x,y,z> <fov>\n");
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid camera coordinates\n", \
	" Camera coordinates must be in the format x,y,z\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid camera orientation\n", \
	" Camera orientation must be in the format x,y,z," \
	" with values between zero and one\n");
	if (check_fov(line[3]))
		free_exit(rt_file, "Error\n Invalid camera FOV\n", \
	" Camera FOV must be between 0 and 180\n");
}

void	check_light(char **line, char ***rt_file)
{
	char	*bright_ratio;

	bright_ratio = line[2];
	if (ft_matrix_len(line) != 4 && ft_matrix_len(line) != 3)
		free_exit(rt_file, "Error\n Invalid light parameters\n", \
	" Light must be in the format <type id> <x,y,z> <brightness> <rgb>\n"
			" (<rgb> is optional)\n");
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid light coordinates\n", \
	" Light coordinates must be in the format x,y,z\n");
	if (check_double(&bright_ratio, 0.0, 1.0))
		free_exit(rt_file, "Error\n Invalid light brightness\n", \
	" Light brightness must be between 0.0 and 1.0\n");
	if (line[3])
	{
		if (check_color(line[3]))
			free_exit(rt_file, "Error\n Invalid light color value\n", \
		" Light color channel values must be between 0 and 255" \
		" in the format r,g,b\n");
	}
}

int	check_int(char *int_str, int min, int max)
{
	int		i;
	char	str[30];
	int		num;	

	i = -1;
	while (*int_str != '\0')
	{
		if (!isdigit(*int_str))
			return (1);
		str[++i] = *int_str;
		int_str++;
	}
	str[++i] = '\0';
	num = ft_atoi(str);
	if (num < min || num > max)
		return (1);
	if (*(int_str) != '\0')
		return (1);
	return (0);
}
