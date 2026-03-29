#include "minirt.h"

static void	pack_cu_transform(t_cube *cu, uint8_t out[40])
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
		scaled = lround(cu->curr_rottran.mat[rot_idx[i]] * 1000.0);
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
		scaled = lround(cu->curr_scale.mat[sc_idx[i]] * 1000.0);
		if (scaled < -1000000)
			scaled = -1000000;
		if (scaled > 1000000)
			scaled = 1000000;
		packed = (uint32_t)(scaled + 1000000);
		write_bits(out, &bit_pos, packed, 21);
		i++;
	}
}

static void	append_cu_transform(t_cube *cu, char *line)
{
	uint8_t	buf[40];
	char	token[55];

	pack_cu_transform(cu, buf);
	base64_no_pad_40(buf, token);
	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line),
		" %s", token);
}

static void	append_cu_material(t_cube *cu, char *line, int spaces)
{
	int	a;
	int	d;
	int	s;
	int	h;
	int	r;
	int	t;
	int	i;
	int	b;

	a = (int)lround(cu->mat.amb * 100.0);
	d = (int)lround(cu->mat.diff * 100.0);
	s = (int)lround(cu->mat.spec * 100.0);
	h = (int)lround(cu->mat.shine);
	r = (int)lround(cu->mat.ref * 100.0);
	t = (int)lround(cu->mat.transp * 100.0);
	i = (int)lround(cu->mat.refract * 100.0);
	b = (int)lround(cu->bump_level);

	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line),
		"%*s%-4d%-4d%-4d%-5d%-4d%-3d%-5d%-4d%-3d%-2d%-2d%-6d",
		spaces, "",
		a, d, s, h, r, t, i, b,
		cu->shadow,
		cu->bump,
		cu->option,
		cu->w_frost);

	append_cu_transform(cu, line);
}

void	add_cu_color(t_cube *cu, char *line)
{
	int	r;
	int	g;
	int	b;
	int	spaces_3;

	r = cu->color.r;
	g = cu->color.g;
	b = cu->color.b;
	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line), "%d,%d,%d", r, g, b);
	spaces_3 = 14 - count_chars(r) - count_chars(g) - count_chars(b);
	if (cu->texture)
		snprintf(line + ft_strlen(line),
			500 - ft_strlen(line), "%*s%s",
			spaces_3, "", (cu->texture->i_name) + 9);
	if (cu->texture)
		append_cu_material(cu, line, 23 - ft_strlen((cu->texture->i_name) + 9));
	else
		append_cu_material(cu, line, 23);
	snprintf(line + ft_strlen(line), 500 - ft_strlen(line), "\n");
}

char	*build_cu_line(t_cube *cu)
{
	static char		line[500];
	t_point			cen;
	t_vec3			n;
	double			w;
	double			h;
	double			d;

	cen = mat_vec_mult(cu->i_transform, vec(0, 0, 0, 1));
	check_tolerance(&cen);
	n = norm_vec(mat_vec_mult(cu->i_transform, vec(0, 1, 0, 0)));
	check_tolerance(&n);
	w = (2.0 / cu->curr_scale.mat[0]);
	h = (2.0 / cu->curr_scale.mat[5]);
	d = (2.0 / cu->curr_scale.mat[10]);
	snprintf(line, sizeof(line),
		"cu          %.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s%.3f%*s%.3f%*s%.3f%*s",
		cen.x, cen.y, cen.z, 13 - count_chars(cen.x) - count_chars(cen.y)
		- count_chars(cen.z), "", n.x, n.y, n.z, 13 - count_chars(n.x)
		- count_chars(n.y) - count_chars(n.z), "", w, 4 - count_chars(w),
		"", h, 4 - count_chars(h), "", d, 4 - count_chars(d), "");
	add_cu_color(cu, line);
	return (line);
}

//go through cube list adding each cube to the file.

void	write_cubes(t_cube *cubes, int fd)
{
	t_cube		*curr_cu;
	char		*line;

	if (cubes == NULL)
		return ;
	ft_putstr_fd("#Cubes:     Cen_x | Cen_y | Cen_z      "
		"Nrm_x | Nrm_y | Nrm_z      W_(x)   H_(y)   D_(z)   "
		"R | G | B       Texture                "
		"A   D   S   H    R   T  I    B   Q  P O F      Xf\n", fd);
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
