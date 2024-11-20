
#include "../libft.h"

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
