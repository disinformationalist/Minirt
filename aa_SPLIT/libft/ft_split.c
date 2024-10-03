/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 07:32:50 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/23 09:51:30 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	str_count(char *str, char c)
{
	size_t	i;
	size_t	count;
	int		in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (str[i])
	{
		if (str[i] == c)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		i++;
	}
	return (count);
}

static void	makes_free(char **strs, size_t j)
{
	while (j)
	{
		free(strs[j - 1]);
		j--;
	}
	free(strs);
}

static char	**ft_allocate(char **split, const char *s, char c)
{
	size_t	j;
	char	*start;

	j = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			start = (char *)s;
			while (*s && *s != c)
				s++;
			split[j] = ft_substr(start, 0, s - start);
			if (split[j] == NULL)
			{
				makes_free(split, j);
				return (NULL);
			}
			j++;
		}
	}
	split[j] = NULL;
	return (split);
}

char	**ft_split(const char *s, char c)
{
	size_t	count;
	char	**split;

	count = str_count((char *)s, c);
	split = (char **)malloc(((count + 1) * sizeof (char *)));
	if (split == NULL)
		return (NULL);
	split = ft_allocate(split, s, c);
	return (split);
}
/*int	main(void)
{
	char const	*str = "hello!";
	char	**result = ft_split(str, ' ');
	int		i = 0;
	while (result[i])
	{
		printf("%s\n", result[i]);
		free (result[i]);
		i++;
	}
    free (result);
	return (0);
}*/
