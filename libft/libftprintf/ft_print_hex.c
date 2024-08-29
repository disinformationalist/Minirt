/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 09:45:54 by jhotchki          #+#    #+#             */
/*   Updated: 2023/10/20 09:46:02 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_hash(t_items *item)
{
	if (item->specifier == 'x')
		write(1, "0x", 2);
	if (item->specifier == 'X')
		write(1, "0X", 2);
	if (item->specifier == 'o' && item->baselen >= item->precision)
		write(1, "0", 1);
}

static void	ft_assign(t_items *item)
{
	if (item->specifier == 'x')
	{
		item->base = "0123456789abcdef";
		item->base_n = 16;
	}
	if (item->specifier == 'X')
	{
		item->base = "0123456789ABCDEF";
		item->base_n = 16;
	}
	if (item->specifier == 'o')
	{
		item->base = "01234567";
		item->base_n = 8;
	}
}

static int	ft_hex_helper(t_items *item, int len, unsigned int n)
{
	if (item->hash && n != 0 && item->specifier != 'o')
		len += 2;
	else if (item->hash && n != 0 && item->specifier == 'o'
		&& item->baselen >= item->precision)
		len += 1;
	if ((item->width && !item->dash) && (!item->zero || item->point))
		ft_right_align(item, len);
	if (item->hash && n != 0)
		ft_hash(item);
	if ((item->zero && !item->dash && !item->point)
		|| (item->precision > item->baselen))
		pad_zeros(item, len);
	return (len);
}

void	ft_print_hex(t_items *item)
{
	unsigned int	n;
	int				len;

	n = (unsigned int)va_arg(item->args, unsigned int);
	ft_assign(item);
	item->baselen = ft_len_base(n, item->base_n);
	len = item->baselen;
	if (!n && !item->precision && item->point)
		len--;
	len = ft_hex_helper(item, len, n);
	if (n != 0 || (n == 0 && (!item->point || item->precision)))
		ft_putnum_base(n, item->base);
	if (item->dash)
		ft_left_align(item, len);
	item->length += len;
	initialize_item(item);
}
