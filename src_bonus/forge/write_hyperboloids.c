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

void	get_hy_transformations(t_hyperboloid *hyp, t_point *cen,
		t_vec3 *n, double *results)
{
	t_matrix_4x4	transform;

	transform = inverse(hyp->transform);
	*cen = mat_vec_mult(transform, vec(0, 0, 0, 1));
	*n = norm_vec(mat_vec_mult(transform, vec(0, 1, 0, 0)));
	results[0] = (2.0 / hyp->curr_scale.m[2][2]);
	results[1] = (1.0 / hyp->curr_scale.m[1][1]) * hyp->height;
}

void	get_hy_spaces(t_point *cen, t_vec3 *n, double *results, int *spaces)
{
	spaces[0] = 13 - count_chars(cen->x) - count_chars(cen->y)
		- count_chars(cen->z);
	spaces[1] = 13 - count_chars(n->x) - count_chars(n->y)
		- count_chars(n->z);
	spaces[2] = 7 - count_chars(results[0]);
	spaces[3] = 9 - count_chars(results[1]);
}

char	*build_hy_line(t_hyperboloid *hyp)
{
	static char	line[200];
	t_point		cen;
	t_vec3		n;
	double		results[2];
	int			spaces[4];

	get_hy_transformations(hyp, &cen, &n, results);
	get_hy_spaces(&cen, &n, results, spaces);
	snprintf(line, sizeof(line),
		"hy          %.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s%.3f%*s%.3f%*s",
		cen.x, cen.y, cen.z, spaces[0], "", n.x, n.y, n.z, spaces[1],
		"", results[0], spaces[2], "", results[1], spaces[3], "");
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
	ft_putstr_fd("#Hyperboloids: Cen_x | Cen_y | Cen_z      ", fd);
	ft_putstr_fd("Nrm_x | Nrm_y | Nrm_z      rad   Waist   ", fd);
	ft_putstr_fd("Height       R | G | B\n", fd);
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
