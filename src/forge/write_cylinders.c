#include "minirt.h"


static void	pack_cy_transform(t_cylinder *cyl, uint8_t out[40])
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
		scaled = lround(cyl->curr_rottran.mat[rot_idx[i]] * 1000.0);
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
		scaled = lround(cyl->curr_scale.mat[sc_idx[i]] * 1000.0);
		if (scaled < -1000000)
			scaled = -1000000;
		if (scaled > 1000000)
			scaled = 1000000;
		packed = (uint32_t)(scaled + 1000000);
		write_bits(out, &bit_pos, packed, 21);
		i++;
	}
}

static void	append_cy_transform(t_cylinder *cyl, char *line)
{
	uint8_t	buf[40];
	char	token[55];

	pack_cy_transform(cyl, buf);
	base64_no_pad_40(buf, token);
	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line),
		" %s", token);
}

static void	append_cy_material(t_cylinder *cyl, char *line, int spaces)
{
	int	a;
	int	d;
	int	s;
	int	h;
	int	r;
	int	t;
	int	i;
	int	b;

	a = (int)lround(cyl->mat.amb * 100.0);
	d = (int)lround(cyl->mat.diff * 100.0);
	s = (int)lround(cyl->mat.spec * 100.0);
	h = (int)lround(cyl->mat.shine);
	r = (int)lround(cyl->mat.ref * 100.0);
	t = (int)lround(cyl->mat.transp * 100.0);
	i = (int)lround(cyl->mat.refract * 100.0);
	b = (int)lround(cyl->bump_level);

	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line),
		"%*s%-4d%-4d%-4d%-5d%-4d%-3d%-5d%-4d%-3d%-2d%-2d%-2d%-4d",
		spaces, "",
		a, d, s, h, r, t, i, b,
		cyl->shadow,
		cyl->bump,
		cyl->option,
		cyl->w_frost,
		cyl->caps);
	append_cy_transform(cyl, line);
}

void	add_cy_color(t_cylinder *cyl, char *line)
{
	int	r;
	int	g;
	int	b;
	int	spaces_3;

	r = ft_round(cyl->color.r * 255);
	g = ft_round(cyl->color.g * 255);
	b = ft_round(cyl->color.b * 255);
	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line), "%d,%d,%d", r, g, b);
	spaces_3 = 14 - count_chars(r) - count_chars(g) - count_chars(b);
	if (cyl->texture)
		snprintf(line + ft_strlen(line),
			500 - ft_strlen(line), "%*s%s",
			spaces_3, "", (cyl->texture->i_name) + 9);
	if (cyl->texture)
		append_cy_material(cyl, line, 23 - ft_strlen((cyl->texture->i_name) + 9));
	else
		append_cy_material(cyl, line, 23);
	snprintf(line + ft_strlen(line), 500 - ft_strlen(line), "\n");
}

char	*build_cy_line(t_cylinder *cyl)
{
	static char		line[500];
	t_point			cen;
	t_vec3			n;
	double			d;
	double			h;

	cen = mat_vec_mult(cyl->i_transform, vec(0, 0, 0, 1));
	check_tolerance(&cen);
	n = norm_vec(mat_vec_mult(cyl->i_transform, vec(0, 1, 0, 0)));
	check_tolerance(&n);
	d = (2.0 / cyl->curr_scale.mat[10]);
	h = (2.0 / cyl->curr_scale.mat[5]);
	snprintf(line, sizeof(line),
		"cy          %.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s%.3f%*s%.3f%*s",
		cen.x, cen.y, cen.z, 13 - count_chars(cen.x) - count_chars(cen.y)
		- count_chars(cen.z), "", n.x, n.y, n.z, 13 - count_chars(n.x)
		- count_chars(n.y) - count_chars(n.z), "", d, 7 - count_chars(d),
		"", h, 9 - count_chars(h), "");
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
	ft_putstr_fd("#Cylinders: Cen_x | Cen_y | Cen_z      "
		"Nrm_x | Nrm_y | Nrm_z      Diameter   Height       "
		"R | G | B       Texture                "
		"A   D   S   H    R   T  I    B   Q  P O F C    Xf\n", fd);
	curr_cy = cylinders;
	while (true)
	{
		line = build_cy_line(curr_cy);
		write(fd, line, ft_strlen(line));
		curr_cy = curr_cy->next;
		if (curr_cy == cylinders)
			break ;
	}
	write(fd, "\n\n", 2);
}
