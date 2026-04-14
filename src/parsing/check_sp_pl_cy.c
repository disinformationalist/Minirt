#include "minirt.h"

//must set str to be start at the tx first char address whem passing

void	check_sp(char **line, char ***rt_file)
{
	char	*diam_str = NULL;
	char	*col_str = NULL;
	/* int		len;
	int		n = 5;

	len = ft_matrix_len(line);
	if (len != n && len != n + 1)
		free_exit(rt_file, "Error\n Invalid sphere parameters\n", \
			"Sphere must be in the format <type id> <x,y,z> <x,y,z>(optional orientation) <diameter> <r,g,b>\n"); */
	diam_str = line[3];
	col_str = line[4];
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid sphere coordinates\n", \
	"sphere coordinates must be in the format x,y,z\n");
	if (check_double(&diam_str, 0.001, 1001.0))
		free_exit(rt_file, "Error\n Invalid sphere diameter\n", \
	"Sphere diameter must be between 0.001 and 1001.0\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid Sphere orientation\n", \
	"Spere orientation must be in the format x,y,z," \
	"with values between minus one and one\n");
	if (col_str && check_color(col_str))
		free_exit(rt_file, "Error\n Invalid sphere color value\n", \
	"Sphere color channel values must be between 0 and 255" \
	"in the format r,g,b\n");
/* 	if (len == n + 1)
	{
		todo:	add a tx check here 
		printf("%s\n", (line[n]));
	
	} */
//todo: add checks for material, also performing len check is now bunk
}

void	check_pl(char **line, char ***rt_file)
{
	/* int		len;
	int		n = 5;

	len = ft_matrix_len(line);
	if (len != n  && len != n - 1)
		free_exit(rt_file, "Error\n Invalid plane parameters\n", \
	"Plane must be in the format <type id> <x,y,z> <x,y,z> <r,g,b>\n"); */
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid plane coordinates\n", \
	"Plane coordinates must be in the format x,y,z\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid plane orientation\n", \
	"Plane normal vector must be in the format x,y,z," \
	"with values between minus one and one\n");
	if (check_color(line[3]))
		free_exit(rt_file, "Error\n Invalid plane color value\n", \
	"Plane color channel values must be between 0 and 255" \
	"in the format r,g,b\n");
	/* if (len == n)
		check_tx(line, rt_file); */
}

void	check_cy2(char **line, char ***rt_file, char *height_str)
{
	if (check_double(&height_str, 0.001, 999.0))
		free_exit(rt_file, "Error\n Invalid cylinder height\n", \
	"Cylinder height must be between 0.001 and 999\n");
	if (check_color(line[5]))
		free_exit(rt_file, "Error\n Invalid cylinder color value\n", \
	"Cylinder color channel values between 0 and 255 in the format r,g,b\n");
	/* if (len == n)
		check_tx(line, rt_file); */
}

void	check_cy(char **line, char ***rt_file)
{
	char	*diam_str;
	char	*height_str;
	/* int		len;
	int		n = 7; */	

	diam_str = line[3];
	height_str = line[4];
	/* len = ft_matrix_len(line);
	if (len != n  && len != n - 1)
		free_exit(rt_file, "Error\n Invalid cylinder parameters\n", \
	"Cylinder must be in the format <type id> <x,y,z> <x,y,z>" \
	"<diameter> <height> <r,g,b>\n"); */
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid cylinder coordinates\n", \
	"Cylinder coordinates must be in the format x,y,z\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid cylinder orientation\n", \
	"Cylinder orientation must be in the format x,y,z," \
	"with values between minus one and one\n");
	if (check_double(&diam_str, 0.001, 999.0))
		free_exit(rt_file, "Error\n Invalid cylinder diameter\n", \
	"Cylinder diameter must be between 0.001 and 999\n");
	check_cy2(line, rt_file, height_str);
}
