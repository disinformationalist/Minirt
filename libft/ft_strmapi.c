/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:02:28 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/18 16:12:28 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*res;
	unsigned int	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	res = (char *)malloc((ft_strlen(s) + 1) * sizeof (char));
	if (res == NULL)
		return (NULL);
	while (s[i])
	{
		res[i] = f(i, s[i]);
		i++;
	}
	res[i] = '\0';
	return (res);
}
/*
char	shift(unsigned int index, char c)
{
	(void)index;
	return(c + 2);
}
int main()
{
	char *s = "abcde";
	char *str;
	str = ft_strmapi(s, shift);
	printf("%s\n", str);
	free(str);
	return 0;
}*/			
