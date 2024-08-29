#include "libft.h"

static void	str_swap(char **s1, char **s2)
{
	char	*hold;

	hold = *s1;
	*s1 = *s2;
	*s2 = hold;
}

static int	partition(char **tab, int start, int end)
{
	char	*pivot;
	int		i;
	int		j;

	j = start;
	i = start -1;
	pivot = tab[end];
	while (j <= end - 1)
	{
		if (ft_strcmp(tab[j], pivot) < 0)
		{
			i++;
			str_swap(&tab[i], &tab[j]);
		}
		j++;
	}
	str_swap(&tab[i + 1], &tab[end]);
	return (i + 1);
}

void	ft_quicksort_params(char **tab, int start, int end)
{
	int	p;

	if (start < end)
	{
		p = partition(tab, start, end);
		ft_quicksort_params(tab, start, p - 1);
		ft_quicksort_params(tab, p + 1, end);
	}
}
