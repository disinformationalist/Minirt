#include "minirt.h"

char	*build_tx_line(t_tx *tx)
{
	static char		line[200];
	int				spaces;
	size_t			dir_len;

	dir_len = ft_strlen("textures/");
	spaces = 36 - ft_strlen(tx->i_name);
	if (tx->m_name)
		snprintf(line, sizeof(line), "tx          %s%*s%s\n", \
		tx->i_name + dir_len, spaces, "", tx->m_name + dir_len);
	else
		snprintf(line, sizeof(line), "tx          %s\n", \
		tx->i_name + dir_len);
	return (line);
}

//go through tx list adding each tx and bumpmap to the file.

void	write_textures(t_tx *textures, int fd)
{
	t_tx		*curr_tx;
	char		*line;

	if (textures == NULL)
		return ;
	ft_putstr_fd("#Textures:  Image                      Height Map\n", fd);
	curr_tx = textures;
	while (true)
	{
		line = build_tx_line(curr_tx);
		write(fd, line, ft_strlen(line));
		curr_tx = curr_tx->next;
		if (curr_tx == textures)
			break ;
	}
	write(fd, "\n\n", 2);
}
