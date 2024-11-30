#ifndef MINIRT_H
# define MINIRT_H

# include "tools.h"
//# include "keyboard (42).h"
# include "keyboard.h"
# include <sys/time.h>
# include "extras.h"

# define ASPECT (16.0 / 9.0)
//# define ASPECT (4.0 / 3.0)


/* typedef struct s_intersects
{
	t_track_hits	*hits;
	int				count;
	int				size;
	t_track_hits	*closest;
}	t_intersects; */

/***DOUBLY LINKED CIRCULAR LISTS OBJECTS***/

typedef enum e_csg_op
{
	UNION,
	INTERSECTION,
	DIFFERENCE,
}	t_csg_op;

typedef struct s_helper_shape
{
	t_type					type;
	void					*left;
	void					*right;
	t_csg_op				op;
	t_shape					*primitive;
	struct s_helper_shape	*next;
	struct s_helper_shape	*prev;
}	t_helper_shape;

typedef struct s_csg
{
	t_type		type;
	void		*left;
	void		*right;
	t_csg_op	op;
}	t_csg;

/* typedef struct s_csg_pool
{
	t_type			type;
	t_helper_shape	**shapes;
	int				shape_count;
	t_csg_op		op;
}	t_csg_pool; */

typedef struct s_sphere
{
	t_type			type;
	int				id;
	bool			shadow;
	bool			bump;
	t_point			center;
	double			radius;
	t_norm_color 	color;
	t_mat			mat;
	t_matrix_4x4	transform;
	t_matrix_4x4	t_transform;
	t_matrix_4x4	i_transform;
	t_matrix_4x4	curr_scale;
	t_matrix_4x4	curr_rottran;
	t_tx			*texture;
	t_pattern		pattern;
	int				option;
	bool			w_frost;
	struct s_sphere	*next;
	struct s_sphere *prev;
}	t_sphere;

typedef struct s_plane 
{
	t_type			type;
	int				id;
	bool			shadow;
	bool			bump;
	t_point			point;
	t_vec3			norm;
	t_norm_color	color;
	t_mat			mat;
	t_matrix_4x4	transform;
	t_matrix_4x4	t_transform;
	t_matrix_4x4	i_transform;
	t_matrix_4x4	curr_scale;
	t_matrix_4x4	curr_rottran;
	t_tx			*texture;
	t_pattern		pattern;
	int				option;
	bool			sine;
	bool			w_frost;
	struct s_plane	*next;
	struct s_plane	*prev;
}	t_plane;

typedef struct s_cylinder
{
	t_type				type;
	int					id;
	bool				shadow;
	bool				bump;
	t_point				center;
	t_vec3				norm;
	double				radius;
	double				height;
	double				half_h;
	t_norm_color		color;
	t_mat				mat;
	t_matrix_4x4		transform;
	t_matrix_4x4		t_transform;
	t_matrix_4x4		i_transform;
	t_matrix_4x4		curr_scale;
	t_matrix_4x4		curr_rottran;
	t_tx				*texture;
	int					option;
	t_pattern			pattern;
	bool				w_frost;
	struct s_cylinder	*prev;
	struct s_cylinder	*next;
}	t_cylinder;

typedef struct s_hyperboloid
{
	t_type					type;
	int						id;
	bool					shadow;
	bool					bump;
	t_point					center;
	t_vec3					norm;
	double					rad1;
	double					rad2;
	double					height;
	double					half_h;
	t_norm_color			color;
	t_mat					mat;
	t_matrix_4x4			transform;
	t_matrix_4x4			t_transform;
	t_matrix_4x4			i_transform;
	t_matrix_4x4			curr_scale;
	t_matrix_4x4			curr_rottran;
	t_tx					*texture;
	bool					w_frost;
	int						option;
	struct s_hyperboloid	*prev;
	struct s_hyperboloid	*next;
}	t_hyperboloid;

typedef struct s_cube
{
	t_type			type;
	int				id;
	bool			shadow;
	bool			bump;
	t_point			center;
	t_vec3			norm;
	double			h_width;
	double			h_height;
	double			h_depth;
	t_norm_color	color;
	t_mat			mat;
	t_matrix_4x4	transform;
	t_matrix_4x4	t_transform;
	t_matrix_4x4	i_transform;
	t_matrix_4x4	curr_scale;
	t_matrix_4x4	curr_rottran;
	t_tx			*texture;
	int				option;
	bool			emitter;
	bool			w_frost;
	double			bright;
	struct s_cube	*prev;
	struct s_cube	*next;
}	t_cube;

/* typedef struct s_helper_shape
{
	t_type				type;
	void				*left;
	void				*right;
	t_csg_op			op;
	t_shape				*primitive;
}	t_helper_shape; */


typedef struct s_tri
{
	int				id;
	bool			shadow;
	t_point			p1;
	t_point			p2;
	t_point			p3;
	t_vec3			edge1;
	t_vec3			edge2;
	t_vec3			norm;
	t_norm_color	color;
	t_mat			mat;
	struct s_tri	*prev;
	struct s_tri	*next;
}	t_tri;

//same struct for spot, area, and point lights

typedef struct s_light
{
	t_vec3				center;
	double				brightness;
	t_norm_color		color;
	t_ltype				type;
	int					id;
	t_vec3				dir;
	int					photons;
	double				inner_cone;
	double				outer_cone;
	double				inv_conediff;
	t_point				corner;
	t_vec3				v1;
	t_vec3				v2;
	t_vec3				uvec;
	t_vec3				vvec;
	int					usteps;
	int					vsteps;
	int					samples;
	t_matrix_4x4		transform;
	t_matrix_4x4		curr_scale;
	t_matrix_4x4		curr_rottran;
	t_cube				*emitter;
	double				area;
	struct s_light		*prev;
	struct s_light		*next;
}	t_light;

/***MAIN STRUCT***/

typedef struct s_trace
{	
	t_group			*group;

	int				total_ints;
	t_depths		depths;
	t_img			img;
	
	t_track_hits	*closest;
	t_on			*on;
	
	t_amb			*amb;
	t_cam			*cam;
	t_sphere		*spheres;
	t_plane			*planes;
	t_cylinder		*cylinders;
	t_hyperboloid	*hyperboloids;
	t_cube			*cubes;
	t_tx			*textures;

	t_tri			*triangles;

	t_light			*lights;

	//for tracking traversing during events can i move this?
	t_sphere		*curr_sp;
	t_plane 		*curr_pl;
	t_cylinder		*curr_cy;
	t_hyperboloid	*curr_hy;
	t_cube			*curr_cu;
	t_light			*curr_lt;

	//mlx
	void			*mlx_connect;
	void			*mlx_win;
	
	//other and color
	bool			layer;
	t_norm_color	*w_colors;
	t_norm_color	*m_colors;
	int				color_i;
	int 			num_colors;

	int				sl_count;
	int				al_count;
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

typedef struct s_piece
{
	int				x_s;
	int				x_e;
	int				y_s;
	int				y_e;
	t_trace			*trace;
	t_intersects	*intersects;
}	t_piece;

typedef struct t_comps
{
	double			t;
	double			sqlt_int;
	t_vec3			point;
	t_vec3			eyev;
	t_vec3			normal;
	t_vec3			norm2;
	t_vec3 			light_dir;
	t_ray			ray;
	t_vec3			reflectv;
	t_mat			mat;
	double			cos_a;
	bool			inside;
	double  		spot_int;
	double			n1;
	double			n2;
	t_vec3			over_pnt;
	t_vec3			under_pnt;
	t_vec3			bump;
	t_map			map;
	bool			is_top;
	bool			is_bot;
	t_position		pos;
	t_position		dims;
	t_norm_color	color;
	t_norm_color	refr_col;
	t_norm_color	refl_col;
}	t_comps;


t_norm_color	uv_pattern_at(t_pattern check, t_vec2 uv);
t_pattern		uv_align_check(t_norm_color main, t_norm_color ul, t_norm_color ur, t_norm_color bl, t_norm_color br);

/***PARSING***/

void			parse_rt(t_trace *trace, char ***rt_file);
char			***split_file(char *file);
char			**split_by_whitespace(char const *s);
void			check_ids(char ***rt_file);
void			count_ids(t_obj_counts *counts, char ***rt_file);

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
void			check_hy(char **line, char ***rt_file);
void			check_cu(char **line, char ***rt_file);
void			check_sl(char **line, char ***rt_file);
void			check_al(char **line, char ***rt_file);
void			check_tx(char **line, char ***rt_file);
void			check_tri(char **line, char ***rt_file);

//check line utils
void			check_str_len(char **line, char ***rt_file);
int				check_int(char *int_str, int min, int max);
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

//***add list obs***
bool			append_sp(t_sphere **start, char **line);
bool			append_pl(t_plane **start, char **line);
bool			append_cy(t_cylinder **start, char **line);
bool			append_hy(t_hyperboloid **start, char **line);
bool			append_cu(t_cube **start, char **line);
bool			append_tx(t_tx **start, char **line);
bool			append_light(t_trace *trace, t_light **start, char **line);
bool			append_tri(t_tri **start, char **line);

//copy and push new list obs, if empty make default
bool			insert_spcopy_after(t_trace *trace, t_sphere **current);
bool			insert_plcopy_after(t_trace *trace, t_plane **current);
bool			insert_cycopy_after(t_trace *trace, t_cylinder **current);
bool			insert_hycopy_after(t_trace *trace, t_hyperboloid **current);
bool			insert_ltcopy_after(t_trace *trace, t_light **current);
bool			insert_cucopy_after(t_trace *trace, t_cube **current);
bool			insert_ltcopy_after2(t_trace *trace);
bool			insert_ltcopy_after3(t_trace *trace);

//remove a list object
void			pop_sp(t_trace *trace, t_sphere **current);
void			pop_cy(t_trace *trace, t_cylinder **current);
void			pop_hy(t_trace *trace, t_hyperboloid **current);
void			pop_pl(t_trace *trace, t_plane **current);
void			pop_lt(t_trace *trace, t_light **current);
void			pop_cu(t_trace *trace, t_cube **current);

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

//init transforms
void			set_sp_transforms(t_trace *trace);
void			set_pl_transforms(t_trace *trace);
void			set_cy_transforms(t_trace *trace);
void			set_hy_transforms(t_trace *trace);
void			set_cu_transforms(t_trace *trace);

//mlx utils
int				new_img_init(void *mlx_con, t_img *img, int width, int height);
void			my_pixel_put(int x, int y, t_img *img, unsigned int color);

//intersect
t_norm_color	check_intersects(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths);
void			find_closest(t_trace *trace, t_ray ray, t_intersects *intersects);
void			intersect(t_intersects *intersects, void *object, double t, t_type type);

//sphere utils
void			ray_sphere_intersect(t_sphere *sphere, t_ray ray, t_intersects *intersects);
void			check_spheres(t_sphere *spheres, t_intersects *intersects, t_ray ray);
t_norm_color	color_sphere(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths);
void			ray_sphere_intersect(t_sphere *sphere, t_ray ray, t_intersects *intersects);

//sp shadow
bool			check_sp_dist(t_sphere *spheres, t_ray ray, double dist);

//plane utils
void			check_planes(t_plane *planes, t_intersects *intersects, t_ray ray);
void			ray_plane_intersect(t_plane *plane, t_ray ray, t_intersects *intersects);
t_norm_color	color_plane(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths);

//cube utils
void			check_cubes(t_cube *cubes, t_intersects *intersects, t_ray ray);
void			ray_cube_intersect(t_cube *cube, t_ray ray, t_intersects *intersects);
t_norm_color	color_cube(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths);

//obj shadow
bool			obscured_b(t_trace *trace, t_point lt_pos, t_comps comps);
bool			check_pl_dist(t_plane *planes, t_ray ray, double dist);
bool			ray_cylinder_intersect2(t_cylinder cylinder, t_ray ray, double dist);
bool			check_hy_dist(t_hyperboloid *hyperboloids, \
t_ray ray, double dist);

//cylinder utils
void			check_cylinders(t_cylinder *cylinders, t_intersects *intersects, t_ray ray);
t_norm_color	color_cylinder(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths);
void			ray_cylinder_intersect(t_cylinder *cylinder, t_ray ray, t_intersects *intersects);
bool			intersect_caps(t_ray ray, double *t3, double *t4);

//hyperboloid utils
void			check_hyperboloids(t_hyperboloid *hyperboloids, t_intersects *intersects, t_ray ray);
void			compute_abc_hy(t_vec3 *abc, t_ray ray, t_hyperboloid *hyperboloid);
t_norm_color	color_hyperboloid(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths);
void			ray_hyperboloid_intersect(t_hyperboloid *hyperboloid, t_ray ray, t_intersects *intersects);

//light utils
void			set_arealt(t_light *lt);
void			check_arealts(t_light *lights, t_intersects *intersects, t_ray ray);
int				set_al_vals(t_trace *trace, t_light *new, char **line);
void			update_light_ids(t_light *light);

// reflect, refract
void			handle_light(t_trace *trace, t_comps *comps, t_norm_color *lt_color, t_light *curr_lt);//move
double			schlick(t_comps comps);
void			set_indicies(t_intersects *intersects, double *n1, double *n2);
t_norm_color 	get_reflected(t_trace *trace, t_comps comps, t_intersects *intersects, t_depths depths);
t_norm_color	get_refracted(t_trace *trace, t_comps comps, t_intersects *intersects, t_depths depths);

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

t_norm_color	*set_metal_colors(void);
t_norm_color	get_final_color4(t_trace *trace, t_comps comps, t_norm_color lt_color);
t_norm_color	color(double r, double g, double b);
uint8_t			clamp_color(double color);
unsigned int	clamped_col(t_norm_color col);
int				ft_round(double num);
t_norm_color 	color(double r, double g, double b);

//texture utils

t_norm_color	texture_plane_at(t_point obj_pnt, t_plane plane, t_comps *comps);
t_norm_color	texture_sp_at(t_point obj_pnt, t_sphere sphere, t_comps *comps);
t_norm_color	texture_cy_at(t_point obj_pnt, t_cylinder cyl, t_comps *comps);
t_norm_color	pixel_color_get(int x, int y, t_img *img);
int				import_textures(void *mlx_con, t_tx *textures);
void			sine_ring_norm(t_point obj_pnt, t_comps *comps, t_plane plane);
t_vec3			frost(t_vec3 norm);

//bump
void			bump_pl(t_point obj_pnt, t_plane plane, t_comps *comps);
void			bump_sp(t_point obj_pnt, t_sphere sphere, t_comps *comps);
void			bump_cy(t_point obj_pnt, t_cylinder cyl, t_comps *comps);

//materials
void			change_mat(t_trace *trace,t_on *on, const t_mat mat);
t_mat			get_mat(t_material material);

//supersampling utils
unsigned int	avg_samples(t_norm_color sum, double n);
t_norm_color	sum_rgbs(t_norm_color sum, t_norm_color to_add);
t_norm_color	mult_color(double scalar, t_norm_color color);

//patterns
t_norm_color	ring_at(t_point point, t_matrix_4x4 transform);
t_norm_color	gradient_at(t_point point, t_matrix_4x4 transform, \
	t_norm_color col1, t_norm_color col2);

/***EVENTS***/
int				key_press(int keycode, t_trace *trace);
int				close_win(t_trace *trace);
void			frost_on(t_trace *trace, t_on on);
void			toggle_shadow(t_trace *trace, t_on *on);
void			toggle_bump(t_trace *trace, t_on *on);
void			rotate_colors(t_trace *trace, int button, t_norm_color *curr);
void			scale_object(t_trace *trace, t_on *on, t_vec3 vec);
void			rotate_object(t_trace *trace, t_on *on, t_matrix_4x4 rot);
void			translate_object(t_trace *trace, t_on *on, t_vec3 vec);
void			pop_object(t_trace *trace, t_on *on);
void			push_new_object(t_trace *trace, t_on *on);
void			adjust_super(int keycode, t_trace *trace);
int				supersample_handle(int keycode, t_trace *trace);
void			switch_list(int keycode, t_trace *trace, t_on *on);
int				mouse_handler(int button, int x, int y, t_trace *trace);
void			set_next_tx(int button, t_tx *textures, t_on *on);
void			forge_or_export(int keycode, t_trace *trace);
char			*get_nxt_name_rt(char *name);

//traverse lists
void			switch_list(int keycode, t_trace *trace, t_on *on);
void			next_list_ob(t_trace *trace, t_on *on);
void			prev_list_ob(t_trace *trace, t_on *on);

/***CLEAN_UP***/
void			clear_few(t_trace *trace);
void			clear_some(t_trace *trace);
void			clear_all(t_trace *trace);
void			error_exit(char *msg);
void			free_sp_list(t_sphere **start);
void			free_pl_list(t_plane **start);
void			free_cy_list(t_cylinder **start);
void			free_hy_list(t_hyperboloid **start);
void			free_tri_list(t_tri **start);
void			free_tx_list(void *mlx_con, t_tx **start);
void			free_lt_list(t_light **start);
void			free_all_objects(t_trace *trace);

/***FORGE FILE***/

void			forge_rt(const char *path, t_trace *trace);
char			*build_sp_line(t_sphere *sphere);
void			write_spheres(t_sphere *spheres, int fd);
void			write_planes(t_plane *plane, int fd);
void			write_cylinders(t_cylinder *cylinders, int fd);
void			write_hyperboloids(t_hyperboloid *hyperboloids, int fd);
void			write_lights(t_light *lights, int fd);
void			write_splights(t_light *lights, int fd);
void			write_arealights(t_light *lights, int fd);
void			write_cubes(t_cube *cubes, int fd);
void			write_textures(t_tx *textures, int fd);
int				count_chars(double n);
void			check_tolerance(t_vec3 *vec);

/***TESTING***/
void			print_all_objects(t_trace *trace);
void			print_spheres(t_sphere *sphere);
void			print_cylinders(t_cylinder *cylinder);
void			print_hyperboloids(t_hyperboloid *hyperboloid);
void			print_planes(t_plane *plane);
void			print_obj_nums(t_obj_counts *counts);
void			print_3d_array(char ***array);
long			get_time(void);
void			print_times(long start, long end, char *msg);

/*** CSG ***/
void			check_csg(t_helper_shape *shapes1, t_helper_shape *shapes2, t_helper_shape *shapes3, t_intersects *intersects);
t_csg			*make_new_csg(t_helper_shape *left, t_helper_shape *right, t_csg_op op);
bool			hit_allowed(t_csg_op op, bool lhit, bool inl, bool inr);
void			filter_intersections(t_csg *csg, t_intersects **intersects, bool *inl, bool *inr);

/*** CSG utils ***/
int				get_type(t_helper_shape *shape);
bool			is_in_csg_left(t_csg *csg, t_intersects **intersects, int i);
bool			is_in_csg_right(t_csg *csg, t_intersects **intersects, int i);
void			set_left(bool *lhit, bool *inl);
void			set_right(bool *lhit, bool *inr);

/*** CSG pool ***/
void			check_csg_sphere_list(t_trace *trace, t_csg_op op, t_intersects *intersects);
void			check_csg_cyl_list(t_trace *trace, t_csg_op op, t_intersects *intersects);
void			check_csg_hyp_list(t_trace *trace, t_csg_op op, t_intersects *intersects);
void			check_csg_cube_list(t_trace *trace, t_csg_op op, t_intersects *intersects);

#endif
