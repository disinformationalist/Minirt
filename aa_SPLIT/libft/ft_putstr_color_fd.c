/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_color.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:57:31 by jhotchki          #+#    #+#             */
/*   Updated: 2024/05/01 17:57:33 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr_color_fd(int fd, char *s, char *color)
{
	int			len;
	const char	*reset_col;

	len = 0;
	reset_col = "\033[0m";
	write(fd, color, ft_strlen(color));
	while (*s)
		len += write(fd, s++, 1);
	write(fd, reset_col, ft_strlen(reset_col));
	return (len);
}
