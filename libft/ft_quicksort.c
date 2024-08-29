/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quicksort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 09:42:10 by jhotchki          #+#    #+#             */
/*   Updated: 2024/01/04 09:42:13 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	swap(int *a, int *b)
{
	int	hold;

	hold = *a;
	*a = *b;
	*b = hold;
}

static int	partition(int *tab, int start, int end)
{
	int	pivot;
	int	i;
	int	j;

	j = start;
	i = start - 1;
	pivot = tab[end];
	while (j <= end - 1)
	{
		if (tab[j] < pivot)
		{
			i++;
			swap(&tab[i], &tab[j]);
		}
		j++;
	}
	swap(&tab[i + 1], &tab[end]);
	return (i + 1);
}

void	ft_quicksort(int *tab, int start, int end)
{
	int	p;

	if (start < end)
	{
		p = partition(tab, start, end);
		ft_quicksort(tab, start, p - 1);
		ft_quicksort(tab, p + 1, end);
	}
}
