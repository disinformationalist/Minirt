#include "minirt.h"

void	add_hy_color(t_hyperboloid *hyp, char *line)
{
	int	r;
	int	g;
	int	b;

	r = hyp->color.r;
	g = hyp->color.g;
	b = hyp->color.b;
	snprintf(line + ft_strlen(line), 200 - ft_strlen(line),
		"%d,%d,%d\n", r, g, b);
}

char	*build_hy_line(t_hyperboloid *hyp)
{
	static char	line[200];
	t_point		cen;
	t_vec3		n;
	double		r;
	double		h;
	double		waist;

	cen = mat_vec_mult(hyp->i_transform, vec(0, 0, 0, 1));
	check_tolerance(&cen);
	n = norm_vec(mat_vec_mult(hyp->i_transform, vec(0, 1, 0, 0)));
	check_tolerance(&n);
	r = (1.0 / hyp->curr_scale.mat[10]);
	h = (2.0 / hyp->curr_scale.mat[5]);
	waist = hyp->waist_val;

	snprintf(line, sizeof(line), \
		"hy          %.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s%.3f%*s%.3f%*s%.3f%*s", \
		cen.x, cen.y, cen.z, 13 - count_chars(cen.x) - count_chars(cen.y) \
		- count_chars(cen.z), "", n.x, n.y, n.z, 13 - count_chars(n.x) \
		- count_chars(n.y) - count_chars(n.z), "", r, 4 - count_chars(r), \
		"", waist, 4 - count_chars(h), "", h, \
		4 - count_chars(waist), "");
	add_hy_color(hyp, line);
	return (line);
}

//go through hyp list adding each hyp to the file.

void	write_hyperboloids(t_hyperboloid *hyperboloids, int fd)
{
	t_hyperboloid	*curr_hy;
	char			*line;

	if (hyperboloids == NULL)
		return ;
	ft_putstr_fd("#Hypes:     Cen_x | Cen_y | Cen_z      "
				"Nrm_x | Nrm_y | Nrm_z      Rad     Waist   "
				"Height  R | G | B\n", fd);
	curr_hy = hyperboloids;
	while (true)
	{
		line = build_hy_line(curr_hy);
		write(fd, line, ft_strlen(line));
		curr_hy = curr_hy->next;
		if (curr_hy == hyperboloids)
			break ;
	}
	write(fd, "\n\n", 2);
}
