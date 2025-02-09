#include "minirt.h"

void	add_pl_color(t_plane *plane, char *line)
{
	int	r;
	int	g;
	int	b;

	r = plane->color.r;
	g = plane->color.g;
	b = plane->color.b;
	snprintf(line + ft_strlen(line), \
		200 - ft_strlen(line), "%d,%d,%d\n", r, g, b);
}

char	*build_pl_line(t_plane *plane)
{
	static char		line[200];
	t_point			p;
	t_vec3			n;
	int				spaces;
	t_mat4	transform;

	transform = inverse(plane->transform);
	p = mat_vec_mult(transform, vec(0, 0, 0, 1));
	n = norm_vec(mat_vec_mult(transform, vec(0, 1, 0, 0)));
	check_tolerance(&n);
	spaces = 13 - count_chars(p.x) - count_chars(p.y) - count_chars(p.z);
	snprintf(line, sizeof(line), \
		"pl          %.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s", \
		p.x, p.y, p.z, spaces, "", n.x, n.y, n.z, \
		37 - count_chars(n.x) - count_chars(n.y) - count_chars(n.z), "");
	add_pl_color(plane, line);
	return (line);
}

//go through planes list adding each plane to the file.

void	write_planes(t_plane *planes, int fd)
{
	t_plane		*curr_pl;
	char		*line;

	if (planes == NULL)
		return ;
	ft_putstr_fd("#Planes:    Pnt_x | Pnt_y | Pnt_z      "
		"Nrm_x | Nrm_y | Nrm_z      			 	  	"
		"	  R | G | B\n", fd);
	curr_pl = planes;
	while (true)
	{
		line = build_pl_line(curr_pl);
		write(fd, line, ft_strlen(line));
		curr_pl = curr_pl->next;
		if (curr_pl == planes)
			break ;
	}
	write(fd, "\n\n", 2);
}
