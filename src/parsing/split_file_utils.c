#include "minirt.h"

void	err_free(char ***split_file, char *msg)
{
	perror(msg);
	free(split_file);
}

int	count_lines(int fd)
{
	int		count;
	char	*line;

	count = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (strlen_no_whitespace(line) > 0 && line[0] != '#')
			count++;
		free(line);
		line = get_next_line(fd);
	}
	return (count);
}

char	*remove_new_line(char *line)
{
	char	*newline_pos;

	newline_pos = strchr(line, '\n');
	if (newline_pos)
		*newline_pos = '\0';
	return (line);
}

int	strlen_no_whitespace(char *s)
{
	int	i;
	int	len;

	len = 0;
	i = -1;
	while (s[++i])
	{
		if (s[i] != 32 && (s[i] < 9 || s[i] > 13))
			len++;
	}
	return (len);
}

void	free_3d_array_i(char ***split_file, int k)
{
	while (--k >= 0)
		ft_free_matrix_i(split_file[k], ft_matrix_len(split_file[k]));
	free(split_file);
	split_file = NULL;
}
