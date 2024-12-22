#include "minirt.h"

void	add_cu_color(t_cube *cyl, char *line)
{
	int	r;
	int	g;
	int	b;

	r = cyl->color.r;
	g = cyl->color.g;
	b = cyl->color.b;
	snprintf(line + ft_strlen(line), \
		200 - ft_strlen(line), "%d,%d,%d\n", r, g, b);
}

//problem when rotating about the normal axis, like with the cam

char	*build_cu_line(t_cube *cu)
{
	static char		line[200];
	t_point			cen;
	t_vec3			n;
	double			w;
	double			h;

	cen = mat_vec_mult(cu->i_transform, vec(0, 0, 0, 1));
	check_tolerance(&cen);
	n = norm_vec(mat_vec_mult(cu->i_transform, vec(0, 1, 0, 0)));
	check_tolerance(&n);
	w = (2.0 / cu->curr_scale.mat[0]);
	h = (2.0 / cu->curr_scale.mat[5]);

	snprintf(line, sizeof(line), \
		"cu          %.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s%.3f%*s%.3f%*s%.3f%*s", \
		cen.x, cen.y, cen.z, 13 - count_chars(cen.x) - count_chars(cen.y) \
		- count_chars(cen.z), "", n.x, n.y, n.z, 13 - count_chars(n.x) \
		- count_chars(n.y) - count_chars(n.z), "", w, 4 - count_chars(w), \
		"", h, 4 - count_chars(h), "", (2.0 / cu->curr_scale.mat[10]), \
		4 - count_chars((2.0 / cu->curr_scale.mat[10])), "");
	add_cu_color(cu, line);
	return (line);
}

//go through cyl list adding each cyl to the file.

void	write_cubes(t_cube *cubes, int fd)
{
	t_cube		*curr_cu;
	char		*line;

	if (cubes == NULL)
		return ;
	ft_putstr_fd("#Cubes:     Cen_x | Cen_y | Cen_z      "
		"Nrm_x | Nrm_y | Nrm_z      W_(x)   H_(y)   D_(z)   "
		"R | G | B\n", fd);
	curr_cu = cubes;
	while (true)
	{
		line = build_cu_line(curr_cu);
		write(fd, line, ft_strlen(line));
		curr_cu = curr_cu->next;
		if (curr_cu == cubes)
			break ;
	}
	write(fd, "\n\n", 2);
}
