/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 08:24:43 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/18 11:33:45 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (*s1 && (*s1 == *s2) && i < n - 1)
	{
		s1++;
		s2++;
		i++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
/*

int main()
{
	const char *str = "get money";
	const char *str1 = "get paid";
	printf("%d\n", ft_strncmp(str, str1, 6));
	printf("%d\n", strncmp(str, str1, 6));
	return 0;
}*/
