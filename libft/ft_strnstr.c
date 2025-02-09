/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 10:45:59 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/11 11:49:26 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	char	*big1;
	char	*little1;

	big1 = (char *)big;
	little1 = (char *)little;
	i = 0;
	j = 0;
	if (!little1[0])
		return (big1);
	if (len == 0)
		return (0);
	while (big1[i] && i < len)
	{
		while (big1[i + j] && big1[i + j] == little1[j] && i + j < len)
			j++;
		if (!little1[j])
			return (big1 + i);
		i++;
		j = 0;
	}
	return (NULL);
}
/*
#include <bsd/string.h>

int main()
{
	char *str = "here is a string";
	char *str2 = "string";
	char *result = (ft_strnstr(str, str2, 16));
	char *result1 = (strnstr(str, str2, 16));
	printf("%p\n", result);
	printf("%p\n", result1);
	return 0;
}
*/
