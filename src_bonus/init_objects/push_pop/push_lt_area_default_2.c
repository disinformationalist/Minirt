/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_lt_area_default_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:24:38 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 14:24:41 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	adj_pntrs_copy2(t_light *to_copy, t_light *new)
{
	new->next = to_copy->next;
	new->prev = to_copy;
	to_copy->next->prev = new;
	to_copy->next = new;
}
