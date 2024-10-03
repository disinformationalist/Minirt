/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_width.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 17:36:56 by jhotchki          #+#    #+#             */
/*   Updated: 2023/10/21 00:58:56 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_get_width(t_items *item, const char *s, int pos)
{
	int	res;

	res = 0;
	while (ft_isdigit(s[pos]) && !ft_strchr(".cspdiuoxX%", s[pos]))
	{
		res = res * 10 + s[pos] - '0';
		pos++;
	}
	item->width = res;
	return (--pos);
}

int	ft_get_precision(t_items *item, const char *s, int pos)
{
	int	res;

	res = 0;
	while (ft_isdigit(s[pos]) && !ft_strchr("cspdiuoxX%", s[pos]))
	{
		res = res * 10 + s[pos] - '0';
		pos++;
	}
	item->precision = res;
	return (--pos);
}
