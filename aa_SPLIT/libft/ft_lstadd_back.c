/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 23:23:12 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/12 23:26:43 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*curr;

	curr = *lst;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	while (curr -> next)
		curr = curr -> next;
	curr -> next = new;
}
