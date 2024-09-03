#ifndef EXTRAS_H
# define EXTRAS_H

# include "tools.h"
//# include "minirt.h"
typedef struct s_pixel
{
	uint8_t alpha;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}	t_pixel;

//struct for downsample variables.

typedef struct s_filter
{
	int				half_k;
	int				x;
	int				y;
	int				i;
	int				j;
	double			red;
	double			green;
	double			blue;
	int				avg_r;
	int				avg_g;
	int				avg_b;
	unsigned int	pixel;
	int				pix_x;
	int				pix_y;
}		t_filter;

//for import export

typedef struct s_png_io
{
	int			y;
	int			x;
	int			pixel_size;
	int			depth;
	t_pixel		temp_pixel;
	png_byte	**row_pointers;
	png_infop	info;
	png_structp	png_ptr;
	FILE		*fp;
	png_text	*text;
}	t_png_io;

/***PNG IMPORT/EXPORT***/

//export lossless png image
int				export_png(const char *filename, t_img *img, int width, int height, png_text *text);
void			get_pixel(t_pixel *pix_t, t_img *img, int x, int y);
void			clean_memory(t_png_io *png_img, int j, bool export);
char 			*get_nxt_name(char *name);

//export/import utils
int				error_1(t_png_io *png_img, const char *msg);
void			free_png_rows(png_structp png_ptr, png_byte **row_pointers, int j);
void			clean_memory(t_png_io *png_img, int j, bool export);
void			init_vars(t_png_io *png_img);

//import png img.
t_img			*import_png(void *mlx_ptr, const char *file, int *width, int *height);
void			*error_2(t_png_io *png_img, const char *msg);
int				error_3(t_png_io *png_img, const char *msg);

//sampling
void			downsample_xl(int width, int height, t_img *img, unsigned int **pixels_xl, int kern_size);


//mthread
int				get_num_cores(void);

#endif