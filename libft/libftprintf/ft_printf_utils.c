/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:28:30 by jhotchki          #+#    #+#             */
/*   Updated: 2023/10/09 20:09:02 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_strlen_2(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	pad_zeros(t_items *item, int len)
{
	int	i;

	i = 0;
	if (item->precision > item->baselen)
	{
		while (i < (item->precision - item->baselen))
		{
			item->length += write(1, "0", 1);
			i++;
		}
	}
	else if (!(item->precision > item->baselen))
	{
		while (i < (item->width - len))
		{
			item->length += write(1, "0", 1);
			i++;
		}
	}
}

void	ft_right_align(t_items *item, int len)
{
	int	i;

	i = 0;
	if (item->precision > item->baselen)
	{
		while (i < (item->width - (item->precision - item->baselen) - len))
		{
			item->length += write(1, " ", 1);
			i++;
		}
	}
	else if (!(item->precision > item->baselen))
	{
		while (i < (item->width - len))
		{
			item->length += write(1, " ", 1);
			i++;
		}
	}
}

void	ft_left_align(t_items *item, int len)
{
	int	i;

	i = 0;
	if (item->precision > item->baselen)
	{
		while (i < (item->width - (item->precision - item->baselen) - len))
		{
			item->length += write(1, " ", 1);
			i++;
		}
	}
	else if (!(item->precision > item->baselen))
	{
		while (i < (item->width - len))
		{
			item -> length += write(1, " ", 1);
			i++;
		}
	}
}
