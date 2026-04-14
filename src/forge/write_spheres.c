#include "minirt.h"

void	write_bits(uint8_t *buf, int *bit_pos, uint32_t value, int bits)
{
	int	byte_i;
	int	bit_i;

	while (bits--)
	{
		byte_i = (*bit_pos) / 8;
		bit_i = 7 - ((*bit_pos) % 8);
		if ((value >> bits) & 1U)
			buf[byte_i] |= (1U << bit_i);
		(*bit_pos)++;
	}
}



/*
** 40 bytes -> 54 chars when written as base64 without '=' padding
*/
void	base64_no_pad_40(const uint8_t in[40], char out[55])
{
	static const char	tab[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int					i;
	int					j;
	uint32_t			n;

	i = 0;
	j = 0;
	while (i + 2 < 40)
	{
		n = ((uint32_t)in[i] << 16) | ((uint32_t)in[i + 1] << 8) | in[i + 2];
		out[j++] = tab[(n >> 18) & 0x3F];
		out[j++] = tab[(n >> 12) & 0x3F];
		out[j++] = tab[(n >> 6) & 0x3F];
		out[j++] = tab[n & 0x3F];
		i += 3;
	}
	if (i < 40)
	{
		n = ((uint32_t)in[i] << 16);
		out[j++] = tab[(n >> 18) & 0x3F];
		out[j++] = tab[(n >> 12) & 0x3F];
	}
	out[j] = '\0';
}

static void	pack_sp_transform(t_sphere *sphere, uint8_t out[40])
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
		scaled = lround(sphere->curr_rottran.mat[rot_idx[i]] * 1000.0);
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
		scaled = lround(sphere->curr_scale.mat[sc_idx[i]] * 1000.0);
		if (scaled < -1000000)
			scaled = -1000000;
		if (scaled > 1000000)
			scaled = 1000000;
		packed = (uint32_t)(scaled + 1000000);
		write_bits(out, &bit_pos, packed, 21);
		i++;
	}
}


static void	append_sp_transform(t_sphere *sphere, char *line)
{
	uint8_t	buf[40];
	char	token[55];

	pack_sp_transform(sphere, buf);
	base64_no_pad_40(buf, token);
	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line),
		" %s", token);
}

int	count_chars(double n)
{
	int	count;
	int	num;

	count = 1;
	if (n < 0)
	{
		count++;
		n = -n;
	}
	num = n;
	while (num / 10)
	{
		count++;
		num /= 10;
	}
	return (count);
}

static void	append_sp_material(t_sphere *sphere, char *line, int spaces)
{
	int	a;
	int	d;
	int	s;
	int	h;
	int	r;
	int	t;
	int	i;
	int	b;

	a = (int)lround(sphere->mat.amb * 100.0);
	d = (int)lround(sphere->mat.diff * 100.0);
	s = (int)lround(sphere->mat.spec * 100.0);
	h = (int)lround(sphere->mat.shine);
	r = (int)lround(sphere->mat.ref * 100.0);
	t = (int)lround(sphere->mat.transp * 100.0);
	i = (int)lround(sphere->mat.refract * 100.0);
	b = (int)lround(sphere->bump_level);

	snprintf(line + ft_strlen(line),
		500 - ft_strlen(line),
		"%*s%-4d%-4d%-4d%-5d%-4d%-3d%-5d%-4d%-3d%-2d%-2d%-2d%-4d",
		spaces, "",
		a, d, s, h, r, t, i, b,
		sphere->shadow,
		sphere->bump,
		sphere->option,
		sphere->w_frost,
		sphere->is_box);

	append_sp_transform(sphere, line);
}

void	add_sp_color(t_sphere *sphere, char *line, int spaces2)
{
	int	r;
	int	g;
	int	b;
	int	spaces_3;

	r = ft_round(sphere->color.r * 255);
	g = ft_round(sphere->color.g * 255);
	b = ft_round(sphere->color.b * 255);
	snprintf(line + ft_strlen(line), \
		500 - ft_strlen(line), "%*s%d,%d,%d", \
		spaces2, "", r, g, b);
	spaces_3 = 14 - count_chars(r) - count_chars(g) - count_chars(b);
	if (sphere->texture)
		snprintf(line + ft_strlen(line), \
			500 - ft_strlen(line), "%*s%s", \
			spaces_3, "", (sphere->texture->i_name) + 9);
	if (sphere->texture)
		append_sp_material(sphere, line, 23 - strlen((sphere->texture->i_name) + 9));
	else
		append_sp_material(sphere, line, 23);
	snprintf(line + ft_strlen(line), 500 - ft_strlen(line), "\n");
}

char	*build_sp_line(t_sphere *sphere)
{
	static char		line[500];
	t_point			cen;
	double			d;
	int				spaces;
	int				spaces2;
	t_vec3			n;

	cen = mat_vec_mult(sphere->i_transform, vec(0, 0, 0, 1));
	n = norm_vec(mat_vec_mult(sphere->i_transform, vec(0, 1, 0, 0)));
	check_tolerance(&n);

	d = (2.0 / sphere->curr_scale.mat[10]);
	spaces = 13 - count_chars(cen.x) - count_chars(cen.y) - count_chars(cen.z);
	spaces2 = 20 - count_chars(d);
	snprintf(line, sizeof(line), \
	"sp          %.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s%.3f", \
	cen.x, cen.y, cen.z, spaces, "", n.x, n.y, n.z, 13 - count_chars(n.x) \
	- count_chars(n.y) - count_chars(n.z), "", d);
	add_sp_color(sphere, line, spaces2);
	return (line);
}

//go through sphere list adding each sphere to the file.

void	write_spheres(t_sphere *spheres, int fd)
{
	t_sphere		*curr_sp;
	char			*line;

	if (spheres == NULL)
		return ;
	ft_putstr_fd("#Spheres:   Cen_x | Cen_y | Cen_z      "
		"Nrm_x | Nrm_y | Nrm_z      "
		"Diameter      		  R | G | B		  Texture              	 A   D   S   H    R   T  I    B   Q  P O F +    Xf\n", fd);
	curr_sp = spheres;
	while (true)
	{
		line = build_sp_line(curr_sp);
		write(fd, line, ft_strlen(line));
		curr_sp = curr_sp->next;
		if (curr_sp == spheres)
			break ;
	}
	write(fd, "\n\n", 2);
}
