#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <math.h>
# include "../minilibx-linux/mlx.h"
# include "../libft/libft.h"
 # include <X11/X.h>
# include <X11/keysym.h>
# include "../image_processing/image_processing.h"
# include "ansi_colors.h"
# include <pthread.h>
# include "tools.h"
# include "limits.h"


//# include "keyboard.h"
# include "keyboard (42).h"


typedef struct s_ray
{
	t_vec3 origin;
	t_vec3 direction;
}	t_ray;

typedef enum e_type
{
	SPHERE,
	PLANE,
	CYLINDER
} 	t_type;

typedef struct s_track_hits
{
	double	t;
	void	*object;
	t_type	object_type;
}	t_track_hits;

/***DOUBLY LINKED CIRCULAR LISTS OBJECTS***/

typedef struct s_sphere
{
	int				id;
	t_vec3			center;
	double			radius;
	t_norm_color 	color;
	struct s_sphere	*next;
	struct s_sphere *prev;
}	t_sphere;

typedef struct s_plane 
{
	int				id;
	t_vec3			point;
	t_vec3			norm_vector;
	t_norm_color	color;
	struct s_plane	*next;
	struct s_plane	*prev;
}	t_plane;

typedef struct s_cylinder
{
	int					id;
	t_vec3				center;
	t_vec3				norm_vector;
	double				radius;
	double				height;
	t_norm_color		color;
	struct s_cylinder	*prev;
	struct s_cylinder	*next;
}	t_cylinder;

/***MAIN STRUCT***/

typedef struct s_trace
{
	t_img			img;
	
	//single objects
	t_amb			*amb;
	t_cam			*cam;
	t_light			*lights;

	t_track_hits	*closest;
	
	//linked list objects
	t_sphere		*spheres;
	t_plane			*planes;
	t_cylinder		*cylinders;

	//mlx
	char			*name;
	void			*mlx_connect;
	void			*mlx_win;
	
	// move
	double			move_x;
	double			move_y;
	double			move_z;

	//other
	double			zoom;
	bool			layer;

	//dimension and view
	int				height;
	int				width;
	int 			height_orig;
	int				width_orig;
	double			aspect_r;
	double			view_width;
	double			view_height;
	double			deg_to_rad;
	double			pixel_width;
	double			pixel_height;
	t_vec3			view_topleft;
	t_vec3			pixel00;
	t_vec3			u_vec;
	t_vec3			v_vec;
	t_vec3			pix_delta_u;
	t_vec3			pix_delta_v;


	
	//for supersampling
	bool			supersample;
	unsigned int	**pixels_xl;
	int				s_kernel;
	double			n;
	
	//nums of each object type
	int				amb_count;
	int				cam_count;
	int				light_count;
	int				sphere_count;
	int				plane_count;
	int				cyl_count;

	//threading	
	int				num_cols;
	int				num_rows;
	pthread_t		*threads;
	pthread_mutex_t	mutex;

}	t_trace;

typedef struct s_piece //for threads
{
	int			x_s;
	int			x_e;
	int			y_s;
	int			y_e;
	t_trace		*trace;
}	t_piece;

/***PARSING***/

void			parse_rt(t_trace *trace, char ***rt_file);
char			***split_file(char *file);

//parse_rt_utils
void			init_obs(t_trace *trace);
void			init_counts(t_trace *trace);
void			count_check(int *item_count, char *msg, char ***rt_file);

//split_file_utils
int				strlen_no_whitespace(char *s);
char			*remove_new_line(char *line);
int				count_lines(int fd);
void			free_3d_array_i(char ***split_file, int k);
void			err_free(char ***split_file, char *msg);
int				ft_3darray_len(char ***array);

//check lines
void			check_amb(char **line, char ***rt_file);
void			check_cam(char **line, char ***rt_file);
void			check_light(char **line, char ***rt_file);
void			check_sp(char **line, char ***rt_file);
void			check_pl(char **line, char ***rt_file);
void			check_cy(char **line, char ***rt_file);

//check line utils
int				check_param_num(char **line, int num);
int				check_double(char **ratio_str, double lower_lim, double upper_lim);
int				check_color(char *color_str);
int				check_rgb_color(char **s);
int				check_fov(char *fov_str);
int				check_orientation(char *orient_str);
int				check_coordinates(char *coord_str);
void			free_exit(char ***rt_file, char *msg1, char *msg2);

/***INIT***/

//get and set vals
void			init_viewing(t_trace *trace);


t_vec3			get_coordinates(char *coord_str);
//t_color			get_color(char *color_str);
t_norm_color	get_color(char *color_str, float val);

double			get_double(char **str);

bool			set_amb(t_amb **amb, char **line);
bool			set_cam(t_cam **cam, char **line);
bool			set_light(t_light **light, char **line);

bool			append_sp(t_sphere **start, char **line);
bool			append_pl(t_plane **start, char **line);
bool			append_cy(t_cylinder **start, char **line);


//bool			insert_sp_after(t_sphere **head, char **line); //check/fix

void 			trace_init(t_trace *trace);

/***RENDER FUNCTIONS***/
void			render_scene(t_trace *trace);
void			render(t_trace *trace);
void			*ray_trace(void *arg);

//sphere utils
void			check_spheres(t_sphere *spheres, t_track_hits *closest, t_ray ray, double *t);
unsigned int	color_sphere(t_trace *trace, t_ray r, t_track_hits *closest);

//plane utils
void			check_planes(t_plane *planes, t_track_hits *closest, t_ray ray, double *t);
unsigned int	color_plane(t_trace *trace, t_ray r, t_track_hits *closest);

//cylinder utils



/***MATH UTILS***/
uint8_t			round_c(double d);
double			magnitude(t_vec3 vec);
double			dot_product(t_vec3 vec1, t_vec3 vec2);
t_vec3 			add_vec(t_vec3 vec1, t_vec3 vec2);
t_vec3			subtract_vec(t_vec3 vec1, t_vec3 vec2);
t_vec3			normalize_vec(t_vec3 vec);
t_vec3			cross_prod(t_vec3 vec1, t_vec3 vec2);
t_vec3			scalar_mult_vec(double scalar, t_vec3 vec);

/***COLOR UTILS***/
//t_color			apply_amb(t_amb *amb, t_color obj_color);
unsigned int 	get_final_color(t_trace *trace, t_norm_color color, double light_intensity);


unsigned int	get_diffuse_color(double light_intensity, t_color color);





/***EVENTS***/
int				key_press(int keycode, t_trace *trace);
int				close_win(t_trace *trace);

/***UTILS***/
char			*ft_strjoin_png(char const *s1, char const *s2);

/***CLEAN_UP***/
void			clear_all(t_trace *trace);
void			error_exit(char *msg);
void			free_sp_list(t_sphere **start);
void			free_pl_list(t_plane **start);
void			free_cy_list(t_cylinder **start);
void			free_all_objects(t_trace *trace);

/***TESTING***/
void			print_all_objects(t_trace *trace);
void			print_spheres(t_sphere *sphere);
void			print_cylinders(t_cylinder *cylinder);
void			print_planes(t_plane *plane);


#endif
