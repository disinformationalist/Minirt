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

bool	append_dir(t_tx *textures)
{	
	t_tx	*curr;
	char	*temp;

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
	if (append_dir(textures))
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
