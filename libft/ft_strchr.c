/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 06:45:39 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/18 12:47:04 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}
/*
int main()
{
	char c = 'a';
	const char *s = "hallo welt";
	char *res = ft_strchr(s, c);
	if(res != NULL)
	printf("%c\n", *res);
	else
	printf("char not found\n");
	//printf("%c\n", *(strchr(s, c)));
	return 0;
}*/
