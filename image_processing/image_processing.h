#ifndef IMAGE_PROCESSING_H
# define IMAGE_PROCESSING_H

#if defined(_WIN32) || defined(_WIN64)
# include <windows.h>
#else
# include <unistd.h>
#endif
/*TO DO LIST
->EXPORT IMAGE!! png done need jpeg
-> SUPERSAMPLERs
->GAUSIAN SUPERSAMPLER and other supersample methods...
->FFT TO AND BACK

->IMAGE cropping top or bottom or left or right.
HIGH/LOW PASS FILTER
BUTTERWORTH
SOBEL

->UNSHARP_MASKING!!!!! use convo or gauss blurs? using gauss is standard*/ 
//de noising monte, non- local means... etc.

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <stddef.h>
# include <png.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdarg.h>
# include "../minilibx-linux/mlx.h"

# define SQ(x) ((x) * (x))


/***STRUCTS***/

typedef struct s_img
{
	void	*img_ptr;
	char	*pixels_ptr;
	int		bpp;
	int		endian;
	int		line_len;
}	t_img;

typedef struct s_pixel
{
	uint8_t alpha;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}	t_pixel;

typedef struct s_position
{
	int	x;
	int	y;
}	t_position;

typedef struct s_filter
{
	int				half_k;
	int				x;
	int				y;
	int				i;
	int				j;
	int				x_r;
	int				y_r;
	
	double			mean_r;
	double			mean_g;
	double			mean_b;

	double			red;
	double			green;
	double			blue;
	int				offset;
	int				avg_r;
	int				avg_g;
	int				avg_b;
	unsigned int	pixel;
	double				**kernel;
	int				factor;
	double			kern_sum;
	int				pix_x;
	int				pix_y;
}		t_filter;

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

typedef struct s_wheel
{
	int		*colors;
	float	hue_step;
	float	c;
	float	x;
	float	m;
	float	r_;
	float	g_;
	float	b_;
	int		hue;
}				t_wheel;

typedef struct s_color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}	t_color;

typedef enum e_op
{
	ZEROS,
	COLOR,
	REFLECT,
	DUPLICATE,
} t_op;

typedef enum e_channel
{
	NONE = 0,
	R = 1 << 0,
	G = 1 << 1,
	B = 1 << 2,
	RG = R | G,
	RB = R | B,
	GB = G | B,
	GR = G | R,
	BR = B | R,
	BG = B | G,
	RGB = R | G | B,
	RBG = R | B | G,
	BGR = B | G | B,
	BRG = B | R | G,
	GBR = G | B | R,
	GRB = G | R | B,
} t_channel;


/***LIB UTILS***/
char			*ft_strjoin_n(int count, ...);
char			*ft_strdup(const char *s);
size_t			ft_strlen(const char *s);
char			*ft_itoa(int n);

/******PIXEL GET AND SET******/

void			my_pixel_put(int x, int y, t_img *img, unsigned int color);
void			my_pixel_put_plus(int x, int y, t_img *img, unsigned int color);
unsigned int	pixel_color_get(int x, int y, t_img *img);
void			get_pixel(t_pixel *pix_t, t_img *img, int x, int y);

// image creation
int				new_img_init(void *mlx_connection, t_img *img, int width, int height);
t_img 			*create_img(void *mlx_ptr, int width, int height);
t_img			*img_cpy(t_img *dest, t_img *src, int width, int height);
t_img			*img_dup(void *mlx_ptr, t_img *img, int width, int height);

/***COLORS***/
int				*set_color_wheel(int num_colors, double saturation, double lightness, int base_hue);
unsigned int	get_color_source(double x, double y, int *colors, int num_colors, int rot);// add linear interpolation here!!!!
unsigned int	interpolate_two_colors(int color1, int color2, double t);


/****THREADING TOOLS****/
int				get_num_cores(void);



/******IMAGE PROCESSING FUNCTIONS******/

void		downsample(long width, long height, t_img *img, t_img *img_big, int kern_size);
int			gaussian_downsample(long width, long height, t_img *img, t_img *img_big, int kern_size, double sigma);//fix this now that pads are correct.
void		convo_down_filter_section(t_img *img, t_img *img_big, t_filter *f);
t_img		combine_images(t_img *img1, t_img *img2, int width, int height, void *mlx_connection);//needs working on

void	downsample_xl(int width, int height, t_img *img, unsigned int **pixels_xl, int kern_size);


int			mean_convo_filter(void *mlx_ptr, t_img *img, int width, int height, int kern_size);
int			gaussian_convo_filter(void *mlx_ptr, t_img *img, int width, int height, int kern_size, double sigma);
int			gaussmirror_convo_filter(void *mlx_ptr, t_img *img, int width, int height, int kern_size, int layers, double sigma);
t_img	*sobel(void *mlx_ptr, t_img *img, int kern_size, int width, int height);//change sobel like gauss whenits done

int			non_local_means(void *mlx_ptr, t_img *img, int width, int height, int region_size, double sigma);//ret -1 on failure



//padding utils
t_img		*loop_reflector(void *mlx_ptr, t_img *img, int *width, int *height, int layers, int iters);
t_img		*pad_img(void *mlx_ptr, t_img *img, int *height, int *width, int num_layers, t_op op);
t_img		*pad_zeros(void *mlx_ptr, t_img *img, int width, int height, int layers);
t_img		*pad_color(void *mlx_ptr, t_img *img, int width, int height, int layers, unsigned int color);
t_img		*pad_mirror(void *mlx_ptr, t_img *img, int width, int height, int layers);
t_img	*pad_mirror_circle(void *mlx_ptr, t_img *img, int width, int height, int layers, int axis_radius);
t_img	*loop_reflector_circle(void *mlx_ptr, t_img *img, int *width, int *height, int layers, int axis_radius, int iters);
unsigned int **pm_circle_matrix(unsigned int **in, int width, int height, int layers, int axis_radius);



double		**pad_mirror_kernel(t_filter *f, int width, int height, int layers);
void		copy_center(t_img *img, t_img *img_pad, int width, int height, int layers);

//processing utils
void		filter_section(t_img *img, t_img *img_new, t_filter *f);
void		apply_convo_filter(t_filter *f, t_img *img_z, t_img *img_out, int width, int height);
int			malloc_kernel(t_filter *f, int kern_size);
void		free_kernel(double **kernel, int j);

//matrix utils
double			get_matrix_sum(double **matrix, int width, int height);
double			**malloc_matrix(int width, int height);
unsigned char	**malloc_uc_matrix(int width, int height);
unsigned int	**malloc_ui_matrix(int width, int height);
void			zero_ui_matrix(unsigned int **pixels_xl, int width, int height);
void			free_ui_matrix(unsigned int **matrix, int j);




//--------make kernels
int			make_gaussian(t_filter *f, int kern_size, double sigma);

//ADJUST PIXELS.
void		adjust_pixels_rgb(t_img *img, int width, int height, double times, double plus, t_channel c);

/******EXPORT******/
int			export_png(const char *filename, t_img *img, int width, int height, png_text *text);

//export utils
char 		*get_nxt_name(char *name);
void		get_pixel(t_pixel *pix_t, t_img *img, int x, int y);
int			set_png_pixels(t_png_io *png_img, t_img *img, int height, int width);
int			init_png_structs(t_png_io *png_img, const char *filename);

// export/import utils
int			error_1(t_png_io *png_img, const char *msg);
void		free_png_rows(png_structp png_ptr, png_byte **row_pointers, int j);
void		clean_memory(t_png_io *png_img, int j, bool export);
void		init_vars(t_png_io *png_img);

/*****IMPORT******/
t_img		*import_png(void *mlx_ptr, const char *filename, int *width, int *height);

//import utils
void		*error_2(t_png_io *png_img, const char *msg);
int			error_3(t_png_io *png_img, const char *msg);

/****PRINTING UTILS****/
void		print_matrix(double **matrix, int cols, int rows);

#endif