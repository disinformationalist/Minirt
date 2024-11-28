#include "minirt.h"

static char	*ft_strjoin_rt(char const *s1, char const *s2)
{
	char	*result;
	size_t	total_length;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	total_length = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	result = (char *)malloc((total_length + 4) * sizeof (char));
	if (result == NULL)
		return (NULL);
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	while (s2[j])
		result[i++] = s2[j++];
	result[i] = '.';
	result[++i] = 'r';
	result[++i] = 't';
	result[++i] = '\0';
	return (result);
}

char	*get_nxt_name_rt(char *name)
{
	static int	i = 0;
	char		*next_name;
	char		*num;

	i++;
	num = ft_itoa(i);
	if (!num)
		return (NULL);
	next_name = ft_strjoin_rt((const char *)name, (const char *)num);
	free(num);
	if (!next_name)
		return (NULL);
	while (!access(next_name, F_OK))
	{
		++i;
		free (next_name);
		num = ft_itoa(i);
		if (!num)
			return (NULL);
		next_name = ft_strjoin_rt((const char *)name, (const char *)num);
		free(num);
	}
	if (!next_name)
		return (NULL);
	return (next_name);
}
