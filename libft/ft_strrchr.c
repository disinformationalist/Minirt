/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 07:33:24 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/18 21:41:57 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*s1;

	s1 = (char *)(s) + ft_strlen(s);
	if ((char)c == '\0')
		return (s1);
	s1--;
	while (s1 >= s)
	{
		if (*s1 == (char)c)
			return (s1);
		s1--;
	}
	return (NULL);
}
/*
int main()
{
    char c = 'o';
    const char *s = "some words";
    char *res = ft_strrchr(s, c);
	if(res != NULL)
    printf("%c\n", *res);
    else
    printf("char not found\n");
    //printf("%c\n", *(strrchr(s, c)));
    return 0;
}*/
