/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 21:17:22 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/11 11:08:22 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = ft_strlen((char *)src);
	if (size != 0)
	{
		while (i < size - 1 && src[i])
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (j);
}
/*
#include <bsd/string.h>

int main()

{
	//char	src[] = "stuff";
	//char	dest[] = "things";
	char	src1[] = "stuff";
	char	dest1[] = "things";

	
	printf("%lu\n", strlcpy(dest1, src1, 3));
	printf("%s", dest1);
	return (0);
}*/
