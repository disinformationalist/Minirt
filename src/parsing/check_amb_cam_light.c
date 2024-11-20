#include "minirt.h"

void	check_amb(char **line, char ***rt_file)
{
	char	*ratio_str;	

	ratio_str = line[1];
	check_str_len(line, rt_file, 3);
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

	check_str_len(line, rt_file, 3);
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
}
