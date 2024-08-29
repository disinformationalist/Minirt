#include "minirt.h"

uint8_t round_c(double d)
{
	return (floor(d + .5));
}

bool	set_amb(t_amb **amb, char **line)
{
	char	*ratio_str;

	*amb = (t_amb *)malloc(sizeof(t_amb));
	if (!*amb)
		return (1);
	ratio_str = line[1];
	(*amb)->ratio = get_double(&ratio_str);
	(*amb)->amb_color = get_color(line[2]);
	(*amb)->amb_color.r = round_c((*amb)->ratio * (double)(*amb)->amb_color.r);
	(*amb)->amb_color.g = round_c((*amb)->ratio * (double)(*amb)->amb_color.g);
	(*amb)->amb_color.b = round_c((*amb)->ratio * (double)(*amb)->amb_color.b);

	return (0);
}

bool	set_cam(t_cam **cam, char **line)
{
	*cam = (t_cam *)malloc(sizeof(t_cam));
	if (!*cam)
		return (1);
	(*cam)->center = get_coordinates(line[1]);
	(*cam)->orient = get_coordinates(line[2]);
	(*cam)->fov = ft_atoi(line[3]);	
	return (0);
}

bool	set_light(t_light **light, char **line)
{
	char	*bright_ratio;
	
	bright_ratio = line[2];
	*light = (t_light *)malloc(sizeof(t_light));
	if (!*light)
		return (1);
	(*light)->center = get_coordinates(line[1]);
	(*light)->brightness = get_double(&bright_ratio);
	if (line[3])
		(*light)->light_color = get_color(line[3]);
	return (0);
}