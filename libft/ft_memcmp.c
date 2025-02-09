/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 10:07:06 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/18 12:52:42 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*s11;
	unsigned char	*s21;

	s11 = (unsigned char *)s1;
	s21 = (unsigned char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && (*s11 == *s21))
	{
		s11++;
		s21++;
		i++;
	}
	return (*s11 - *s21);
}
/*
int main()
{
	char *str1 = "hello";
	char *str2 = "heillo";
	printf("%d\n", ft_memcmp(str1, str2, 10));
	printf("%d\n", memcmp(str1, str2, 10));
	return 0;
}*/
