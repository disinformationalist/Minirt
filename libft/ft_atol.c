/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:55:47 by jhotchki          #+#    #+#             */
/*   Updated: 2024/05/01 17:55:51 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	spaces(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	return (i);
}

long	ft_atol(const char *nptr)
{
	long	result;
	int		i;
	int		sign;

	result = 0;
	sign = 1;
	i = spaces(nptr);
	if (nptr[i] == 45 || nptr[i] == 43)
	{
		sign = 44 - nptr[i];
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result *= 10;
		result += nptr[i] - '0';
		i++;
	}
	result *= sign;
	return (result);
}
