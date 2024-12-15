#include "minirt.h"

//check valid here using subdir name

static inline bool	check_tx_access(t_tx *textures)
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
			"image must be in textures directory with read permission\n", RED);
			return (1);
		}
		if (curr->m_name && (access(curr->m_name, F_OK) \
			|| access(curr->m_name, R_OK)))
		{
			ft_putstr_color_fd(2, "Error\n Invalid texture param\n Texture " \
			"image must be in textures directory with read permission\n", RED);
			return (1);
		}
		curr = curr->next;
		if (curr == textures)
			break ;
	}
	return (0);
}

//add directory name to all maps and textures for importing.

static inline bool	append_dir(t_tx *textures, char *temp)
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
			break ;
	}
	if (check_tx_access(textures))
		return (1);
	return (0);
}

static inline bool	fill_tx(void *mlx_con, t_tx *curr)
{
	curr->image = import_png(mlx_con, curr->i_name, \
	&curr->i_width, &curr->i_height);
	if (!curr->image)
		return (1);
	if (curr->m_name)
		curr->bump_map = import_png(mlx_con, curr->m_name, \
		&curr->m_width, &curr->m_height);
	else
	{
		curr->bump_map = build_lumin_map(mlx_con, curr->image, \
		curr->i_width, curr->i_height);
		curr->m_width = curr->i_width;
		curr->m_height = curr->i_height;
	}
	if (!curr->bump_map)
		return (1);
	if ((curr->m_width != curr->i_width) || curr->m_height != curr->i_height)
	{
		ft_putstr_color_fd(2, "Error\n Invalid texture params\n Texture " \
			"image dimensions must match height map dimensions\n", RED);
			return (1);
		return (1);
	}
	curr->img_iasp = (double)curr->i_height / (double)curr->i_width;
	return (0);
}

//build texture/bump map lists

int	import_textures(void *mlx_con, t_tx *textures)
{
	t_tx	*curr;

	if (textures == NULL)
		return (0);
	if (append_dir(textures, NULL))
		return (1);
	curr = textures;
	while (true)
	{
		if (fill_tx(mlx_con, curr))
			return (1);
		curr = curr->next;
		if (curr == textures)
			break ;
	}
	return (0);
}
