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

typedef struct s_control
{
	t_img		*menu;
	t_img		*lt_con;
	t_img		*r;
	t_img		*g;
	t_img		*b;
	t_img		*bron;
	t_img		*sil;
	t_img		*ct;
	t_img		*pt;

	t_img		*rot_dials;
	t_img		*rot;
	t_img		*dtx;
	t_img		*dty;
	t_img		*dtz;

	t_img		*pos_dials;
	t_img		*pos;
	t_img		*dtpx;
	t_img		*dtpy;
	t_img		*dtpz;

	t_img		*sca_dials;
	t_img		*sca;
	t_img		*sca_tracks;
	t_img		*waist;
	int			w_width;
	int			w_height;

	t_img		*fov;
	int			f_width;
	int			f_height;

	t_img		*flash;
	int			dials_ys;
	
	int			d_width;
	int			d_height;
	int			m_height;
	int			m_width;
	int			k_width;
	int			k_height;
	int			ct_height;
	int			ct_width;
	int			pt_width;
	int			pt_height;
	int			dt_width;
	int			dt_height;

	int			sdials_ys;
	int			sd_width;
	int			sd_height;
	int			st_width;
	int			st_height;

	bool		on_dials;
	bool		rot_open;
	bool		pos_open;
	bool		sca_open;
	bool		lt_con_open;

	t_vec3		rots;
	t_position	rotsx;
	t_position	rotsy;
	t_position	rotsz;

	t_vec3		poss;
	t_position	posx;
	t_position	posy;
	t_position	posz;

	t_vec3		sca1;
	t_position	sca1x;
	t_position	sca1y;
	t_position	sca1z;
	
	t_vec3		sca2;
	t_position	sca2x;
	t_position	sca2y;
	t_position	sca2z;

	t_vec3		scale;
	t_position	scale_xyz;
	
	bool		pressed;
	int			press_xs;
	int			press_ys;
	int			press_width;
	int			press_height;


} t_control;

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