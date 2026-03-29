#include "minirt.h"

static void	set_cy_transform(t_cylinder *new, char *token)
{
	static const int	rot_idx[12] = {0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14};
	static const int	sc_idx[3] = {0, 5, 10};
	uint8_t				buf[40];
	int					bit_pos;
	int					i;
	int32_t				scaled;

	if (!decode_base64_no_pad_40(token, buf))
		return ;
	ft_bzero(new->curr_rottran.mat, sizeof(new->curr_rottran.mat));
	ft_bzero(new->curr_scale.mat, sizeof(new->curr_scale.mat));
	bit_pos = 0;
	i = 0;
	while (i < 12)
	{
		scaled = (int32_t)read_bits(buf, &bit_pos, 21) - 1000000;
		new->curr_rottran.mat[rot_idx[i]] = scaled / 1000.0;
		i++;
	}
	i = 0;
	while (i < 3)
	{
		scaled = (int32_t)read_bits(buf, &bit_pos, 21) - 1000000;
		new->curr_scale.mat[sc_idx[i]] = scaled / 1000.0;
		i++;
	}
	new->curr_rottran.mat[3] = 0.0;
	new->curr_rottran.mat[7] = 0.0;
	new->curr_rottran.mat[11] = 0.0;
	new->curr_rottran.mat[15] = 1.0;
	new->curr_scale.mat[1] = 0.0;
	new->curr_scale.mat[2] = 0.0;
	new->curr_scale.mat[3] = 0.0;
	new->curr_scale.mat[4] = 0.0;
	new->curr_scale.mat[6] = 0.0;
	new->curr_scale.mat[7] = 0.0;
	new->curr_scale.mat[8] = 0.0;
	new->curr_scale.mat[9] = 0.0;
	new->curr_scale.mat[11] = 0.0;
	new->curr_scale.mat[12] = 0.0;
	new->curr_scale.mat[13] = 0.0;
	new->curr_scale.mat[14] = 0.0;
	new->curr_scale.mat[15] = 1.0;
}

void	set_cy_material(t_cylinder *new, char **line)
{
	new->mat.amb = ft_atoi(line[7]) / 100.0;
	new->mat.diff = ft_atoi(line[8]) / 100.0;
	new->mat.spec = ft_atoi(line[9]) / 100.0;
	new->mat.shine = ft_atoi(line[10]);
	new->mat.ref = ft_atoi(line[11]) / 100.0;
	new->mat.transp = ft_atoi(line[12]) / 100.0;
	new->mat.refract = ft_atoi(line[13]) / 100.0;
	new->bump_level = ft_atoi(line[14]);
	new->shadow = ft_atoi(line[15]);
	new->bump = ft_atoi(line[16]);
	new->option = ft_atoi(line[17]);
	new->w_frost = ft_atoi(line[18]);
	new->caps  = ft_atoi(line[19]);
	if (line[20])
	{
		set_cy_transform(new, line[20]);
		new->params = 1;
	}
	else
		new->params = 0;
}

void	set_cy_vals(t_cylinder *new, char **line)
{
	char	*diam_str;
	char	*height_str;

	diam_str = line[3];
	height_str = line[4];
	new->center = get_coordinates(line[1], 1.0);
	new->norm = norm_vec(get_coordinates(line[2], 0.0));
	new->radius = get_double(&diam_str) / 2;
	new->height = get_double(&height_str);
	new->color = get_color(line[5], 1.0);
	if (line[6])
		new->i_name = ft_strdup(line[6]);
	else 
		new->i_name = NULL;
	if (line[6] && line[7])
		set_cy_material(new, line);
	else
	{
		new->mat = get_mat(ENAMEL);
		new->shadow = true;
		new->bump = false;
		new->bump_level = DEFAULT_BUMP;
		new->option = 0;
		new->w_frost = false;
		new->caps = true;
	}
	new->texture = NULL;
	new->fuzz_lev = new->bump_level / 1000;
}

t_cylinder	*create_cylinder(char **line)
{
	t_cylinder	*new;

	new = (t_cylinder *)malloc(sizeof(t_cylinder));
	if (!new)
		return (NULL);
	set_cy_vals(new, line);
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_cy(t_cylinder **start, char **line)
{
	t_cylinder	*new;
	t_cylinder	*last;

	new = create_cylinder(line);
	if (!new)
		return (true);
	if (*start == NULL)
	{
		*start = new;
		new->id = 1;
		return (false);
	}
	last = (*start)->prev;
	new->id = last->id + 1;
	new->next = *start;
	(*start)->prev = new;
	new->prev = last;
	last->next = new;
	return (false);
}
