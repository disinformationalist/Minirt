#include "minirt.h"

static void	pack_pl_transform(t_plane *plane, uint8_t out[40])
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
		scaled = lround(plane->curr_rottran.mat[rot_idx[i]] * 1000.0);
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
		scaled = lround(plane->curr_scale.mat[sc_idx[i]] * 1000.0);
		if (scaled < -1000000)
			scaled = -1000000;
		if (scaled > 1000000)
			scaled = 1000000;
		packed = (uint32_t)(scaled + 1000000);
		write_bits(out, &bit_pos, packed, 21);
		i++;
	}
}

static void	append_pl_transform(t_plane *plane, char *line)
{
	uint8_t	buf[40];
	char	token[55];

	pack_pl_transform(plane, buf);
	base64_no_pad_40(buf, token);
	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line),
		" %s", token);
}

static void	append_pl_material(t_plane *plane, char *line, int spaces)
{
	int	a;
	int	d;
	int	s;
	int	h;
	int	r;
	int	t;
	int	i;
	int	b;

	a = (int)lround(plane->mat.amb * 100.0);
	d = (int)lround(plane->mat.diff * 100.0);
	s = (int)lround(plane->mat.spec * 100.0);
	h = (int)lround(plane->mat.shine);
	r = (int)lround(plane->mat.ref * 100.0);
	t = (int)lround(plane->mat.transp * 100.0);
	i = (int)lround(plane->mat.refract * 100.0);
	b = (int)lround(plane->bump_level);

	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line),
		"%*s%-4d%-4d%-4d%-5d%-4d%-3d%-5d%-4d%-3d%-2d%-2d%-2d%-4d",
		spaces, "",
		a, d, s, h, r, t, i, b,
		plane->shadow,
		plane->bump,
		plane->option,
		plane->w_frost,
		plane->sine);
	append_pl_transform(plane, line);
}

void	add_pl_color(t_plane *plane, char *line)
{
	int	r;
	int	g;
	int	b;
	int	spaces_3;

	r = plane->color.r;
	g = plane->color.g;
	b = plane->color.b;
	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line), "%d,%d,%d", r, g, b);
	spaces_3 = 14 - count_chars(r) - count_chars(g) - count_chars(b);
	if (plane->texture)
		snprintf(line + ft_strlen(line),
			500 - ft_strlen(line), "%*s%s",
			spaces_3, "", (plane->texture->i_name) + 9);
	if (plane->texture)
		append_pl_material(plane, line, 23 - ft_strlen((plane->texture->i_name) + 9));
	else
		append_pl_material(plane, line, 23);
	snprintf(line + ft_strlen(line), 500 - ft_strlen(line), "\n");
}

char	*build_pl_line(t_plane *plane)
{
	static char		line[500];
	t_point			p;
	t_vec3			n;
	int				spaces;
	t_mat4			transform;

	transform = inverse(plane->transform);
	p = mat_vec_mult(transform, vec(0, 0, 0, 1));
	n = norm_vec(mat_vec_mult(transform, vec(0, 1, 0, 0)));
	check_tolerance(&n);
	spaces = 13 - count_chars(p.x) - count_chars(p.y) - count_chars(p.z);
	snprintf(line, sizeof(line),
		"pl          %.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s",
		p.x, p.y, p.z, spaces, "", n.x, n.y, n.z,
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
		"Nrm_x | Nrm_y | Nrm_z                              "
		"R | G | B       Texture                "
		"A   D   S   H    R   T  I    B   Q  P O F W    Xf\n", fd);
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
