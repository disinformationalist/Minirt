/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_push_to_stack.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:49:44 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 13:49:49 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_push(t_stack **to, t_stack **from)
{
	t_stack	*node;

	if (*from == NULL)
		return ;
	node = *from;
	*from = node->next;
	if (*from)
		(*from)->prev = NULL;
	if (*to == NULL)
	{
		*to = node;
		(*to)->next = NULL;
	}
	else
	{
		(*to)->prev = node;
		node->next = *to;
		*to = node;
	}
}
