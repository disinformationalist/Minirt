#include "minirt.h"

double	get_double(char **doub_str)
{
	int		i;
	double	ret;
	char	str[50];

	i = -1;
	if (**doub_str == '-')
	{
		str[++i] = **doub_str;
		(*doub_str)++;
	}
	while (**doub_str != ',' && **doub_str != '\0')
	{
		str[++i] = **doub_str;
		(*doub_str)++;
	}
	str[++i] = '\0';
	ret = ft_atof(str);
	return (ret);
}

double	get_rgb(char **s, double val)
{
	int		i;
	char	str[3];
	int		chan_color;
	double	norm_color;

	i = -1;
	while (**s != ',' && **s != '\0')
	{
		str[++i] = **s;
		(*s)++;
	}
	str[++i] = '\0';
	chan_color = ft_atoi(str);
	norm_color = (double)chan_color / val;
	return (norm_color);
}

//val decides if we norm the color or not. passes in the devisor
//obj colors are not normed, light colors are.

t_norm_color	get_color(char *color_str, double val)
{
	t_norm_color	color;

	color.r = get_rgb(&color_str, val);
	color_str++;
	color.g = get_rgb(&color_str, val);
	color_str++;
	color.b = get_rgb(&color_str, val);
	return (color);
}

t_vec3	get_coordinates(char *coord_str, double w)
{
	t_vec3	coord;

	coord.x = get_double(&coord_str);
	coord_str++;
	coord.y = get_double(&coord_str);
	coord_str++;
	coord.z = get_double(&coord_str);
	coord.w = w;
	return (coord);
}
