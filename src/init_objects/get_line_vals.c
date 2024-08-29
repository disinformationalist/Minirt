#include "minirt.h"

double	get_double(char **doub_str)
{
	int i;
	double ret;
	char str[50];

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
//	printf("double ret: %f\n", ret);
	return (ret);
}

uint8_t get_rgb(char **s)
{
	int 	i;
	char 	str[3];
	int		chan_color;

	i = -1;
	while (**s != ',' && **s != '\0')
	{
		str[++i] = **s;
		(*s)++;
	}
	str[++i] = '\0';
	chan_color = ft_atoi(str);
//	printf("chan_color: %d\n", chan_color);
	return (chan_color);
}

t_color	get_color(char *color_str)
{
	t_color color;

	color.r = get_rgb(&color_str);
	color_str++;
	color.g = get_rgb(&color_str);
	color_str++;
	color.b = get_rgb(&color_str);
	return (color);
}

t_vec3	get_coordinates(char *coord_str)
{
	t_vec3 coord;

	coord.x = get_double(&coord_str);
	coord_str++;
	coord.y = get_double(&coord_str);
	coord_str++;
	coord.z = get_double(&coord_str);
	return (coord);
}
