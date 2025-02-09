#include "minirt.h"

void	check_al2(char **line, char ***rt_file, \
	char **wid_str, char **bright_ratio)
{
	if (check_double(bright_ratio, 0.0, 1.0))
		free_exit(rt_file, "Error\n Invalid light brightness\n", \
	" Light brightness must be between 0.0 and 1.0\n");
	if (check_double(wid_str, 1.0, 10.0))
		free_exit(rt_file, "Error\n Invalid area light width\n", \
	" Area light width must be between 1 and 10\n");
	if (check_color(line[6]))
		free_exit(rt_file, "Error\n Invalid light color value\n", \
	" Light color channel values must be between 0 and 255" \
	" in the format r,g,b\n");
	if (check_int(line[7], 2, 20))
		free_exit(rt_file, "Error\n Invalid area light width cells\n", \
	" Area light width cells must be between 2 and 20\n");
	if (check_int(line[8], 2, 20))
		free_exit(rt_file, "Error\n Invalid area light length cells\n", \
	" Area light length cells must be between 2 and 20\n");
}

//check area light line vals

void	check_al(char **line, char ***rt_file)
{
	char	*bright_ratio;
	int		len;
	char	*wid_str;
	char	*len_str;

	len = ft_matrix_len(line);
	bright_ratio = line[3];
	wid_str = line[4];
	len_str = line[5];
	if (len != 9)
		free_exit(rt_file, "Error\n Invalid area light parameters\n", \
			" Area Lt has the format <type id> <x,y,z> <x,y,z> <brightness>\n"
			" <w> <l> <rgb> <w_cells> <l_cells>\n");
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid area light coordinates\n", \
	" Area light coordinates must be in the format x,y,z\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid area light orientation\n", \
	" Area light orientation must be in the format x,y,z," \
	" with values between zero and one\n");
	if (check_double(&len_str, 1.0, 10.0))
		free_exit(rt_file, "Error\n Invalid area light length\n", \
	" Area light length must be between 1 and 10\n");
	check_al2(line, rt_file, &wid_str, &bright_ratio);
}

void	check_sl2(char **line, char ***rt_file, int len)
{
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid light coordinates\n", \
	" Light coordinates must be in the format x,y,z\n");
	if (len == 6)
	{
		if (check_int(line[4], 0, 90))
			free_exit(rt_file, "Error\n Invalid light inner cone angle\n", \
		" light inner cone angle must be between 0 and 180\n");
		if (check_int(line[5], 0, 90))
			free_exit(rt_file, "Error\n Invalid light outer cone angle\n", \
		" light outer cone angle must be between 0 and 180\n");
	}
	else if (len == 7)
	{
		if (check_color(line[4]))
			free_exit(rt_file, "Error\n Invalid light color value\n", \
		" Light color channel values must be between 0 and 255" \
		" in the format r,g,b\n");
		if (check_int(line[5], 0, 90))
			free_exit(rt_file, "Error\n Invalid light inner cone angle\n", \
		" light inner cone angle must be between 0 and 180\n");
		if (check_int(line[6], 0, 90))
			free_exit(rt_file, "Error\n Invalid light outer cone angle\n", \
		" light outer cone angle must be between 0 and 180\n");
	}
}

//check spot light values

void	check_sl(char **line, char ***rt_file)
{
	char	*bright_ratio;
	int		len;

	len = ft_matrix_len(line);
	bright_ratio = line[3];
	if (len != 6 && len != 7)
		free_exit(rt_file, "Error\n Invalid spot light parameters\n", \
			" Spot Lt has the format <type id> <x,y,z> <x,y,z> <brightness>\n"
			"<rgb>(optional) <inner cone angle> <outer cone angle>\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid spot light orientation\n", \
	" Spot light orientation must be in the format x,y,z," \
	" with values between zero and one\n");
	if (check_double(&bright_ratio, 0.0, 1.0))
		free_exit(rt_file, "Error\n Invalid light brightness\n", \
	" Light brightness must be between 0.0 and 1.0\n");
	check_sl2(line, rt_file, len);
}
