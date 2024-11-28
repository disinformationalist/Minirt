#include "minirt.h"

void	add_lt_color(t_light *light, char *line)
{
	int	r;
	int	g;
	int	b;

	r = ft_round(light->color.r * 255.0);
	g = ft_round(light->color.g * 255.0);
	b = ft_round(light->color.b * 255.0);
	snprintf(line + ft_strlen(line), 200 - ft_strlen(line), \
		"%d,%d,%d\n", r, g, b);
}

char	*build_lt_line(t_light *light)
{
	static char		line[200];
	t_point			cen;
	double			bright;
	int				sp;
	int				sp2;

	cen = light->center;
	bright = light->brightness;
	sp = 40 - count_chars(cen.x) - count_chars(cen.y) - count_chars(cen.z);
	sp2 = 20 - count_chars(bright);
	snprintf(line, sizeof(line), "L           %.3f,%.3f,%.3f%*s%.3f%*s", \
		cen.x, cen.y, cen.z, sp, "", bright, sp2, "");
	add_lt_color(light, line);
	return (line);
}

//count number of point lights

int	count_lights(t_light *lights)
{
	t_light	*curr_lt;
	int		count;

	count = 0;
	curr_lt = lights;
	while (true)
	{
		if (curr_lt->type == POINT)
			count++;
		curr_lt = curr_lt->next;
		if (curr_lt == lights)
			break ;
	}
	return (count);
}

//go through light list adding each light to the file.

void	write_lights(t_light *lights, int fd)
{
	t_light		*curr_lt;
	char		*line;

	if (lights == NULL || !count_lights(lights))
		return ;
	ft_putstr_fd("#Lights:    Cen_x | Cen_y | Cen_z 					"\
		"			  Intensity               R | G | B\n", fd);
	curr_lt = lights;
	while (true)
	{
		if (curr_lt->type == POINT)
		{
			line = build_lt_line(curr_lt);
			write(fd, line, ft_strlen(line));
		}
		curr_lt = curr_lt->next;
		if (curr_lt == lights)
			break ;
	}
	write(fd, "\n\n", 2);
}
