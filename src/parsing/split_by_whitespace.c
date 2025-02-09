#include "minirt.h"

bool	ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

static size_t	str_count(char *str)
{
	size_t	i;
	size_t	count;
	int		in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
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

static char	**ft_allocate(char **split, const char *s)
{
	size_t	j;
	char	*start;

	j = 0;
	while (*s)
	{
		while (*s && ft_isspace(*s))
			s++;
		if (*s)
		{
			start = (char *)s;
			while (*s && !ft_isspace(*s))
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

char	**split_by_whitespace(const char *s)
{
	size_t	count;
	char	**split;

	count = str_count((char *)s);
	split = (char **)malloc(((count + 1) * sizeof (char *)));
	if (split == NULL)
		return (NULL);
	split = ft_allocate(split, s);
	return (split);
}
