/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:33:22 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/18 21:44:46 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	char	*s1;
	size_t	i;

	i = 0;
	s1 = (char *)s;
	while (i < n)
	{
		*s1 = (char)c;
		i++;
		s1++;
	}
	return ((char *)s);
}
/*
int main()
{
	//	int i = 0;
		//int n = 10;
		//int arr[n];
		char str[20] = "stuff for the thing";
	//	char *str1 = memset(str + 5, '#', 5*sizeof(char));
		char *str1 = ft_memset((str + 5), 'A', 5);
		
		printf("%s\n", str1);
	//	memset(arr, '/', n*sizeof(arr[0])); 

		return 0;
}*/
