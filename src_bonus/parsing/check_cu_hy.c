#include "minirt.h"

void	check_hy2(char **line, char ***rt_file)
{
	char	*rad1_str;
	char	*rad2_str;
	char	*height_str;	

	rad1_str = line[3];
	rad2_str = line[4];
	height_str = line[5];
	if (check_double(&rad1_str, 0.001, 999.0))
		free_exit(rt_file, "Error\n Invalid hyperboloid rad1\n", \
	"Hyperboloid rad1 must be between 0.001 and 999\n");
	if (check_double(&rad2_str, 0.001, 999.0))
		free_exit(rt_file, "Error\n Invalid hyperboloid rad2\n", \
	"Hyperboloid rad2 must be between 0.001 and 999\n");
	if (check_double(&height_str, 0.001, 999.0))
		free_exit(rt_file, "Error\n Invalid hyperboloid height\n", \
	"Hyperboloid height must be between 0.001 and 999\n");
	if (check_color(line[6]))
		free_exit(rt_file, "Error\n Invalid hyperboloid color value\n", \
	"Hyperboloid color channel values between 0 and 255 in the format r,g,b\n");
}

void	check_hy(char **line, char ***rt_file)
{
	check_str_len(line, rt_file, 8);
	if (check_param_num(line, 7))
		free_exit(rt_file, "Error\n Invalid hyperboloid parameters\n", \
	"Hyperboloid must be in the format <type id> <x,y,z> <x,y,z>" \
	"<rad1> <rad2> <height> <r,g,b>\n");
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid hyperboloid coordinates\n", \
	"Hyperboloid coordinates must be in the format x,y,z\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid hyperboloid orientation\n", \
	"Hyperboloid orientation must be in the format x,y,z," \
	"with values between minus one and one\n");
	if (check_normalization(line[2]))
		free_exit(rt_file, "Error\n Invalid hyperboloid orientation\n", \
	"Hyperboloid orientation vector must be normalized," \
	" with magnitude of one\n");
	check_hy2(line, rt_file);
}

void	check_cu2(char **line, char ***rt_file, \
	char *height_str, char *depth_str)
{
	if (check_normalization(line[2]))
		free_exit(rt_file, "Error\n Invalid cube orientation vector\n", \
	"Cube orientation vector must be normalized," \
	" with magnitude of one\n");
	if (check_double(&height_str, 0.001, 999.0))
		free_exit(rt_file, "Error\n Invalid cube height\n", \
	"Cube height must be between 0.001 and 999\n");
	if (check_double(&depth_str, 0.001, 999.0))
		free_exit(rt_file, "Error\n Invalid cube depth\n", \
	"Cube depth must be between 0.001 and 999\n");
	if (check_color(line[6]))
		free_exit(rt_file, "Error\n Invalid cube color value\n", \
	"Cube color channel values between 0 and 255 in the format r,g,b\n");
}

void	check_cu(char **line, char ***rt_file)
{
	char	*width_str;
	char	*height_str;
	char	*depth_str;	

	width_str = line[3];
	height_str = line[4];
	depth_str = line[5];
	check_str_len(line, rt_file, 7);
	if (check_param_num(line, 7))
		free_exit(rt_file, "Error\n Invalid Cube parameters\n Cube ", \
	"must be in the format <type id> <x,y,z> <x,y,z> <w> <h> <d> <r,g,b>\n");
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid cube coordinates\n", \
	"Cube coordinates must be in the format x,y,z\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid cube orientation\n", \
	"Cube orientation must be in the format x,y,z," \
	"with values between minus one and one\n");
	if (check_double(&width_str, 0.001, 999.0))
		free_exit(rt_file, "Error\n Invalid cube width\n", \
	"Cube width must be between 0.001 and 999\n");
	check_cu2(line, rt_file, height_str, depth_str);
}