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

	len = ft_matrix_len(line);
	bright_ratio = line[3];
	if ( len != 6 && len != 7)
		free_exit(rt_file, "Error\n Invalid spot light parameters\n", \
	" Spot Lt must be in the format <type id> <x,y,z> <x,y,z> <brightness>\n"
		"<rgb>(optional) <inner cone angle> <outer cone angle>\n");
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid light coordinates\n", \
	" Light coordinates must be in the format x,y,z\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid spot light orientation\n", \
	" Spot light orientation must be in the format x,y,z," \
	" with values between zero and one\n");
	if (check_normalization(line[2]))
		free_exit(rt_file, "Error\n Invalid SL orientation vector\n", \
	"SL orientation vector must be normalized," \
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

//move this to utils
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

void	check_al(char **line, char ***rt_file)
{
	char	*bright_ratio;
	int		len;
	char 	*wid_str;
	char 	*len_str;

	len = ft_matrix_len(line);
	bright_ratio = line[3];
	wid_str = line[4];
	len_str = line[5];
	if (len != 9)
		free_exit(rt_file, "Error\n Invalid area light parameters\n", \
	" Area Lt must be in the format <type id> <x,y,z> <x,y,z> <brightness>\n"
		" <w> <l> <rgb> <w_cells> <l_cells>\n");
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid area light coordinates\n", \
	" Area light coordinates must be in the format x,y,z\n");
	if (check_orientation(line[2]))
		free_exit(rt_file, "Error\n Invalid area light orientation\n", \
	" Area light orientation must be in the format x,y,z," \
	" with values between zero and one\n");
	if (check_normalization(line[2]))
		free_exit(rt_file, "Error\n Invalid area light orientation vector\n", \
	"Area light orientation vector must be normalized," \
	" with magnitude of one\n");
	if (check_double(&bright_ratio, 0.0, 1.0))
		free_exit(rt_file, "Error\n Invalid light brightness\n", \
	" Light brightness must be between 0.0 and 1.0\n");
	if (check_double(&wid_str, 1.0, 10.0))
		free_exit(rt_file, "Error\n Invalid area light width\n", \
	" Area light width must be between 1 and 10\n");
	if (check_double(&len_str, 1.0, 10.0))
		free_exit(rt_file, "Error\n Invalid area light length\n", \
	" Area light length must be between 1 and 10\n");
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
