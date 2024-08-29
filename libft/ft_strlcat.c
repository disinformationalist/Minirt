/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 21:59:54 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/11 11:16:50 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len_src;
	size_t	len_dst;
	char	*src1;

	src1 = (char *)src;
	i = 0;
	len_src = ft_strlen(src1);
	len_dst = ft_strlen(dst);
	if (size == 0 || size <= len_dst)
		return (len_src + size);
	while (src[i] && len_dst < size - 1)
	{
		dst[len_dst] = src1[i];
		i++;
		len_dst++;
	}
	dst[len_dst] = '\0';
	return (len_dst + ft_strlen(&src1[i]));
}
/*#include <bsd/string.h>
int main (void)
{
	char src[] = "hello";
	char dest[10] = "dest";
	printf("%lu\n", strlcat(dest, src, 8));
	printf("%s\n", dest);
}*/
