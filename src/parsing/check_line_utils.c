#include "minirt.h"

void	free_exit(char ***rt_file, char *msg1, char *msg2)
{
	if (msg1)
		ft_putstr_color_fd(2, msg1, RED);
	if (msg2)
		ft_putstr_color_fd(2, msg2, RED);
	if (rt_file)
		free_3d_array_i(rt_file, ft_3darray_len(rt_file));
	exit(EXIT_FAILURE);
}

//used to check param length of each item in a line

void	check_str_len(char **line, char ***rt_file)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (ft_strlen(line[i]) > 30)
			free_exit(rt_file, "Error\n Parameter string too long\n", NULL);
	}
}

static inline int	check_rgb_color(char **s)
{
	int		i;
	char	str[50];
	int		chan_color;

	i = -1;
	while (**s != ',' && **s != '\0')
	{
		if (!isdigit((**s)))
			return (-1);
		str[++i] = **s;
		(*s)++;
	}
	str[++i] = '\0';
	chan_color = ft_atoi(str);
	if (chan_color < 0 || chan_color > 255)
		return (-1);
	return (0);
}

int	check_color(char *color_str)
{
	if (check_rgb_color(&color_str) == -1)
		return (1);
	if (*(color_str) != ',')
		return (1);
	color_str++;
	if (check_rgb_color(&color_str) == -1)
		return (1);
	if (*(color_str) != ',')
		return (1);
	color_str++;
	if (check_rgb_color(&color_str) == -1)
		return (1);
	if (*(color_str) != '\0')
		return (1);
	return (0);
}

int	check_double(char **doub_str, double lower_lim, double upper_lim)
{
	int		i;
	double	ratio;
	int		dot_cnt;
	char	str[50];

	i = -1;
	dot_cnt = 0;
	if (**doub_str == '-')
	{
		str[++i] = **doub_str;
		(*doub_str)++;
	}
	while (**doub_str != ',' && **doub_str != '\0')
	{
		if (**doub_str == '.')
			dot_cnt++;
		if ((!ft_isdigit(**doub_str) && **doub_str != '.') || dot_cnt > 1)
			return (1);
		str[++i] = **doub_str;
		(*doub_str)++;
	}
	str[++i] = '\0';
	ratio = (double)ft_atof(str);
	return ((ratio < lower_lim || ratio > upper_lim) == true);
}
