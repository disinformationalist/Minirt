#include "minirt.h"

void	add_pl_color(t_plane *plane, char *line)
{
	int r;
	int g;
	int b;

	r = plane->color.r;
	g = plane->color.g;
	b = plane->color.b;

	snprintf(line + ft_strlen(line), 200 - ft_strlen(line), "%d,%d,%d\n", r, g, b);
}

char	*build_pl_line(t_plane *plane)
{
	static char 	line[200];
	t_point			p;
	t_vec3			n;
	int 			spaces;
	int				spaces2;

	p = plane->point;
	n = plane->norm_vector;
	spaces = 16 - count_chars(p.x) - count_chars(p.y) - count_chars(p.z);
	spaces2 = 38 - count_chars(n.x) - count_chars(n.y) - count_chars(n.z);
	snprintf(line, sizeof(line), "pl          %.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s", p.x, p.y, p.z, spaces, "", n.x, n.y, n.z, spaces2, "");
	
	add_pl_color(plane, line);

	return (line);
}

//go through planes list adding each sphere to the file.

void write_planes(t_plane *planes, int fd)
{
	t_plane		*curr_pl;
	char		*line;
	
	if (planes == NULL)
		return ;
	ft_putstr_fd("#Planes:    Pnt_x | Pnt_y | Pnt_z         Nrm_x | Nrm_y | Nrm_z      			 	  		  R | G | B\n", fd);
	curr_pl = planes;
	while (true)
	{
		line = build_pl_line(curr_pl);
		write(fd, line, ft_strlen(line));
		curr_pl = curr_pl->next;
		if (curr_pl == planes)
			break;
	}
	write(fd, "\n\n", 2);
}