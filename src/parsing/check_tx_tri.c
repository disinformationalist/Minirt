#include "minirt.h"

int	is_valid_tx(char *filename)
{
	int		len;
	char	*dot;

	len = ft_strlen(filename);
	if (len < 5)
		return (1);
	dot = ft_strrchr(filename, '.');
	if (!dot || ft_strcmp(dot, ".png"))
		return (2);
	if (len > 29)
		return (3);
	return (0);
}

void	check_tx(char **line, char ***rt_file)
{
	int		len;
	int		status1;
	int		status2;

	len = ft_matrix_len(line);
	status1 = is_valid_tx(line[1]);
	status2 = -1;
	if (line[2])
		status2 = is_valid_tx(line[2]);
	if (len != 2 && len != 3)
		free_exit(rt_file, "Error\n Invalid texture parameters\n Texture " \
	"has the format <type id> <image.png> <image.png>(optional)\n", RED);
	if (status1 == 1 || status1 == 2)
		free_exit(rt_file, "Error\n Invalid texture parameters\n Texture " \
	"has the format <type id> <image.png> <image.png>(optional)\n", RED);
	if (status2 == 1 || status2 == 2)
		free_exit(rt_file, "Error\n Invalid texture parameters\n texture " \
	"has the format <type id> <image.png> <image.png>(optional)\n", RED);
	if (status1 == 3 || status2 == 3)
		free_exit(rt_file, "Error\n Invalid texture parameters\n texture " \
	"filename too long\n", RED);
}

void	check_tri(char **line, char ***rt_file)
{
	if (check_param_num(line, 5))
		free_exit(rt_file, "Error\n Invalid triangle parameters\n Triangle ", \
	"must be in the format <type id> <x,y,z> <x,y,z> <x,y,z>\n");
	if (check_coordinates(line[1]))
		free_exit(rt_file, "Error\n Invalid triangle point\n", \
	"Triangle point must be in the format x,y,z\n");
	if (check_coordinates(line[2]))
		free_exit(rt_file, "Error\n Invalid triangle point\n", \
	"Triangle point must be in the format x,y,z\n");
	if (check_coordinates(line[3]))
		free_exit(rt_file, "Error\n Invalid triangle point\n", \
	"Triangle point must be in the format x,y,z\n");
	if (check_color(line[4]))
		free_exit(rt_file, "Error\n Invalid cube color value\n", \
	"Cube color channel values between 0 and 255 in the format r,g,b\n");
}
