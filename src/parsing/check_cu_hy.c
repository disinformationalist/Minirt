#include "minirt.h"

void	check_hy2(char **line, char ***rt_file)
{
	char	*rad_str;
	char	*waist_str;
	char	*height_str;	

	rad_str = line[3];
	waist_str = line[4];
	height_str = line[5];
	if (check_double(&rad_str, 0.001, 999.0))
		free_exit(rt_file, "Error\n Invalid hyperboloid rad\n", \
	"Hyperboloid rad must be between 0.001 and 999\n");
	if (check_double(&waist_str, -1, 1))
		free_exit(rt_file, "Error\n Invalid hyperboloid waist\n", \
	"Hyperboloid waist must be between -1 and 1\n");
	if (check_double(&height_str, 0.001, 999.0))
		free_exit(rt_file, "Error\n Invalid hyperboloid height\n", \
	"Hyperboloid height must be between 0.001 and 999\n");
	if (check_color(line[6]))
		free_exit(rt_file, "Error\n Invalid hyperboloid color value\n", \
	"Hyperboloid color channel values between 0 and 255 in the format r,g,b\n");
}

void	check_hy(char **line, char ***rt_file)
{
	if (check_param_num(line, 7))
		free_exit(rt_file, "Error\n Invalid hyperboloid parameters\n", \
	"Hyperboloid must be in the format <type id> <x,y,z> <x,y,z>" \
	"<rad> <waist> <height> <r,g,b>\n");
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid hyperboloid coordinates\n", \
	" Hyperboloid coordinates must be in the format x,y,z\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid hyperboloid orientation\n", \
	"Hyperboloid orientation must be in the format x,y,z," \
	"with values between minus one and one\n");
	check_hy2(line, rt_file);
}

void	check_cu2(char **line, char ***rt_file, \
	char *height_str, char *depth_str)
{
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
