/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:07:58 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/11 18:59:35 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		(*f)(i, &s[i]);
		i++;
	}
}
/*
void	shift_even(unsigned int index, char *c)
{
		if(index % 2 == 0)
			*c += 2;
}

int main()
{
		char str[] = "abcdefg";
		ft_striteri(str, shift_even);
		printf("%s\n", str);
		return 0;
}
*/
