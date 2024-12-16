/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:06:36 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 14:06:38 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	my_pixel_put(int x, int y, t_img *img, unsigned int color)
{
	int	offset;

	offset = (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)(img->pixels_ptr + offset) = color;
}

void	clear_some(t_trace *trace)
{
	free_all_objects(trace);
	free(trace->mlx_connect);
	perror("Malloc error\n");
	exit(EXIT_FAILURE);
}

void	clear_all(t_trace *trace)
{
	free_all_objects(trace);
	if (trace->on)
		free(trace->on);
	if (trace->img.img_ptr)
		mlx_destroy_image(trace->mlx_connect, trace->img.img_ptr);
	mlx_destroy_window(trace->mlx_connect, trace->mlx_win);
	mlx_destroy_display(trace->mlx_connect);
	free(trace->mlx_connect);
	perror("Malloc error\n");
	exit(EXIT_FAILURE);
}

void	error_exit(char *msg)
{
	ft_putstr_color_fd(2, msg, RED);
	exit(EXIT_FAILURE);
}
