/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extras.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:54:41 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 13:54:43 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXTRAS_H
# define EXTRAS_H

# include "tools.h"

typedef struct s_pixel
{
	uint8_t	alpha;
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;
}	t_pixel;

//for import export

typedef struct s_png_io
{
	int			y;
	int			x;
	int			pixel_size;
	int			depth;
	int			color_type;
	t_pixel		temp_pixel;
	png_byte	**row_pointers;
	png_infop	info;
	png_structp	png_ptr;
	FILE		*fp;
	png_text	*text;
}	t_png_io;

//texures

typedef struct s_tx
{
	int				id;
	char			*i_name;
	char			*m_name;
	t_img			*image;
	t_img			*bump_map;
	int				i_height;
	int				i_width;
	int				m_height;
	int				m_width;
	double			img_iasp;
	struct s_tx		*prev;
	struct s_tx		*next;
}	t_tx;

/***PNG IMPORT/EXPORT***/

//export lossless png image
int				export_png(const char *filename, \
				t_img *img, int width, int height);
void			get_pixel(t_pixel *pix_t, t_img *img, int x, int y);
void			clean_memory(t_png_io *png_img, int j, bool export);
char			*get_nxt_name(char *name);
void			init_vars(t_png_io *png_img);

//export/import utils
int				error_1(t_png_io *png_img, const char *msg);
void			free_png_rows(png_structp png_ptr, \
				png_byte **row_pointers, int j);
void			clean_memory(t_png_io *png_img, int j, bool export);

//import png img.
t_img			*import_png(void *mlx_ptr, \
				const char *file, int *width, int *height);
int				init_import_vars(t_png_io *png_img);
void			*error_2(t_png_io *png_img, const char *msg);
int				error_3(t_png_io *png_img, const char *msg);
void			set_img_pixels_rgb(t_png_io *png_img, \
				t_img *image, int width, int height);
void			set_img_pixels_rgba(t_png_io *png_img, \
				t_img *image, int width, int height);
void			set_img_pixels_gray(t_png_io *png_img, \
				t_img *image, int width, int height);
void			set_img_pixels_gray_alpha(t_png_io *png_img, \
				t_img *image, int width, int height);

//mthread
int				get_num_cores(void);

#endif
