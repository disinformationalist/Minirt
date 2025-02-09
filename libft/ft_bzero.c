/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:21:42 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/11 10:31:36 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*s1;

	s1 = (unsigned char *)s;
	while (n)
	{
		*s1 = 0;
		s1++;
		n--;
	}
}
/*
int main()
{
		char str[20] = "thingsforstuff";
		ft_bzero(str + 3, 5);
		//bzero(str + 3, 5);
		printf("%s\n", str);
		printf("%s\n", str + 9);
	return 0;
}*/
