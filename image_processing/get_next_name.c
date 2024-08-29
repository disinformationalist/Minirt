#include "image_processing.h"
//USE THIS VERSION TO UPDATE ONE IN MINIRT
/* name = get_next_name("chaos_"); 
result file name: chaos_1.png 
next export: chaos_2.png ... etc.
this will pick up where the previous left off, and will also fill any blanks in
the counting sequence, so it you have chaos_1.png, chaos_2.png, chaos_5.png, 
get next name will start with chaos_3.png, then 4, then 6, and so on.
 */
static int	count_digit(int n)
{
	int	count;

	if (n < 0)
		count = 1;
	else
		count = 0;
	while (n / 10)
	{
		n = n / 10;
		count++;
	}
	return (++count);
}

static int	sign(int a)
{
	if (a < 0)
		a = -a;
	return (a);
}

static void	cases( int n, int n_start, char *str)
{
	if (n_start >= 0)
		*str = n + '0';
	else
		*str = '-';
}

char	*ft_itoa(int n)
{
	char	*str;
	int		count;
	int		n_start;

	n_start = n;
	count = count_digit(n);
	str = (char *)malloc((count + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[count] = '\0';
	while (--count)
	{
		str[count] = sign(n % 10) + '0';
		if (n < 0 && count == 1)
			str[count] = -n + '0';
		n /= 10;
	}
	cases(n, n_start, str);
	return (str);
}

static char	*ft_strjoin_png(char const *s1, char const *s2)
{
	char	*result;
	size_t	total_length;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	total_length = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	result = (char *)malloc((total_length + 5) * sizeof (char));
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
	result[++i] = 'p';
	result[++i] = 'n';
	result[++i] = 'g';
	result[++i] = '\0';
	return (result);
}

char *get_nxt_name(char *name)
{
	static int	i = 0;
	char		*next_name;
	char		*num;
	
	i++;
	num = ft_itoa(i);
	if (!num)
		return (NULL);
	next_name = ft_strjoin_png((const char*)name, (const char*)num);
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
		next_name = ft_strjoin_png((const char*)name, (const char*)num);
		free(num);
	}
	if (!next_name)
		return (NULL);
	return (next_name);
}