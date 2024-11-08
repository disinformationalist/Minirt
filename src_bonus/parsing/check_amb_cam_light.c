#include "minirt.h"

void	check_amb(char **line, char ***rt_file)
{
	char	*ratio_str;	

	check_str_len(line, rt_file, 3);
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
	check_str_len(line, rt_file, 4);
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
	if (check_normalization(line[2]))
		free_exit(rt_file, "Error\n Invalid camera orientation vector\n", \
	"Camera orientation vector must be normalized," \
	" with magnitude of one\n");
	if (check_fov(line[3]))
		free_exit(rt_file, "Error\n Invalid camera FOV\n", \
	" Camera FOV must be between 0 and 180\n");
}

void	check_light(char **line, char ***rt_file)
{
	char	*bright_ratio;

	check_str_len(line, rt_file, 4);
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

void	check_sl(char **line, char ***rt_file)
{
	char	*bright_ratio;
	int		len;

	check_str_len(line, rt_file, 7);
	len = ft_matrix_len(line);
	bright_ratio = line[3];
	if ( len != 6 && len != 7)
		free_exit(rt_file, "Error\n Invalid light parameters\n", \
	" Spot Lt must be in the format <type id> <x,y,z> <x,y,z> <brightness>\n"
		"<rgb> <inner cone angle> <outer cone angle> (<rgb> is optional)\n");
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid light coordinates\n", \
	" Light coordinates must be in the format x,y,z\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid camera orientation\n", \
	" Camera orientation must be in the format x,y,z," \
	" with values between zero and one\n");
	if (check_normalization(line[2]))
		free_exit(rt_file, "Error\n Invalid camera orientation vector\n", \
	"Camera orientation vector must be normalized," \
	" with magnitude of one\n");
	if (check_double(&bright_ratio, 0.0, 1.0))
		free_exit(rt_file, "Error\n Invalid light brightness\n", \
	" Light brightness must be between 0.0 and 1.0\n");
	if (len == 6)
	{
		if (check_fov(line[4]))
			free_exit(rt_file, "Error\n Invalid light inner cone angle\n", \
		" light inner cone angle must be between 0 and 180\n");
		if (check_fov(line[5]))
			free_exit(rt_file, "Error\n Invalid light outer cone angle\n", \
		" light outer cone angle must be between 0 and 180\n");
	}
	else if (len == 7)
	{
		if (check_color(line[4]))
			free_exit(rt_file, "Error\n Invalid light color value\n", \
		" Light color channel values must be between 0 and 255" \
		" in the format r,g,b\n");
		if (check_fov(line[5]))
			free_exit(rt_file, "Error\n Invalid light inner cone angle\n", \
		" light inner cone angle must be between 0 and 180\n");
		if (check_fov(line[6]))
			free_exit(rt_file, "Error\n Invalid light outer cone angle\n", \
		" light outer cone angle must be between 0 and 180\n");
	}
}
