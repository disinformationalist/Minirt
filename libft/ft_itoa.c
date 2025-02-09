/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:47:46 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/11 16:59:55 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
/*int main()
{
	char *res = ft_itoa(-2147483648);
	int i = 0;
	while (i < 11)
	{
		printf("%c\n", res[i]);
	i++;
	}
	free(res);
	//printf("%s\n", ft_itoa(1234));
	return 0;
}*/
