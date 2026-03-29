#include "minirt.h"

static inline void	set_sp_box(t_sphere *new)
{
	t_mat4	transform;

	new->center = vec(0.0, 0.0, 0.0, 1.0);
	new->radius = 500;
	new->color = color(124, 162, 205);
	transform = inv_scaling(new->radius, \
			new->radius, new->radius);
	new->curr_scale = transform;
	new->t_transform = transpose(transform);
	new->i_transform = inverse(transform);
	new->curr_rottran = identity();
	new->transform = transform;
	new->mat = get_mat(DEFAULT);
	new->id = 1;
	new->shadow = false;
	new->bump = false;
	new->next = new;
	new->prev = new;
	new->option = 0;
	new->w_frost = false;
	new->is_box = true;
	new->pattern = uv_checker(20, 10, color(40, 40, 40), color(255, 255, 255));//adjust
	new->bump_level = DEFAULT_BUMP;
	new->fuzz_lev = new->bump_level / 1000;
	new->rots = vec(0, 0, 0, 0);
}

static inline t_sphere	*create_sp_box(void)
{
	t_sphere	*new;

	new = (t_sphere *)malloc(sizeof(t_sphere));
	if (!new)
		return (NULL);
	set_sp_box(new);
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_sp_box(t_trace *trace, t_sphere **start)
{
	t_sphere	*new;
	t_sphere	*last;
	bool		temp;

	if (!trace->spheres)
		temp = false;
	else
		temp = true;
	//printf("temp: %d\n", temp);
	
	//trace->on->object = trace->spheres;
	//trace->on->type = SPHERE;
	trace->total_ints += 2;
	new = create_sp_box();
	if (!new)
		return (true);
	if (*start == NULL)
	{
		*start = new;
		new->id = 1;
		if (!temp)
			trace->curr_sp = trace->spheres;
		new->texture = trace->textures;
		return (false);
	}
	last = (*start)->prev;
	new->id = last->id + 1;
	new->next = *start;
	(*start)->prev = new;
	new->prev = last;
	last->next = new;
	(*start) = new;
	if (!temp)
		trace->curr_sp = trace->spheres;
	new->texture = trace->textures;
	return (false);
}

static int	base64_val(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (c - 'A');
	if (c >= 'a' && c <= 'z')
		return (c - 'a' + 26);
	if (c >= '0' && c <= '9')
		return (c - '0' + 52);
	if (c == '+')
		return (62);
	if (c == '/')
		return (63);
	return (-1);
}


int	decode_base64_no_pad_40(const char *in, uint8_t out[40])
{
	int			i;
	int			j;
	int			v0;
	int			v1;
	int			v2;
	int			v3;
	uint32_t	n;

	ft_bzero(out, 40);
	i = 0;
	j = 0;
	while (in[i] && in[i + 1] && in[i + 2] && in[i + 3] && j + 2 < 40)
	{
		v0 = base64_val(in[i]);
		v1 = base64_val(in[i + 1]);
		v2 = base64_val(in[i + 2]);
		v3 = base64_val(in[i + 3]);
		if (v0 < 0 || v1 < 0 || v2 < 0 || v3 < 0)
			return (0);
		n = ((uint32_t)v0 << 18) | ((uint32_t)v1 << 12)
			| ((uint32_t)v2 << 6) | (uint32_t)v3;
		out[j++] = (n >> 16) & 0xFF;
		out[j++] = (n >> 8) & 0xFF;
		out[j++] = n & 0xFF;
		i += 4;
	}
	if (in[i] && in[i + 1] && j < 40)
	{
		v0 = base64_val(in[i]);
		v1 = base64_val(in[i + 1]);
		if (v0 < 0 || v1 < 0)
			return (0);
		n = ((uint32_t)v0 << 18) | ((uint32_t)v1 << 12);
		out[j++] = (n >> 16) & 0xFF;
	}
	return (j == 40);
}

uint32_t	read_bits(const uint8_t *buf, int *bit_pos, int bits)
{
	uint32_t	value;
	int			byte_i;
	int			bit_i;

	value = 0;
	while (bits--)
	{
		byte_i = (*bit_pos) / 8;
		bit_i = 7 - ((*bit_pos) % 8);
		value = (value << 1) | ((buf[byte_i] >> bit_i) & 1U);
		(*bit_pos)++;
	}
	return (value);
}

static void	set_sp_transform(t_sphere *new, char *token)
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

void	set_sp_material(t_sphere *new, char **line)
{
	new->mat.amb = ft_atoi(line[6]) / 100.0;
	new->mat.diff = ft_atoi(line[7]) / 100.0;
	new->mat.spec = ft_atoi(line[8]) / 100.0;
	new->mat.shine = ft_atoi(line[9]);
	new->mat.ref = ft_atoi(line[10]) / 100.0;
	new->mat.transp = ft_atoi(line[11]) / 100.0;
	new->mat.refract = ft_atoi(line[12]) / 100.0;
	new->bump_level = ft_atoi(line[13]);
	new->shadow = ft_atoi(line[14]);
	new->bump = ft_atoi(line[15]);
	new->option = ft_atoi(line[16]);
	new->w_frost = ft_atoi(line[17]);
	if (line[18])
	{
		set_sp_transform(new, line[18]);
		new->params = 1;
	}
	else
		new->params = 0;

}

void	set_sp_vals(t_sphere *new, char **line)
{
	char	*diam_str;
	char	*col_str;

	diam_str = line[3], col_str = line[4];
	new->norm = norm_vec(get_coordinates(line[2], 0.0));
	new->center = get_coordinates(line[1], 1.0);
	new->radius = get_double(&diam_str) / 2.0;
		if (line[5])
		new->i_name = ft_strdup(line[5]);
	else 
		new->i_name = NULL;
	if (line[5] && line[6])
		set_sp_material(new, line);
	else
	{
		new->mat = get_mat(ENAMEL);
		new->shadow = true;
		new->bump = false;
		new->bump_level = DEFAULT_BUMP;
		new->option = 0;
		new->w_frost = false;
	}
	//new->mat = get_mat(DEFAULT);
	//new->mat = get_mat(MIRROR);
	//new->mat = get_mat(GLASS);
	//new->mat = get_mat(METAL);
	//new->color = color(220, 155, 43);//pure gold used in spflake
	//new->color = color(220, 130, 35); //gold brown used in spflake
	//new->color = color(255, 255, 255);
	//new->mat.spec = 0;
	//new->shadow = false;
	new->color = get_color(col_str, 1.0);
	new->is_box = false;
	new->fuzz_lev = new->bump_level / 1000;
	new->texture = NULL;
}

t_sphere	*create_sphere(char **line)
{
	t_sphere	*new;

	new = (t_sphere *)malloc(sizeof(t_sphere));
	if (!new)
		return (NULL);
	set_sp_vals(new, line);
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_sp(t_sphere **start, char **line)
{
	t_sphere	*new;
	t_sphere	*last;

	new = create_sphere(line);
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
