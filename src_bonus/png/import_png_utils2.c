/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   import_png_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:29:29 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 14:29:30 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	*error_2(t_png_io *png_img, const char *msg)
{
	png_destroy_read_struct(&png_img->png_ptr, &png_img->info, NULL);
	fclose(png_img->fp);
	free(png_img);
	perror(msg);
	return (NULL);
}

int	error_3(t_png_io *png_img, const char *msg)
{
	png_destroy_read_struct(&png_img->png_ptr, &png_img->info, NULL);
	fclose(png_img->fp);
	free(png_img);
	perror(msg);
	return (1);
}
