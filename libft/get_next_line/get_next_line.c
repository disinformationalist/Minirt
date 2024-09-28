/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 22:01:49 by arybarsk          #+#    #+#             */
/*   Updated: 2023/10/10 13:52:30 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE  5

#endif

int	ft_strlen2(const char *str)
{
	int	count;

	count = 0;
	if (!str)
		return (0);
	while (*str)
	{
		str++;
		count++;
	}
	return (count);
}

void	*ft_memmove2(void *to, const void *from, size_t n)
{
	char		*t;
	const char	*f;
	size_t		i;

	t = (char *) to;
	f = (const char *) from;
	i = 0;
	if (t == f)
		return (to);
	if (t < f || t >= (f + n))
	{
		while (i < n)
		{
			t[i] = f[i];
			i++;
		}
	}
	else
	{
		i = n;
		while (i-- > 0)
			t[i] = f[i];
	}
	return (to);
}

char	*ft_strdup2(const char *s)
{
	int		s_size;
	char	*pass_string;
	char	*new_string;
	char	*new_string_start;

	pass_string = (char *) s;
	s_size = ft_strlen(pass_string);
	new_string = (char *)malloc(s_size + 1);
	if (new_string == NULL)
		return (NULL);
	new_string_start = new_string;
	while (*s)
	{
		*new_string = *s;
		new_string++;
		s++;
	}
	*new_string = '\0';
	return (new_string_start);
}

char	*ft_strjoin2(char const *s1, char const *s2)
{
	int		i;
	int		s1_size;
	int		s2_size;
	char	*joined_string;

	i = 0;
	s1_size = ft_strlen2((char *) s1);
	s2_size = ft_strlen2((char *) s2);
	joined_string = malloc(sizeof(char) * (s1_size + s2_size + 1));
	if (joined_string == NULL)
		return (NULL);
	while (i < s1_size)
	{
		joined_string[i] = s1[i];
		i++;
	}
	while (i < (s1_size + s2_size))
	{
		joined_string[i] = s2[i - s1_size];
		i++;
	}
	joined_string[i] = '\0';
	return (joined_string);
}

char	*ft_strchr2(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s && *s != (unsigned char) c)
		s++;
	if (*s == (unsigned char) c)
		return ((char *) s);
	return (NULL);
}



unsigned int	ft_strlcpy2(char *dest, char *src, unsigned int size)
{
	unsigned int	i;
	unsigned int	length;

	i = 0;
	length = ft_strlen2(src);
	if (size != 0)
	{
		while (src[i] != '\0' && i < size - 1)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (length);
}

ssize_t	read_to_keeper(int fd, char **state_keeper)
{
	char	*buffer;
	char	*temp;
	ssize_t	readout;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (-1);
	readout = 0;
	while (!ft_strchr2(*state_keeper, '\n'))
	{
		readout = read(fd, buffer, BUFFER_SIZE);
		if (readout <= 0)
			break ;
		buffer[readout] = '\0';
		temp = ft_strjoin2(*state_keeper, buffer);
		free(*state_keeper);
		*state_keeper = temp;
	}
	free(buffer);
	return (readout);
}

char	*cut_line_from_keeper(char **state_keeper)
{
	char	*line;
	char	*found_newline;
	int		line_len;
	int		rest_of_keeper;

	if (!*state_keeper || ft_strlen2(*state_keeper) < 1)
		return (NULL);
	found_newline = ft_strchr2(*state_keeper, '\n');
	if (found_newline)
	{
		rest_of_keeper = ft_strlen2(found_newline + 1);
		line_len = ft_strlen2(*state_keeper) - rest_of_keeper;
		line = (char *)malloc(sizeof(char) * (line_len + 1));
		if (!line)
			return (NULL);
		ft_strlcpy2(line, *state_keeper, line_len + 1);
		ft_memmove2(*state_keeper, found_newline + 1, rest_of_keeper + 1);
	}
	else
	{
		line = ft_strdup2(*state_keeper);
		if (!line)
			return (NULL);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*state_keeper;
	ssize_t		result;
	char		*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!state_keeper)
	{
		state_keeper = ft_strdup2("");
		if (!state_keeper)
			return (NULL);
	}
	result = read_to_keeper(fd, &state_keeper);
	if (result < 0)
		return (free(state_keeper), state_keeper = NULL, NULL);
	line = cut_line_from_keeper(&state_keeper);
	if (!(ft_strchr2(line, '\n')))
	{
		free(state_keeper);
		state_keeper = NULL;
	}
	return (line);
}