/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:23:16 by jhotchki          #+#    #+#             */
/*   Updated: 2023/10/21 01:04:05 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	initialize_item(t_items *item)
{
	item->dash = 0;
	item->zero = 0;
	item->point = 0;
	item->plus = 0;
	item->space = 0;
	item->hash = 0;
	item->width = 0;
	item->precision = 0;
	item->baselen = 0;
	item->specifier = 0;
	item->base = NULL;
	item->base_n = 0;
}

static void	ft_print_format(t_items *item, const char *format, int pos)
{
	item->specifier = format[pos];
	if (format[pos] == 'c')
		ft_print_char(item);
	else if (format[pos] == 's')
		ft_print_str(item);
	else if (format[pos] == 'p')
		ft_print_ptr(item);
	else if (format[pos] == 'd' || format[pos] == 'i')
		ft_print_nbr(item);
	else if (format[pos] == 'u')
		ft_print_nbr_u(item);
	else if (format[pos] == 'x' || format[pos] == 'X' || format[pos] == 'o')
		ft_print_hex(item);
	else if (format[pos] == '%')
		ft_print_per(item);
}

static int	ft_check_flags(t_items *item, const char *format, int pos)
{
	while (ft_strchr("0-+# ", format[pos]))
	{
		if (format[pos] == '0')
			item->zero = 1;
		if (format[pos] == '-')
			item->dash = 1;
		if (format[pos] == '+')
			item->plus = 1;
		if (format[pos] == '#')
			item->hash = 1;
		if (format[pos] == ' ')
			item->space = 1;
		pos++;
	}
	return (pos);
}

static int	ft_manage_format(t_items *item, const char *format, int pos)
{
	int	err_flag;
	int	start;

	start = pos;
	pos = ft_check_flags(item, format, pos);
	err_flag = ft_check_error(format, start);
	if (!err_flag && ft_strchr("cspdiuoxX%123456789.", format[pos]))
	{
		while (format[pos + 1] && !ft_strchr("cspdiuoxX%", format[pos]))
		{
			if (ft_isdigit(format[pos]) && !(item->point))
				pos = ft_get_width(item, format, pos);
			if (format[pos] == '.')
			{
				item->point = 1;
				pos = ft_get_precision(item, format, pos + 1);
			}
			pos++;
		}
		ft_print_format(item, format, pos);
		return (pos);
	}
	pos = ft_error_handle(item, format, pos);
	return (pos);
}

int	ft_printf(const char *format, ...)
{
	t_items	*item;
	int		i;
	int		count;

	i = -1;
	count = 0;
	if (!format)
		return (-1);
	item = (t_items *)malloc(sizeof(t_items));
	if (!item)
		return (-1);
	initialize_item(item);
	item->length = 0;
	va_start(item->args, format);
	while (format[++i])
	{
		if (format[i] == '%')
			i = ft_manage_format(item, format, i + 1);
		else
			count += write(1, &format[i], 1);
	}
	va_end(item->args);
	count += item->length;
	free(item);
	return (count);
}
/* int main()
{
	int	a;
	int b;
	int v = 6;
	int *p = 0; 
	char *str;
	str = NULL;
	a = ft_printf("|%#10.10o|\n", -2222) - 3;
	b = printf("|%#10.10o|\n", -2222) - 3;
	
	printf("%d\n", a);
	printf("%d\n", b);
	return (0);
}  */