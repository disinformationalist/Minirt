#include "minirt.h"

void	add_alt_color(t_light *light, char *line)
{
	int	r;
	int	g;
	int	b;
	int	spaces6;

	r = ft_round(light->color.r * 255.0);
	g = ft_round(light->color.g * 255.0);
	b = ft_round(light->color.b * 255.0);
	spaces6 = 14 - count_chars(r) - count_chars(g) - count_chars(b);
	snprintf(line + ft_strlen(line), \
		200 - ft_strlen(line), "%d,%d,%d%*s", r, g, b, spaces6, "");
}

void	add_lt_cells(t_light *light, char *line)
{
	int	w_cells;
	int	l_cells;
	int	spaces7;

	w_cells = light->usteps;
	l_cells = light->vsteps;
	spaces7 = 10 - count_chars(w_cells);
	snprintf(line + ft_strlen(line), \
		200 - ft_strlen(line), "%d%*s%d\n", w_cells, spaces7, "", l_cells);
}

char	*build_alt_line(t_light *light)
{
	static char		line[200];
	t_point			cen;
	t_vec3			n;
	double			wid;
	double			len;

	wid = (2.0 * light->curr_scale.mat[0]);
	len = (2.0 * light->curr_scale.mat[10]);
	cen = light->center;
	n = light->dir;
	check_tolerance(&n);
	snprintf(line, sizeof(line), "AL          "\
		"%.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s%.3f%*s%.3f%*s%.3f%*s", \
		cen.x, cen.y, cen.z, 13 - count_chars(cen.x) - count_chars(cen.y) \
		- count_chars(cen.z), "", n.x, n.y, n.z, 13 - count_chars(n.x) \
		- count_chars(n.y) - count_chars(n.z), "", light->brightness, \
		7 - count_chars(light->brightness), "", wid, 2 - count_chars(wid), \
		"", len, 3 - count_chars(len), "");
	add_alt_color(light, line);
	add_lt_cells(light, line);
	return (line);
}

//count number of area lights

int	count_alights(t_light *lights)
{
	t_light	*curr_lt;
	int		count;

	count = 0;
	curr_lt = lights;
	while (true)
	{
		if (curr_lt->type == AREA)
			count++;
		curr_lt = curr_lt->next;
		if (curr_lt == lights)
			break ;
	}
	return (count);
}

//go through light list adding each light to the file.

void	write_arealights(t_light *lights, int fd)
{
	t_light		*curr_lt;
	char		*line;

	if (lights == NULL || !count_alights(lights))
		return ;
	ft_putstr_fd("#ALights:   Cen_x | Cen_y | Cen_z      "
		"Ori_x | Ori_y | Ori_z      Intensity	 W_(x) L_(z)  R | G | B"
		"    	  W_cells  L_cells\n", fd);
	curr_lt = lights;
	while (true)
	{
		if (curr_lt->type == AREA)
		{
			line = build_alt_line(curr_lt);
			write(fd, line, ft_strlen(line));
		}
		curr_lt = curr_lt->next;
		if (curr_lt == lights)
			break ;
	}
	write(fd, "\n\n", 2);
}
