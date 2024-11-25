#include "minirt.h"

uint8_t luminosity(t_norm_color color)
{
	return ((uint8_t)(0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b));
}

t_img 	*create_img(void *mlx_ptr, int width, int height)
{
	t_img *img;
	
	img = (t_img *)malloc(sizeof(t_img));
	if (!img)
		return (NULL);
	if (new_img_init(mlx_ptr, img, width, height) == -1)
	{
		free(img);
		return (NULL);
	}
	return (img);
}

/*
For optimization, build packed gradient map in advance and handle edges here

 static inline unsigned char pixel_gray_get(int x, int y, t_img *img)
{
	unsigned char	gray;
	int 			offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	gray = *(unsigned char *)(img->pixels_ptr + offset);
	return (gray);
}

static inline t_vec3 bump(t_position pos, t_img *img, double strength)
{
	t_vec3 			perturb;
	unsigned char	curr;
	double			dfdx;
	double			dfdy;
	double			norm_factor;
	
	curr = pixel_gray_get(pos.i, pos.j, img);
	dfdx = strength * ((double)(pixel_gray_get((pos.i + 1), pos.j, img) - curr)) / 255.0;
	dfdy = strength * ((double)(pixel_gray_get(pos.i, (pos.j + 1), img) - curr)) / 255.0;
	norm_factor = 1.0 / (sqrt(dfdx * dfdx + dfdy * dfdy + 1.0));
	perturb.x = -dfdx * norm_factor;
	perturb.z = -dfdy * norm_factor;
	perturb.y = 1 - norm_factor; 
	perturb.w = 0;
	return (perturb);
} */
//-------------------------------

//try grad map here.


unsigned int	pack_up(int i, int j, t_img *img)
{
	double			dfdx;
	double			dfdy;
	double			norm_factor;
	uint8_t			gray_val;
	unsigned int	grad_col;

	gray_val = luminosity(pixel_color_get(i, j, img));
	dfdx = 30 * (luminosity(pixel_color_get(i + 1, j, img)) - gray_val);
	dfdy = 30 * (luminosity(pixel_color_get(i, j + 1, img)) - gray_val);
	norm_factor = 1.0 / (sqrt(dfdx * dfdx + dfdy * dfdy + 1));
	dfdx = (dfdx * norm_factor + 1.0) * 127.5;//x
	dfdy = (dfdy * norm_factor + 1.0) * 127.5;//y -> z later
	norm_factor = norm_factor * 255; //z ->y at unpack

	grad_col = ((uint8_t)dfdx << 16 | (uint8_t)dfdy << 8 | (uint8_t)norm_factor);
	return (grad_col);
}

/* t_img *build_lumin_map(void *mlx_con, t_img *img, int width, int height)
{
	t_img			*bump_map;
	unsigned int	grad_col;
	int				i;
	int				j;

	grad_col = 0;
	bump_map = create_img(mlx_con, width, height);
	if (!bump_map)
		return (NULL);
	j = -1;
	while (++j < height - 1)
	{
		i = -1;
		while (++i < width - 1)
		{
			grad_col = pack_up(i, j, img);
			my_pixel_put(i, j, bump_map, grad_col);
		}
		my_pixel_put(i, j, bump_map, grad_col);
	}
	i = -1;
	while (++i < width)
	{
		grad_col = pack_up(i, j, img);
		my_pixel_put(i, j, bump_map, grad_col);
	}
	return (bump_map);
} */

//orig
t_img *build_lumin_map(void *mlx_con, t_img *img, int width, int height)
{
	t_img			*bump_map;
	uint8_t			gray_val;
	unsigned int	gray_col;
	int				i;
	int				j;

	bump_map = create_img(mlx_con, width, height);
	if (!bump_map)
		return (NULL);
	j = -1;
	while (++j < height)
	{
		i = -1;
		while (++i < width)
		{
			gray_val = luminosity(pixel_color_get(i, j, img));
			gray_col = (gray_val << 16 | gray_val << 8 | gray_val);
			my_pixel_put(i, j, bump_map, gray_col);
		}
	}
	return (bump_map);
}

//check valid here using subdir name

bool	check_tx_access(t_tx *textures)
{
	t_tx	*curr;

	if (textures == NULL)
		return (0);
	curr = textures;
	while (true)
	{
		if (access(curr->i_name, F_OK) || access(curr->i_name, R_OK))
		{
			ft_putstr_color_fd(2, "Error\n Invalid texture param\n Texture " \
			"image must exist in textures directory with read permission\n", RED);
			return (1);
		}
		if (curr->m_name && (access(curr->m_name, F_OK) || access(curr->m_name, R_OK)))
		{
			ft_putstr_color_fd(2, "Error\n Invalid texture param\n Texture " \
			"image must exist in textures directory with read permission\n", RED);
			return (1);
		}
		curr = curr->next;
		if (curr == textures)
			break;
	}
	return (0);
}

//add directory name to all maps and textures for importing.

bool	append_dir(t_tx *textures, char *temp)
{	
	t_tx	*curr;

	curr = textures;
	while (true)
	{
		temp = curr->i_name;
		curr->i_name = ft_strjoin("textures/", temp);
		free(temp);
		if (!curr->i_name)
			return (1);
		if (curr->m_name)
		{
			temp = curr->m_name;
			curr->m_name = ft_strjoin("textures/", temp);
			free(temp);
			if (!curr->m_name)
				return (1);
		}
		curr = curr->next;
		if (curr == textures)
			break;
	}
	if (check_tx_access(textures))
		return (1);
	return (0);
}

//build texture/bump map list here, if no bump map provided, create a luminosity based

int	import_textures(void *mlx_con, t_tx *textures)
{
	t_tx *curr;

	if (textures == NULL)
		return (0);
	if (append_dir(textures, NULL))
		return (1);
	curr = textures;
	while (true)
	{
		curr->image = import_png(mlx_con, curr->i_name, &curr->i_width, &curr->i_height);
		if (!curr->image)
			return (1);
		if (curr->m_name)
			curr->bump_map = import_png(mlx_con, curr->m_name, &curr->m_width, &curr->m_height);
		else
			curr->bump_map = build_lumin_map(mlx_con, curr->image, curr->i_width, curr->i_height);
		if (!curr->bump_map)
			return (1);
		curr->img_iasp = (double)curr->i_height / (double)curr->i_width;
		curr = curr->next;
		if (curr == textures)
			break;
	}
	return (0);
}
