#include "minirt.h"

void	check_sp(char **line, char ***rt_file)
{
	char	*diam_str;

	diam_str = line[2];
	if (check_param_num(line, 4))
		free_exit(rt_file, "Error\n Invalid sphere parameters\n", \
	"Sphere must be in the format <type id> <x,y,z> <diameter> <r,g,b>\n");
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid sphere coordinates\n", \
	"sphere coordinates must be in the format x,y,z\n");
	if (check_double(&diam_str, 0.0001, 9999.0))
		free_exit(rt_file, "Error\n Invalid sphere diameter\n", \
	"Sphere diameter must be between 0.0001 and 9999.0\n");
	if (check_color(line[3]))
		free_exit(rt_file, "Error\n Invalid sphere color value\n", \
	"Sphere color channel values must be between 0 and 255" \
	"in the format r,g,b\n");
}

void	check_pl(char **line, char ***rt_file)
{
	if (check_param_num(line, 4))
		free_exit(rt_file, "Error\n Invalid plane parameters\n", \
	"Plane must be in the format <type id> <x,y,z> <x,y,z> <r,g,b>\n");
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid plane coordinates\n", \
	"Plane coordinates must be in the format x,y,z\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid plane orientation\n", \
	"Plane normal vector must be in the format x,y,z," \
	"with values between minus one and one\n");
	if (check_normalization(line[2]))
		free_exit(rt_file, "Error\n Invalid plane orientation vector\n", \
	"Plane orientation vector must be normalized," \
	" with magnitude of one\n");
	if (check_color(line[3]))
		free_exit(rt_file, "Error\n Invalid plane color value\n", \
	"Plane color channel values must be between 0 and 255" \
	"in the format r,g,b\n");
}

void	check_cy2(char **line, char ***rt_file, char *height_str)
{
	if (check_double(&height_str, 0.0001, 9999.0))
		free_exit(rt_file, "Error\n Invalid cylinder height\n", \
	"Cylinder height must be between 0.0001 and 9999\n");
	if (check_color(line[5]))
		free_exit(rt_file, "Error\n Invalid cylinder color value\n", \
	"Cylinder color channel values between 0 and 255 in the format r,g,b\n");
}

void	check_cy(char **line, char ***rt_file)
{
	char	*diam_str;
	char	*height_str;	

	diam_str = line[3];
	height_str = line[4];
	if (check_param_num(line, 6))
		free_exit(rt_file, "Error\n Invalid cylinder parameters\n", \
	"Cylinder must be in the format <type id> <x,y,z> <x,y,z>" \
	"<diameter> <height> <r,g,b>\n");
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid cylinder coordinates\n", \
	"Cylinder coordinates must be in the format x,y,z\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid cylinder orientation\n", \
	"Cylinder orientation must be in the format x,y,z," \
	"with values between minus one and one\n");
	if (check_normalization(line[2]))
		free_exit(rt_file, "Error\n Invalid cylinder orientation vector\n", \
	"Cylinder orientation vector must be normalized," \
	" with magnitude of one\n");
	if (check_double(&diam_str, 0.0001, 9999.0))
		free_exit(rt_file, "Error\n Invalid cylinder diameter\n", \
	"Cylinder diameter must be between 0.0001 and 9999\n");
	check_cy2(line, rt_file, height_str);
}

void	check_le(char **line, char ***rt_file)
{
	char	*diam_str_1;
	char	*diam_str_2;

	diam_str_1 = line[2];
	diam_str_2 = line[5];
	if (check_param_num(line, 7))
		free_exit(rt_file, "Error\n Invalid lens parameters\n", \
	"Required lens spheres format: <type id> <x,y,z> <diameter> <r,g,b>\n");
	if (check_coordinates(line[1]) || check_coordinates(line[4]))
		free_exit(rt_file, "Error\n Invalid lens sphere coordinates\n", \
	"Lens sphere coordinates must be in the format x,y,z\n");
	if (check_double(&diam_str_1, 0.0001, 9999.0)
		|| check_double(&diam_str_2, 0.0001, 9999.0))
		free_exit(rt_file, "Error\n Invalid lens sphere diameter\n", \
	"Lens sphere diameter must be between 0.0001 and 9999.0\n");
	if (check_spheres_intersect(line[1], line[2], line[4], line[5]))
		free_exit(rt_file, "Error\n Invalid lens parameters\n", \
	"Lens spheres must intersect\n");
	if (check_color(line[3]) || check_color(line[6]))
		free_exit(rt_file, "Error\n Invalid lens sphere color value\n", \
	"Lens sphere color channel values must be between 0 and 255" \
	"in the format r,g,b\n");
}