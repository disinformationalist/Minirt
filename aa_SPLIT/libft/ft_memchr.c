/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:29:42 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/18 21:53:50 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*s1;

	c = (unsigned char)c;
	s1 = (unsigned char *)s;
	while (n--)
	{
		if (*s1 == c)
			return ((void *)s1);
		s1++;
	}
	return (NULL);
}
/*
int main()
{
	char c = 'e';
	const char *s = "hallo welt";
	char *res = memchr(s, c, 13);
	if(res != NULL)
	printf("%c\n", *res);
	else
	printf("char not found\n");
	return 0;
}*/	
