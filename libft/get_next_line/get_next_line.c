/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 10:53:11 by jhotchki          #+#    #+#             */
/*   Updated: 2023/10/23 18:53:48 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE  4

#endif

static char	*ft_temp(char *s1, char *s2)
{
	char	*temp;

	temp = ft_strjoin_g(s1, s2);
	free(s1);
	return (temp);
}

static char	*extra(char *s)
{
	char	*s1;
	int		i;
	int		j;

	j = 0;
	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	if (!s[i])
	{
		free(s);
		return (NULL);
	}
	s1 = (char *)malloc((ft_strlen_g(s) - i + 1) * sizeof(char));
	if (!s1)
	{
		free(s);
		return (NULL);
	}
	while (s[i])
		s1[j++] = s[++i];
	s1[j] = '\0';
	free(s);
	return (s1);
}

static char	*make_line(char *s)
{
	char	*s1;
	int		len;
	int		i;

	if (!(*s))
		return (NULL);
	len = 0;
	while (s[len] && s[len] != '\n')
		len++;
	s1 = (char *)malloc((len + 2) * sizeof(char));
	if (!s1)
		return (NULL);
	i = -1;
	while (++i < len)
		s1[i] = s[i];
	if (s[i] == '\n')
	{
		s1[i] = s[i];
		i++;
	}
	s1[i] = '\0';
	return (s1);
}

static char	*build_raw_line(int fd, char *buffer, char *s)
{
	long long int	bytes_read;

	bytes_read = 1;
	while (!ft_strchr_g(s, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(s);
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		s = ft_temp(s, buffer);
		if (!s)
		{
			free(buffer);
			return (NULL);
		}
	}
	free(buffer);
	return (s);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*str = NULL;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	if (str == NULL)
	{
		str = (char *)malloc(sizeof(char));
		if (!str)
			return (NULL);
		*str = '\0';
	}
	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
	{
		free(str);
		return (NULL);
	}
	str = build_raw_line(fd, buffer, str);
	if (!str)
		return (NULL);
	line = make_line(str);
	str = extra(str);
	return (line);
}
