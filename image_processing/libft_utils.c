#include "image_processing.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (s == NULL)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	size_t	i;
	char	*dest;
	char	*src;

	src = (char *)s;
	i = 0;
	dest = (char *)malloc((ft_strlen(src) + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin_n(int count, ...)
{
	va_list args;
	int		i;
	int		j;
	size_t	total_length;
	char	*result;
	const char *s;

	va_start(args, count);
	total_length = 0;
	i = -1;
	while (++i < count)
	{
		s = va_arg(args, const char *);
		total_length += ft_strlen(s);
	}
	va_end(args);
	result = (char *)malloc((total_length + 1) * sizeof (char));
	if (result == NULL)
		return (NULL);
	va_start(args, count);
	j = 0;
	i = -1;
	while (++i < count)
	{
		s = va_arg(args, const char *);
		while (*s)
			result[j++] = *s++;
	}
	va_end(args);

	result[j] = '\0';
	return (result);
}
