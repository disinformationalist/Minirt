/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 10:21:27 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/18 21:54:45 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*hold;
	t_list	*curr;

	if (!lst || !del)
		return ;
	curr = *lst;
	while (curr)
	{
		hold = curr -> next;
		(*del)(curr -> content);
		free(curr);
		curr = hold;
	}
	*lst = NULL;
}
