/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 12:53:43 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/11 12:14:04 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	void	*ptr;

	i = 0;
	ptr = malloc(nmemb * size);
	if (ptr == NULL)
		return (NULL);
	while (i < nmemb * size)
	{
		((char *)ptr)[i] = 0;
		i++;
	}
	return (ptr);
}
/*
int main()
{
	int i = 0;
	char *str = (char *)ft_calloc(5, sizeof(char));

	while (i < 5)
	{
		printf("%c\n", *str);
		i++;
	}
	free(str);
	char *str1 = (char *)calloc(5, sizeof(char));

    while (i < 5)
    {
        printf("%c\n", *str1);
        i++;
    }
    free(str1);

	return 0;
}*/
