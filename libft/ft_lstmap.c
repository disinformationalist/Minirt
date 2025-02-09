/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 10:23:20 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/19 11:07:42 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*holdc;
	t_list	*holdl;

	if (!f || !del || !lst)
		return (NULL);
	head = NULL;
	while (lst)
	{
		holdc = f(lst -> content);
		holdl = ft_lstnew(holdc);
		if (!holdl)
		{
			free(holdc);
			free(holdl);
			ft_lstclear(&head, del);
			return (NULL);
		}
		ft_lstadd_back(&head, holdl);
		lst = lst -> next;
	}
	return (head);
}
