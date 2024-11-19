#include "minirt.h"

char	*skip_empty(char *line, int fd)
{
	while (line && (strlen_no_whitespace(line) == 0 || line[0] == '#'))
	{
		free (line);
		line = get_next_line(fd);
	}
	return (line);
}

void	free_and_close( char ***split_file, char *msg, int fd, int i)
{
	perror(msg);
	free_3d_array_i(split_file, i);
	close(fd);
}

int	fill_array_loop(char **line, char ***split_file, int fd, int i)
{
	*line = remove_new_line(*line);
	*line = ft_strtrim(*line, " \t");
	if (!(*line))
		return (free_and_close(split_file,
				"Error\n ft_strtrim failure\n", fd, i), 1);
	split_file[i] = split_by_whitespace(*line);
	free(*line);
	if (!split_file[i])
		return (free_and_close(split_file,
				"Error\n ft_split failure\n", fd, i), 1);
	*line = get_next_line(fd);
	*line = skip_empty(*line, fd);
	return (0);
}

char	***fill_3darray(char ***split_file, char *file, int num_lines)
{
	int		fd;
	int		i;
	char	*line;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (err_free(split_file, "Error\n Error opening file\n"), NULL);
	i = -1;
	line = get_next_line(fd);
	line = skip_empty(line, fd);
	if (!line)
		return (close(fd), err_free(split_file, "Error\n gnl failure\n"), NULL);
	while (++i < num_lines)
	{
		if (fill_array_loop(&line, split_file, fd, i))
			return (NULL);
		if (!line && i < num_lines - 1)
			return (free_and_close(split_file,
					"Error\n gnl failure\n", fd, i), NULL);
	}
	split_file[i] = NULL;
	close(fd);
	return (split_file);
}

//split file into malloced 3d array

char	***split_file(char *file)
{
	int		fd;
	int		num_lines;
	char	***split_file;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("Error\n Error opening file\n");
		return (NULL);
	}
	num_lines = count_lines(fd);
	close(fd);
	if (num_lines == 0)
		return (perror("Error\n empty file\n"), NULL);
	split_file = (char ***)malloc((num_lines + 1) * sizeof(char **));
	if (!split_file)
		return (perror("Error\n split_file() malloc failure\n"), NULL);
	split_file = fill_3darray(split_file, file, num_lines);
	return (split_file);
}
