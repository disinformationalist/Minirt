#include "minirt.h"

void	add_splt_color(t_light *light, char *line)
{
	int	r;
	int	g;
	int	b;
	int	sp4;

	r = ft_round(light->color.r * 255.0);
	g = ft_round(light->color.g * 255.0);
	b = ft_round(light->color.b * 255.0);
	sp4 = 14 - count_chars(r) - count_chars(g) - count_chars(b);
	snprintf(line + ft_strlen(line), \
		200 - ft_strlen(line), "%d,%d,%d%*s", r, g, b, sp4, "");
}

void	add_lt_cones(t_light *light, char *line)
{
	int	in_angle;
	int	out_angle;
	int	sp5;

	in_angle = ft_round(acos(light->inner_cone) / DEG_TO_RAD);
	out_angle = ft_round(acos(light->outer_cone) / DEG_TO_RAD);
	sp5 = 13 - count_chars(in_angle);
	snprintf(line + ft_strlen(line), 200 - ft_strlen(line), \
		"%d%*s%d\n", in_angle, sp5, "", out_angle);
}

char	*build_splt_line(t_light *light)
{
	static char		line[200];
	t_point			cen;
	t_vec3			n;
	double			bright;

	cen = light->center;
	n = neg(light->dir);
	check_tolerance(&n);
	bright = light->brightness;
	snprintf(line, sizeof(line), \
		"SL          %.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s%.3f%*s", \
		cen.x, cen.y, cen.z, 13 - count_chars(cen.x) - count_chars(cen.y) \
		- count_chars(cen.z), "", n.x, n.y, n.z, \
		13 - count_chars(n.x) - count_chars(n.y) - count_chars(n.z), \
		"", bright, 20 - count_chars(bright), "");
	add_splt_color(light, line);
	add_lt_cones(light, line);
	return (line);
}

//count number of spot lights

int	count_splights(t_light *lights)
{
	t_light	*curr_lt;
	int		count;

	count = 0;
	curr_lt = lights;
	while (true)
	{
		if (curr_lt->type == SPOT)
			count++;
		curr_lt = curr_lt->next;
		if (curr_lt == lights)
			break ;
	}
	return (count);
}

//go through light list adding each light to the file.

void	write_splights(t_light *lights, int fd)
{
	t_light		*curr_lt;
	char		*line;

	if (lights == NULL || !count_splights(lights))
		return ;
	ft_putstr_fd("#SpLights:  Cen_x | Cen_y | Cen_z      "\
		"Ori_x | Ori_y | Ori_z      Intensity			      "\
		"R | G | B       INNERANGLE   OUTERANGLE\n", fd);
	curr_lt = lights;
	while (true)
	{
		if (curr_lt->type == SPOT)
		{
			line = build_splt_line(curr_lt);
			write(fd, line, ft_strlen(line));
		}
		curr_lt = curr_lt->next;
		if (curr_lt == lights)
			break ;
	}
	write(fd, "\n\n", 2);
}
