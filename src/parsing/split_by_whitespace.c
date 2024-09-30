/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 11:29:13 by arybarsk          #+#    #+#             */
/*   Updated: 2023/09/19 20:47:08 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	ft_iswhitespace(int c)
{
	if (c == ' ' || c == '\f' || c == '\n'
		|| c == '\r' || c == '\t' || c == '\v')
		return (1);
	else
		return (0);
}

int	count_words(char const *s)
{
	int	words;

	words = 0;
	if (!*s)
		return (0);
	while (*s)
	{
		while (*s && ft_iswhitespace(*s))
			s++;
		if (*s)
			words++;
		while (*s && !ft_iswhitespace(*s))
			s++;
	}
	return (words);
}

int	count_word_len(char const *s)
{
	int	word_len;

	word_len = 0;
	while (*s && ft_iswhitespace(*s))
		s++;
	while (*s && !ft_iswhitespace(*s))
	{
		s++;
		word_len++;
	}
	return (word_len);
}

void	*free_whole_array(int i, char **array)
{
	while (--i >= 0)
	{
		free(array[i]);
	}
	free(array);
	return (NULL);
}

char	**split_by_whitespace(char const *s)
{
	int		i;
	char	**array;
	char	num_of_words;

	if (!s)
		return (NULL);
	num_of_words = count_words(s);
	array = (char **)malloc(sizeof(char *) * (num_of_words + 1));
	if (!(array))
		return (NULL);
	i = 0;
	while (*s)
	{
		while (ft_iswhitespace(*s))
			s++;
		if (*s && count_word_len(s) > 0)
		{
			array[i++] = ft_substr(s, 0, count_word_len(s));
			if (!(array[i - 1]))
				return (free_whole_array(i - 1, array), NULL);
			s += count_word_len(s);
		}
	}
	array[i] = NULL;
	return (array);
}
