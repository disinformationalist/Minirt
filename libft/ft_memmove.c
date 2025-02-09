/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 19:25:13 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/18 21:42:54 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dest1;
	unsigned char	*src1;

	i = 0;
	dest1 = (unsigned char *)dest;
	src1 = (unsigned char *)src;
	if (!dest && !src)
		return (dest);
	if (dest < src)
	{
		while (i < n)
		{
			dest1[i] = src1[i];
			i++;
		}
		return (dest);
	}
	while (n--)
	{
		dest1[n] = src1[n];
	}
	return (dest);
}
/*
int main()
{
		char str[50] = "stuff for the thing";
		//char str1[40];
		//ft_memmove(str + 10, str + 5, 19);
		memmove(str + 10, str + 5, 19);
		printf("%s\n", str);
		return 0;
}*/
