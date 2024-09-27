#include "minirt.h"

void	add_cy_color(t_cylinder *cyl, char *line)
{
	int r;
	int g;
	int b;

	r = cyl->color.r;
	g = cyl->color.g;
	b = cyl->color.b;

	snprintf(line + ft_strlen(line), 200 - ft_strlen(line), "%d,%d,%d\n", r, g, b);
}

char	*build_cy_line(t_cylinder *cyl)//maybe just save and apply the transform for later version do this
{
	static char 	line[200];
	t_point			cen;
	t_vec3			n;
	double			d;
	double			h;
	int 			spaces;
	int				spaces2;
	int				spaces3;
	int				spaces4;
	t_matrix_4x4    transform;

	transform = inverse(cyl->transform);
	cen = mat_vec_mult(transform, vec(0, 0, 0, 1));
	n = norm_vec(mat_vec_mult(transform, vec(0, 1, 0, 0)));
	d = (2.0 / cyl->curr_scale.m[2][2]);
	h = (1.0 / cyl->curr_scale.m[1][1]) * cyl->height;
	
	spaces = 16 - count_chars(cen.x) - count_chars(cen.y) - count_chars(cen.z);
	spaces2 = 16 - count_chars(n.x) - count_chars(n.y) - count_chars(n.z);
	spaces3 = 7 - count_chars(d);
	spaces4 = 7 - count_chars(h);

	snprintf(line, sizeof(line), "cy          %.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s%.3f%*s%.3f%*s", cen.x, cen.y, cen.z, spaces, "", n.x, n.y, n.z, spaces2, "", d, spaces3, "", h, spaces4, "");
	
	add_cy_color(cyl, line);

	return (line);
}

//go through cyl list adding each cyl to the file.

void	write_cylinders(t_cylinder *cylinders, int fd)
{
	t_cylinder		*curr_cy;
	char			*line;
	
	if (cylinders == NULL)
		return ;
	ft_putstr_fd("#Cylinders: Cen_x | Cen_y | Cen_z         Nrm_x | Nrm_y | Nrm_z         Diameter   Height     R | G | B\n", fd);
	curr_cy = cylinders;
	while (true)
	{
		line = build_cy_line(curr_cy);
		write(fd, line, ft_strlen(line));
		curr_cy = curr_cy->next;
		if (curr_cy == cylinders)
			break;
	}
	write(fd, "\n\n", 2);
}
