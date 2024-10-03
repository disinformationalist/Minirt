/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_chars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 22:43:13 by jhotchki          #+#    #+#             */
/*   Updated: 2023/10/16 22:43:23 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_print_char(t_items *item)
{
	char	c;

	c = va_arg(item->args, int);
	if (item->width && !(item->dash))
		ft_right_align(item, 1);
	item->length += write(1, &c, 1);
	if (item->width && item->dash)
		ft_left_align(item, 1);
	initialize_item(item);
}

void	ft_print_str(t_items *item)
{
	char	*str;
	int		i;
	int		len;

	i = -1;
	str = va_arg(item->args, char *);
	if ((!str && item->precision >= 6) || (!str && !item->point))
		str = "(null)";
	if (!str && item->point && item->precision < 6)
		str = "";
	len = ft_strlen_2(str);
	item->baselen = len;
	if (item->point)
		item->baselen = item->precision;
	if (item->point && item->precision < len)
		len = item->precision;
	if (item->width && !item->dash)
		ft_right_align(item, len);
	while (str[++i] && i < len)
		item->length += write(1, &str[i], 1);
	if (item->width && item->dash)
		ft_left_align(item, len);
	initialize_item(item);
}

void	ft_print_per(t_items *item)
{
	item->length += write(1, "%", 1);
	initialize_item(item);
}
