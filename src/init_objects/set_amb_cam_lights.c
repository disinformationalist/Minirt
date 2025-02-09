#include "minirt.h"

//ambient has already adjusted its color based on intensity

bool	set_amb(t_amb **amb, char **line)
{
	char	*ratio_str;

	*amb = (t_amb *)malloc(sizeof(t_amb));
	if (!*amb)
		return (1);
	ratio_str = line[1];
	(*amb)->ratio = get_double(&ratio_str);
	(*amb)->color = get_color(line[2], 255.0);
	(*amb)->color.r = (*amb)->ratio * (*amb)->color.r;
	(*amb)->color.g = (*amb)->ratio * (*amb)->color.g;
	(*amb)->color.b = (*amb)->ratio * (*amb)->color.b;
	return (0);
}

bool	set_empty_amb(t_amb **amb)
{
	*amb = (t_amb *)malloc(sizeof(t_amb));
	if (!*amb)
		return (1);
	(*amb)->ratio = 0;
	//(*amb)->color = (t_norm_color)(0, 0, 0);
	(*amb)->color.r = 0;
	(*amb)->color.g = 0;
	(*amb)->color.b = 0;
	(*amb)->color.r = (*amb)->ratio * (*amb)->color.r;
	(*amb)->color.g = (*amb)->ratio * (*amb)->color.g;
	(*amb)->color.b = (*amb)->ratio * (*amb)->color.b;
	return (0);
}

bool	set_cam(t_cam **cam, char **line)
{
	*cam = (t_cam *)malloc(sizeof(t_cam));
	if (!*cam)
		return (1);
	(*cam)->center = get_coordinates(line[1], 1.0);
	(*cam)->orient = norm_vec(get_coordinates(line[2], 0.0));
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
	(*light)->center = get_coordinates(line[1], 1.0);
	(*light)->brightness = get_double(&bright_ratio);
	if (line[3])
		(*light)->color = get_color(line[3], 255.0);
	return (0);
}
