#ifndef MINIRT_H
# define MINIRT_H

# include "tools.h"
//# include "keyboard (42).h"//figure out how to automatically select correct one
# include "keyboard.h"
# include <sys/time.h>//testing speed
# include "extras.h"

# define ASPECT (16.0 / 9.0)

// holds the current closest object

typedef struct s_track_hits
{
	double	t;
	void	*object;
	t_type	object_type;
}	t_track_hits;

// gives vals for material TOFINISH

typedef struct s_mat
{
	double	amb;
	double	diff;
	double	spec;
	double	shine;
}	t_mat;

/***DOUBLY LINKED CIRCULAR LISTS OBJECTS***/ //add mats

typedef struct s_sphere
{
	int				id;
	t_point			center;
	double			radius;
	t_norm_color 	color;//color will move into mat
	t_mat			mat;
	t_matrix_4x4	transform;
	t_matrix_4x4	curr_scale;
	t_matrix_4x4	curr_rottran;
	struct s_sphere	*next;
	struct s_sphere *prev;
}	t_sphere;

typedef struct s_plane 
{
	int				id;
	t_point			point;
	t_vec3			norm;
	t_norm_color	color;
	t_mat			mat;
	t_matrix_4x4	transform;
	t_matrix_4x4	curr_scale;
	t_matrix_4x4	curr_rottran;
	struct s_plane	*next;
	struct s_plane	*prev;
}	t_plane;

typedef struct s_cylinder
{
	int					id;
	t_point				center;
	t_vec3				norm;
	double				radius;
	double				height;
	t_norm_color		color;
	t_mat				mat;
	t_matrix_4x4		transform;
	t_matrix_4x4		curr_scale;
	t_matrix_4x4		curr_rottran;
	struct s_cylinder	*prev;
	struct s_cylinder	*next;
}	t_cylinder;

typedef struct s_light
{
	t_vec3				center;
	double				brightness;
	t_ltype				type;
	int					id;
	t_point				pos;
	t_vec3				dir;
	double				inner_cone;
	double				outer_cone;
	double				inten;
	struct s_light		*prev;
	struct s_light		*next;
}	t_light;


/***MAIN STRUCT***/

typedef struct s_trace
{
	t_img			img;
	
	t_amb			*amb;
	t_cam			*cam;

	t_track_hits	*closest;
	t_on			*on;
	
	//linked list objects
	t_sphere		*spheres;
	t_plane			*planes;
	t_cylinder		*cylinders;

	t_light			*lights;

	//for tracking traversing during events can i move this?
	t_sphere		*curr_sp;
	t_plane 		*curr_pl;
	t_cylinder		*curr_cy;

	t_light			*curr_sl;

	//mlx
	void			*mlx_connect;
	void			*mlx_win;
	
	//other
	bool			layer;

	//dimension and view
	int				height;
	int				width;
	double			pixel_width;
	double			pixel_height;
	t_point			pixel00;
	t_vec3			pix_delta_down;
	t_vec3 			pix_delta_rht;
	
	//for supersampling
	bool			supersample;
	double			n;
	t_vec3			move_x;
	t_vec3			move_y;
	double			n2;
	
	//threading	
	int				num_cols;
	int				num_rows;
	pthread_t		*threads;

}	t_trace;

typedef struct s_piece //for threads
{
	int			x_s;
	int			x_e;
	int			y_s;
	int			y_e;
	t_trace		*trace;
	t_track_hits *closest;
}	t_piece;

/***PARSING***/

void			parse_rt(t_trace *trace, char ***rt_file);
char			***split_file(char *file);
char			**split_by_whitespace(char const *s);

//parse_rt_utils
void			init_obs(t_trace *trace);
void			init_counts(t_obj_counts *counts);
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
int				check_fov(char *fov_str);
int				check_orientation(char *orient_str);
int				check_normalization(char *orient_str);
int				check_coordinates(char *coord_str);
void			free_exit(char ***rt_file, char *msg1, char *msg2);
int				check_spheres_intersect(char *center_str_1, char *diam_str_1, char *center_str_2, char *diam_str_2);

/***INIT***/

void 			trace_init(t_trace *trace);
void			init_viewing(t_trace *trace);
void			reinit_viewing(t_trace *trace);

//get and set vals

t_vec3			get_coordinates(char *coord_str, double w);
t_norm_color	get_color(char *color_str, double val);
double			get_double(char **str);
bool			set_amb(t_amb **amb, char **line);
bool			set_cam(t_cam **cam, char **line);
bool			set_light(t_light **light, char **line);

bool			append_light(t_light **start, char **line);

//***add list obs***
bool			append_sp(t_sphere **start, char **line);
bool			append_pl(t_plane **start, char **line);
bool			append_cy(t_cylinder **start, char **line);


//copy and push new list obs, if empty make default
bool			insert_spcopy_after(t_trace *trace, t_sphere **current);
bool			insert_plcopy_after(t_trace *trace, t_plane **current);
bool			insert_cycopy_after(t_trace *trace, t_cylinder **current);

//remove a list object
void			pop_sp(t_trace *trace, t_sphere **current);
void			pop_cy(t_trace *trace, t_cylinder **current);
void			pop_pl(t_trace *trace, t_plane **current);

/***RENDER FUNCTIONS***/
void			render_scene(t_trace *trace);
void			render(t_trace *trace);
void			*ray_trace(void *arg);

//super
void			*ray_trace_s(void *arg);

//thread
void			join_threads(t_trace *trace);
void			thread_error(t_trace *trace, int i);
int				set_pieces(t_trace *trace, t_piece piece[][trace->num_cols], int i, int j);
void			free_closests(t_trace *trace, t_piece piece[][trace->num_cols], int i, int j);


void			set_sp_transforms(t_trace *trace);
void			set_pl_transforms(t_trace *trace);
void			set_cy_transforms(t_trace *trace);

//mlx utils
int				new_img_init(void *mlx_con, t_img *img, int width, int height);
void			my_pixel_put(int x, int y, t_img *img, unsigned int color);

//sphere utils
void			check_spheres(t_sphere *spheres, t_track_hits *closest, t_ray ray, double *t);
t_norm_color	color_sphere(t_trace *trace, t_ray r, t_track_hits *closest);
bool			ray_sphere_intersect(t_sphere sphere, t_ray r, double *t);

//plane utils
void			check_planes(t_plane *planes, t_track_hits *closest, t_ray ray, double *t);
t_norm_color	color_plane(t_trace *trace, t_ray r, t_track_hits *closest);
bool			ray_plane_intersect(t_plane plane, t_ray ray, double *t);

//cylinder utils
void			check_cylinders(t_cylinder *cylinders, t_track_hits *closest, t_ray ray, double *t);
t_norm_color	color_cylinder(t_trace *trace, t_ray r, t_track_hits *closest);
bool			ray_cylinder_intersect(t_cylinder cylinder, t_ray ray, double *t);

//light
double			get_light_int(t_vec3 norm, t_vec3 light_dir, t_vec3 view_dir);//, t_mat sphere->mat


//shadows
bool			obscured(t_trace *trace, t_point int_pnt, t_vec3 light_dir, t_vec3 normal);
bool			obscured_b(t_trace *trace, t_ray s_ray, t_point lt_pos, t_point int_pnt);//bones

//view
void			reinit_viewing(t_trace *trace);


/***MATH UTILS***/
uint8_t			round_c(double d);
double			magnitude(t_vec3 vec);
double			dot_product(t_vec3 vec1, t_vec3 vec2);
t_vec3			vec(double x, double y, double z, double w);
t_vec3 			add_vec(t_vec3 vec1, t_vec3 vec2);
t_vec3			subtract_vec(t_vec3 vec1, t_vec3 vec2);
t_vec3			scale_vec(double scalar, t_vec3 vec);
t_vec3			div_vec(double scalar, t_vec3 vec);
t_vec3			norm_vec(t_vec3 vec);
bool			is_normalized(t_vec3 vec);
t_vec3			cross_prod(t_vec3 vec1, t_vec3 vec2);
t_vec3 			neg(t_vec3 vec);
t_vec3			mult_vec(t_vec3 v1, t_vec3 v2);
t_ray			ray(t_vec3 dir, t_point origin);

t_matrix_4x4	rot_to(t_vec3 from, t_vec3 to);
t_matrix_4x4	get_rotation(t_vec3 ax, double cos, double sin);
bool			veccmp(t_vec3 v1, t_vec3 v2);

/***COLOR UTILS***/

t_norm_color	get_final_color(t_trace *trace, t_norm_color color, double light_int);

t_norm_color	color(double r, double g, double b);
uint8_t			clamp_color(double color);
int				ft_round(double num);

//used in mthread
unsigned int	avg_samples(t_norm_color sum, double n);
t_norm_color	sum_sample_rgbs(t_norm_color sum, t_norm_color to_add);

//patterns
t_norm_color	ring_at(t_point point, t_matrix_4x4 transform);
t_norm_color	stripe_at(t_point point, t_matrix_4x4 transform);
t_norm_color 	checker_at(t_point point, t_matrix_4x4 transform);//, t_norm_color color1, t_norm_color color2
t_norm_color	gradient_at(t_point point, t_matrix_4x4 transform, t_norm_color col1, t_norm_color col2);

/***EVENTS***/
int				key_press(int keycode, t_trace *trace);
int				close_win(t_trace *trace);

void			scale_object(t_trace *trace, t_on *on, t_vec3 vec);
void			rotate_object(t_trace *trace, t_on *on, t_matrix_4x4 rot);
void			translate_object(t_trace *trace, t_on *on, t_vec3 vec);
void			pop_object(t_trace *trace, t_on *on);
void			push_new_object(t_trace *trace, t_on *on);
void			adjust_super(int keycode, t_trace *trace);
int				supersample_handle(int keycode, t_trace *trace);
void			switch_list(int keycode, t_trace *trace, t_on *on);

//traverse lists
void			switch_list(int keycode, t_trace *trace, t_on *on);
void			next_list_ob(t_trace *trace, t_on *on);
void			prev_list_ob(t_trace *trace, t_on *on);

/***CLEAN_UP***/
void			clear_some(t_trace *trace);
void			clear_all(t_trace *trace);
void			error_exit(char *msg);
void			free_sp_list(t_sphere **start);
void			free_pl_list(t_plane **start);
void			free_cy_list(t_cylinder **start);
void			free_all_objects(t_trace *trace);

/***EXTRAS ***/ //extras only remain in bonus version here or in extras header.

//forge rt file, builds rt file from current scene.
void			forge_rt(const char *path, t_trace *trace);
char			*build_sp_line(t_sphere *sphere);
void			write_spheres(t_sphere *spheres, int fd);
void			write_planes(t_plane *plane, int fd);
void			write_cylinders(t_cylinder *cylinders, int fd);
int				count_chars(double n);
char			*get_nxt_name_rt(char *name);
void			forge_or_export(int keycode, t_trace *trace);
int				supersample_handle(int keycode, t_trace *trace);

/***TESTING***/
void			print_all_objects(t_trace *trace);
void			print_spheres(t_sphere *sphere);
void			print_cylinders(t_cylinder *cylinder);
void			print_planes(t_plane *plane);
void			print_obj_nums(t_obj_counts *counts);
void			print_3d_array(char ***array);
long			get_time(void);
void			print_times(long start, long end, char *msg);

#endif
