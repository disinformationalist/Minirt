/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 20:17:31 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/11 23:50:39 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (new == NULL)
		return (NULL);
	new -> content = content;
	new -> next = NULL;
	return (new);
}
/*
int main()
{
	int	*test = (int*)malloc(sizeof(int));
	*test = 289;
	t_list *new = ft_lstnew(test);

	printf("%d\n", *((int*)new->content));
	free(new->content);
	free(new);
	return 0;
}
*/
