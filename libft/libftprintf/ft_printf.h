/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 09:26:39 by jhotchki          #+#    #+#             */
/*   Updated: 2023/10/21 00:52:11 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "../libft.h"

typedef struct s_items
{
	va_list	args;
	int		dash;
	int		zero;
	int		point;
	int		plus;
	int		space;
	int		hash;
	int		width;
	int		length;
	int		precision;
	int		baselen;
	char	specifier;
	char	*base;
	int		base_n;
}	t_items;

int		ft_printf(const char *format, ...);
void	initialize_item(t_items *item);
void	ft_print_nbr_u(t_items *item);
int		ft_get_width(t_items *item, const char *s, int pos);
int		ft_get_precision(t_items *item, const char *s, int pos);
void	pad_zeros(t_items *item, int len);
void	ft_right_align(t_items *item, int len);
void	ft_left_align(t_items *item, int len);
int		ft_strlen_2(char *str);
void	ft_print_char(t_items *item);
void	ft_print_str(t_items *item);
void	ft_putchar(char c);
int		ft_len_base(long int n, int base);
void	ft_putnum_base(long int n, char *base);
void	ft_print_ptr(t_items *item);
void	ft_print_nbr(t_items *item);
void	ft_print_hex(t_items *item);
void	ft_print_per(t_items *item);
int		ft_check_error(const char *format, int pos);
int		ft_error_handle(t_items *item, const char *format, int pos);

#endif
