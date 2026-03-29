#include "minirt.h"

static void	pack_hy_transform(t_hyperboloid *hyp, uint8_t out[40])
{
	static const int	rot_idx[12] = {0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14};
	static const int	sc_idx[3] = {0, 5, 10};
	int					i;
	int					bit_pos;
	long				scaled;
	uint32_t			packed;

	ft_bzero(out, 40);
	bit_pos = 0;
	i = 0;
	while (i < 12)
	{
		scaled = lround(hyp->curr_rottran.mat[rot_idx[i]] * 1000.0);
		if (scaled < -1000000)
			scaled = -1000000;
		if (scaled > 1000000)
			scaled = 1000000;
		packed = (uint32_t)(scaled + 1000000);
		write_bits(out, &bit_pos, packed, 21);
		i++;
	}
	i = 0;
	while (i < 3)
	{
		scaled = lround(hyp->curr_scale.mat[sc_idx[i]] * 1000.0);
		if (scaled < -1000000)
			scaled = -1000000;
		if (scaled > 1000000)
			scaled = 1000000;
		packed = (uint32_t)(scaled + 1000000);
		write_bits(out, &bit_pos, packed, 21);
		i++;
	}
}

static void	append_hy_transform(t_hyperboloid *hyp, char *line)
{
	uint8_t	buf[40];
	char	token[55];

	pack_hy_transform(hyp, buf);
	base64_no_pad_40(buf, token);
	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line),
		" %s", token);
}

static void	append_hy_material(t_hyperboloid *hyp, char *line, int spaces)
{
	int	a;
	int	d;
	int	s;
	int	h;
	int	r;
	int	t;
	int	i;
	int	b;

	a = (int)lround(hyp->mat.amb * 100.0);
	d = (int)lround(hyp->mat.diff * 100.0);
	s = (int)lround(hyp->mat.spec * 100.0);
	h = (int)lround(hyp->mat.shine);
	r = (int)lround(hyp->mat.ref * 100.0);
	t = (int)lround(hyp->mat.transp * 100.0);
	i = (int)lround(hyp->mat.refract * 100.0);
	b = (int)lround(hyp->bump_level);

	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line),
		"%*s%-4d%-4d%-4d%-5d%-4d%-3d%-5d%-4d%-3d%-2d%-2d%-2d%-2d%-2d",
		spaces, "",
		a, d, s, h, r, t, i, b,
		hyp->shadow,
		hyp->bump,
		hyp->option,
		hyp->w_frost,
		hyp->caps,
		hyp->single);
	append_hy_transform(hyp, line);
}

void	add_hy_color(t_hyperboloid *hyp, char *line)
{
	int	r;
	int	g;
	int	b;
	int	spaces_3;

	r = hyp->color.r;
	g = hyp->color.g;
	b = hyp->color.b;
	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line), "%d,%d,%d", r, g, b);
	spaces_3 = 14 - count_chars(r) - count_chars(g) - count_chars(b);
	if (hyp->texture)
		snprintf(line + ft_strlen(line),
			500 - ft_strlen(line), "%*s%s",
			spaces_3, "", (hyp->texture->i_name) + 9);
	if (hyp->texture)
		append_hy_material(hyp, line, 23 - ft_strlen((hyp->texture->i_name) + 9));
	else
		append_hy_material(hyp, line, 23);
	snprintf(line + ft_strlen(line), 500 - ft_strlen(line), "\n");
}

char	*build_hy_line(t_hyperboloid *hyp)
{
	static char	line[500];
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
	snprintf(line, sizeof(line),
		"hy          %.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s%.3f%*s%.3f%*s%.3f%*s",
		cen.x, cen.y, cen.z, 13 - count_chars(cen.x) - count_chars(cen.y)
		- count_chars(cen.z), "", n.x, n.y, n.z, 13 - count_chars(n.x)
		- count_chars(n.y) - count_chars(n.z), "", r, 4 - count_chars(r),
		"", waist, 4 - count_chars(waist), "", h, 4 - count_chars(h), "");
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
		"Nrm_x | Nrm_y | Nrm_z      Rad     Waist   Height  "
		"R | G | B       Texture                "
		"A   D   S   H    R   T  I    B   Q  P O F C S  Xf\n", fd);
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
