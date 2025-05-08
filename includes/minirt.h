#ifndef MINIRT_H
# define MINIRT_H

# include "tools.h"
# include "keyboard.h"
# include <sys/time.h>
# include "extras.h"

//# define ASPECT (16.0 / 9.0)
# define ASPECT 1.7778
# define DEFAULT_BUMP 30
# define PI_HALVES 1.5708
# define FIVE_PI_SIXTHS 2.618
# define TWO_PI 6.283185
# define FOUR_PI_THIRDS 4.1888
# define PI_SIXTHS 0.5236
# define PI_FIFTHS 0.628319
# define PI_12THS 0.2618

/***DOUBLY LINKED CIRCULAR LISTS OBJECTS***/

typedef struct s_sphere
{
	t_type			type;
	int				id;
	bool			shadow;
	bool			bump;
	double			bump_level;
	double			fuzz_lev;
	t_point			center;
	double			radius;
	t_norm_color	color;
	t_mat			mat;
	t_mat4			transform;
	t_mat4			t_transform;
	t_mat4			i_transform;
	t_mat4			curr_scale;
	t_mat4			curr_rottran;
	t_vec3			rots;
	t_tx			*texture;
	t_pattern		pattern;
	int				option;
	bool			w_frost;
	bool			is_box;
	struct s_sphere	*next;
	struct s_sphere	*prev;
}	t_sphere;

typedef struct s_plane
{
	t_type			type;
	int				id;
	bool			shadow;
	bool			bump;
	double			bump_level;
	double			fuzz_lev;
	t_point			point;
	t_vec3			norm;
	t_norm_color	color;
	t_mat			mat;
	t_mat4			transform;
	t_mat4			t_transform;
	t_mat4			i_transform;
	t_mat4			curr_scale;
	t_mat4			curr_rottran;
	t_vec3			rots;
	t_tx			*texture;
	t_pattern		pattern;
	int				option;
	bool			sine;
	bool			w_frost;
	double			amp;
	struct s_plane	*next;
	struct s_plane	*prev;
}	t_plane;

typedef struct s_cylinder
{
	t_type				type;
	int					id;
	bool				caps;
	bool				shadow;
	bool				bump;
	double				bump_level;
	double				fuzz_lev;
	t_point				center;
	t_vec3				norm;
	double				radius;
	double				height;
	t_norm_color		color;
	t_mat				mat;
	t_mat4				transform;
	t_mat4				t_transform;
	t_mat4				i_transform;
	t_mat4				curr_scale;
	t_mat4				curr_rottran;
	t_vec3				rots;
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
	bool					caps;
	bool					shadow;
	bool					bump;
	double					bump_level;
	double					fuzz_lev;
	t_point					center;
	t_vec3					norm;
	double					rad;
	double					height;
	double					half_h;
	double					waist_val;
	double					waist3;
	t_norm_color			color;
	t_mat					mat;
	t_mat4					transform;
	t_mat4					t_transform;
	t_mat4					i_transform;
	t_mat4					curr_scale;
	t_mat4					curr_rottran;
	t_vec3					rots;
	t_tx					*texture;
	t_pattern				pattern;
	bool					w_frost;
	bool					single;
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
	double			bump_level;
	double			fuzz_lev;
	t_point			center;
	t_vec3			norm;
	double			h_width;
	double			h_height;
	double			h_depth;
	t_norm_color	color;
	t_mat			mat;
	t_mat4			transform;
	t_mat4			t_transform;
	t_mat4			i_transform;
	t_mat4			curr_scale;
	t_mat4			curr_rottran;
	t_vec3			rots;
	t_tx			*texture;
	t_pattern		pattern;
	int				option;
	bool			emitter;
	bool			sine;
	bool			w_frost;
	double			bright;
	double			amp;
	struct s_cube	*prev;
	struct s_cube	*next;
}	t_cube;

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
	double				in_angle;
	double				out_angle;
	double				inv_conediff;
	t_point				corner;
	t_vec3				v1;
	t_vec3				v2;
	t_vec3				uvec;
	t_vec3				vvec;
	int					usteps;
	int					vsteps;
	int					samples;
	t_mat4				transform;
	t_mat4				curr_scale;
	t_mat4				curr_rottran;
	t_vec3				rots;
	t_cube				*emitter;
	double				area;
	struct s_light		*prev;
	struct s_light		*next;
}	t_light;

/***MAIN STRUCT***/

typedef struct s_trace
{
	t_group			*group;
	t_group			*bvh;
	t_box			*box;
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
	t_sphere		*curr_sp;
	t_plane			*curr_pl;
	t_cylinder		*curr_cy;
	t_hyperboloid	*curr_hy;
	t_cube			*curr_cu;
	t_light			*curr_lt;
	void			*mlx_connect;
	void			*mlx_win;	
	bool			layer;
	t_norm_color	*w_colors;
	t_norm_color	*m_colors;
	int				color_i;
	int				num_colors;
	int				sl_count;
	int				al_count;
	int				height;
	int				width;
	double			pixel_width;
	double			pixel_height;
	t_point			pixel00;
	t_vec3			pix_delta_down;
	t_vec3			pix_delta_rht;
	bool			supersample;
	double			n;
	t_vec3			move_x;
	t_vec3			move_y;
	double			n2;
	int				num_cols;
	int				num_rows;
	pthread_t		*threads;
	bool			shift_on;
	bool			low_res;
	int				low_inc;
	bool			dragging;
	int				start_x;
	int				start_y;

	double			start_xangle;
	double			start_yangle;
	double			start_zangle;
	double			m_raydist;

	bool			low_flag;
	int				m_lowinc;
	bool			menu_open;
	bool			stash;
	t_control		*obj_control;
	bool			on_menu;
	int				knob;

	bool			sp_box;
}	t_trace;

//thread info

typedef struct s_piece
{
	int				x_s;
	int				x_e;
	int				y_s;
	int				y_e;
	t_trace			*trace;
	t_intersects	*intersects;

	bool			bridge;
	int				x_s2;
	int				x_e2;
	int				y_s2;
	int				y_e2;

	unsigned int	thread_color;
}	t_piece;

typedef struct t_comps
{
	double			t;
	double			sqlt_int;
	t_vec3			point;
	t_vec3			eyev;
	t_vec3			normal;
	t_vec3			norm2;
	t_vec3			light_dir;
	t_ray			ray;
	t_vec3			reflectv;
	t_mat			mat;
	double			cos_a;
	bool			inside;
	double			spot_int;
	double			n1;
	double			n2;
	t_vec3			over_pnt;
	t_vec3			under_pnt;
	t_vec3			bump;
	t_map			map;
	bool			is_top;
	bool			is_bot;
	t_face			face;
	t_vec2			dirs;
	t_position		pos;
	t_position		dims;
	t_norm_color	color;
	t_norm_color	refr_col;
	t_norm_color	refl_col;
}	t_comps;

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
int				check_double(char **ratio_str, double lower_lim, \
				double upper_lim);
int				check_color(char *color_str);
int				check_fov(char *fov_str);
int				check_orientation(char *orient_str);
int				check_coordinates(char *coord_str);
void			free_exit(char ***rt_file, char *msg1, char *msg2);

/***INIT***/

void			trace_init(t_trace *trace);
void			init_viewing(t_trace *trace);
void			reinit_viewing(t_trace *trace);

//get and set vals

t_vec3			get_coordinates(char *coord_str, double w);
t_norm_color	get_color(char *color_str, double val);
double			get_double(char **str);
bool			set_amb(t_amb **amb, char **line);
bool			set_cam(t_cam **cam, char **line);
bool			set_light(t_light **light, char **line);
bool			set_empty_amb(t_amb **amb);

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
bool			insert_spcopy_after(t_trace *trace, t_sphere **current, bool flag);
bool			insert_plcopy_after(t_trace *trace, t_plane **current, bool flag);
bool			insert_cycopy_after(t_trace *trace, t_cylinder **current, bool flag);
bool			insert_hycopy_after(t_trace *trace, t_hyperboloid **current, bool flag);
bool			insert_ltcopy_after(t_trace *trace, t_light **current);
bool			insert_cucopy_after(t_trace *trace, t_cube **current, bool flag);
bool			insert_ltcopy_after2(t_trace *trace, t_light **current);
void			adj_pntrs_copy2(t_light *to_copy, t_light *new);
bool			insert_ltcopy_after3(t_trace *trace, t_light **current);

//remove a list object
void			pop_sp(t_trace *trace, t_sphere **current);
void			pop_cy(t_trace *trace, t_cylinder **current);
void			pop_hy(t_trace *trace, t_hyperboloid **current);
void			pop_pl(t_trace *trace, t_plane **current);
void			pop_lt(t_trace *trace, t_light **current);
void			pop_cu(t_trace *trace, t_cube **current);
void			make_default_cu(t_cube **start, t_cube *new);

/***RENDER FUNCTIONS***/

void			render_scene(t_trace *trace);
void			render(t_trace *trace);
void			*ray_trace(void *arg);

//super
void			*ray_trace_s(void *arg);

//low resolution
void			*ray_trace_l(void *arg);

//thread
void			join_threads(t_trace *trace);
t_intersects	*create_ints(int total);
void			thread_error(t_trace *trace, int i);
int				set_pieces(t_trace *trace, \
				t_piece piece[][trace->num_cols], int i, int j);
void			free_closests(t_trace *trace, \
				t_piece piece[][trace->num_cols], int i, int j);

//init transforms
void			set_sp_transforms(t_trace *trace);
void			set_pl_transforms(t_trace *trace);
void			set_cy_transforms(t_trace *trace);
void			set_hy_transforms(t_trace *trace);
void			set_cu_transforms(t_trace *trace);

//mlx utils
int				new_img_init(void *mlx_con, t_img *img, int width, int height);
void			my_pixel_put(int x, int y, t_img *img, unsigned int color);
t_img			*create_img(void *mlx_ptr, int width, int height);

//intersect
t_norm_color	check_intersects(t_trace *trace, t_ray r, \
				t_intersects *intersects, t_depths depths);
void			find_closest(t_trace *trace, t_ray ray, \
				t_intersects *intersects);

//sphere utils
void			ray_sphere_intersect(t_sphere *sphere, \
				t_ray ray, t_intersects *intersects);
void			check_spheres(t_sphere *spheres, \
				t_intersects *intersects, t_ray ray);
t_norm_color	color_sphere(t_trace *trace, t_ray r, \
				t_intersects *intersects, t_depths depths);
void			ray_sphere_intersect(t_sphere *sphere, \
				t_ray ray, t_intersects *intersects);
bool			append_sp_box(t_trace *trace, t_sphere **start);



//sp shadow
bool			check_sp_dist(t_sphere *spheres, t_ray ray, double dist);

//plane utils
void			check_planes(t_plane *planes, \
				t_intersects *intersects, t_ray ray);
void			ray_plane_intersect(t_plane *plane, \
				t_ray ray, t_intersects *intersects);
t_norm_color	color_plane(t_trace *trace, \
				t_ray r, t_intersects *intersects, t_depths depths);

//cube utils
void			check_cubes(t_cube *cubes, t_intersects *intersects, t_ray ray);
void			ray_cube_intersect(t_cube *cube, \
				t_ray ray, t_intersects *intersects);
t_norm_color	color_cube(t_trace *trace, t_ray r, \
				t_intersects *intersects, t_depths depths);

//obj shadow
bool			obscured_b(t_trace *trace, t_point lt_pos, t_comps comps);
//bool			check_pl_dist(t_plane *planes, t_ray ray, double dist);
bool			ray_cylinder_intersect2(t_cylinder cylinder, \
				t_ray ray, double dist);
bool			check_hy_dist(t_hyperboloid *hyperboloids, \
				t_ray ray, double dist);

bool			check_group_dist(t_group *group, double dist, t_ray ray);
bool			check_hier_dist(t_group *top, double dist, t_ray ray);



//cylinder utils
void			check_cylinders(t_cylinder *cylinders, \
				t_intersects *intersects, t_ray ray);
t_norm_color	color_cylinder(t_trace *trace, t_ray r, \
				t_intersects *intersects, t_depths depths);
void			ray_cylinder_intersect(t_cylinder *cylinder, \
				t_ray ray, t_intersects *intersects);

//hyperboloid utils
void			check_hyperboloids(t_hyperboloid *hyperboloids, \
				t_intersects *intersects, t_ray ray);
t_norm_color	color_hyperboloid(t_trace *trace, t_ray r, \
				t_intersects *intersects, t_depths depths);
void			ray_hype_intersect(t_hyperboloid *hype, \
				t_ray r, t_intersects *intersects, bool flag);

//light utils
void			set_arealt(t_light *lt);
void			check_arealts(t_light *lights, \
				t_intersects *intersects, t_ray ray);
int				set_al_vals(t_trace *trace, t_light *new, char **line);
void			update_light_ids(t_light *light);

// reflect, refract
void			handle_light(t_trace *trace, t_comps *comps, \
				t_norm_color *lt_color, t_light *curr_lt);
double			schlick(t_comps comps);
void			set_indicies(t_intersects *intersects, double *n1, double *n2);
t_norm_color	get_reflected(t_trace *trace, t_comps comps, \
				t_intersects *intersects, t_depths depths);
t_norm_color	get_refracted(t_trace *trace, t_comps comps, \
				t_intersects *intersects, t_depths depths);

//view
void			reinit_viewing(t_trace *trace);

/***MATH UTILS***/

double			magnitude(t_vec3 vec);
double			dot_product(t_vec3 vec1, t_vec3 vec2);
t_vec3			vec(double x, double y, double z, double w);
t_vec3			add_vec(t_vec3 vec1, t_vec3 vec2);
t_vec3			subtract_vec(t_vec3 vec1, t_vec3 vec2);
t_vec3			scale_vec(double scalar, t_vec3 vec);
t_vec3			div_vec(double scalar, t_vec3 vec);
t_vec3			norm_vec(t_vec3 vec);
t_vec3			cross_prod(t_vec3 vec1, t_vec3 vec2);
t_vec3			neg(t_vec3 vec);
t_vec3			mult_vec(t_vec3 v1, t_vec3 v2);
t_ray			ray(t_vec3 dir, t_point origin);
t_mat4			rot_to(t_vec3 from, t_vec3 to);
t_mat4			get_rotation(t_vec3 ax, double cos, double sin);
bool			veccmp(t_vec3 v1, t_vec3 v2);

/***COLOR UTILS***/

t_norm_color	*set_metal_colors(void);
t_norm_color	get_final_color4(t_trace *trace, t_comps comps,
					t_norm_color lt_color);
t_norm_color	color(double r, double g, double b);
uint8_t			clamp_color(double color);
unsigned int	clamped_col(t_norm_color col);
int				ft_round(double num);
t_norm_color	color(double r, double g, double b);

//texture utils

t_norm_color	texture_plane_at(t_point obj_pnt, t_plane plane, \
				t_comps *comps);
t_norm_color	texture_sp_at(t_point obj_pnt, t_sphere sphere, t_comps *comps);
t_norm_color	texture_cy_at(t_point obj_pnt, t_cylinder cyl, t_comps *comps);
t_norm_color	texture_cube_at(t_point obj_pnt, t_cube cube, \
				t_comps *comps, t_face face);
t_norm_color	texture_hy_at(t_point obj_pnt, t_hyperboloid hy,
					t_comps *comps);
t_vec2			set_plane_uv(t_point obj_pnt, double img_iasp);
t_norm_color	uv_pattern_at(t_pattern check, t_vec2 uv);
int				import_textures(void *mlx_con, t_tx *textures);
void			sine_ring_norm(t_point obj_pnt, t_comps *comps, \
				t_mat4 t_tran, t_mat4 i_tran, double amp);
void			sine_ring_norm_cu(t_point obj_pnt, t_comps *comps, \
				t_mat4 t_tran, t_mat4 i_tran, double amp);
t_vec3			frost(t_vec3 norm, double strength);
t_map			hyperbolic_map(t_point obj_pnt, bool flag, \
				t_comps *comps);

//bump
void			bump_pl(t_point obj_pnt, t_plane plane, t_comps *comps);
void			bump_sp(t_point obj_pnt, t_sphere sphere, t_comps *comps);
void			bump_cy(t_point obj_pnt, t_cylinder cyl, t_comps *comps);
void			bump_hy(t_point obj_pnt, t_hyperboloid hyp, t_comps *comps);
void			bump_cu(t_point obj_pnt, t_cube cube, \
				t_comps *comps, t_face face);
t_img			*build_lumin_map(void *mlx_con, t_img *img, \
				int width, int height);

//materials
void			change_mat(t_trace *trace, t_on *on, const t_mat mat);
t_mat			get_mat(t_material material);

//supersampling utils
unsigned int	avg_samples(t_norm_color sum, double n);
t_norm_color	sum_rgbs(t_norm_color sum, t_norm_color to_add);
t_norm_color	mult_color(double scalar, t_norm_color color);

//patterns
t_norm_color	ring_at(t_point point, t_mat4 transform);
t_norm_color	gradient_at(t_point point, t_mat4 transform, \
				t_norm_color col1, t_norm_color col2);

/***EVENTS***/
void			build_group(t_trace *trace);
void			rebuild_hierarchy(t_trace *trace);
int				key_press(int keycode, t_trace *trace);
void			print_guide(void);
void			print_gui_guide(void);
int				close_win(t_trace *trace);
void			frost_on(t_trace *trace, t_on on);
void			toggle_shadow(t_trace *trace, t_on *on);
void			toggle_bump(t_trace *trace, t_on *on);
void			toggle_sine(t_trace *trace);
void			toggle_lowres(t_trace *trace);
void			toggle_caps(t_trace *trace);
void			rotate_colors(t_trace *trace, int button, t_norm_color *curr);
void			scale_object(t_trace *trace, t_on *on, t_vec3 vec, int keycode);
void			rotate_object(t_trace *trace, t_on *on, t_mat4 rot, t_vec3 add, bool flag);
void			translate_object(t_trace *trace, t_on *on, t_vec3 vec, bool flag);
void			pop_object(t_trace *trace, t_on *on);
void			push_new_object(t_trace *trace, t_on *on);
void			push_new_object2(t_trace *trace, t_on *on);
void			adjust_super(int keycode, t_trace *trace);
int				supersample_handle(int keycode, t_trace *trace);
void			switch_list(int keycode, t_trace *trace, t_on *on);
int				mouse_press(int button, int x, int y, t_trace *trace);
void			set_next_tx(int button, t_tx *textures, t_on *on);
void			forge_or_export(int keycode, t_trace *trace);
char			*get_nxt_name_rt(char *name);
void			set_mouse_on(t_trace *trace, t_track_hits *closest);
int				mouse_move(int x, int y, t_trace *trace);
int				mouse_release(int button, int x, int y, t_trace *trace);
int				key_release(int keycode, t_trace *trace);
void			track_object(t_trace *trace, double x, double y);
void			set_obj_color(t_on *on, t_norm_color new_col);
t_norm_color	get_obj_color(t_on *on);
char			*get_obj_type(t_type type);

//traverse lists
void			switch_list(int keycode, t_trace *trace, t_on *on);
void			next_list_ob(t_trace *trace, t_on *on);
void			prev_list_ob(t_trace *trace, t_on *on);

/*****GUI CONTROLS*****/

void			reset_con(t_trace *trace);
void			reset_con_non(t_trace *trace);
void			destroy_img(t_img *img, void *mlx);
void			set_controls(t_trace *trace);
t_control		*make_controls(void *con);
void			set_menu_vals(t_trace *trace, t_on *on);
void			set_light_vals(t_trace *trace, t_on *on);
bool			insert_ltpoint(t_trace *trace, t_light **current);
void			set_lt_transforms(t_light *new);
void			set_lttype(void *con, void *win, t_light *lt);
void			set_ltknobs(t_img *img, t_control control, t_on *on);
void			set_color(void *mlx_con, void *mlx_win, t_on *on);
void			set_type(void *mlx_con, void *mlx_win, t_type type);
int				get_option(t_on *on);
t_norm_color	get_obj_color2(t_on *on);
void			controls(t_trace *trace);
void			set_con_vals(void *con, void *win, t_trace *trace);
unsigned int	pixel_color_get3(int x, int y, t_img *img);
void			set_bknob(t_img *img, t_control control, t_on *on);
void			set_rknob(t_img *img, t_control control, t_on *on);
void			set_gknob(t_img *img, t_control control, t_on *on);
void			reset_track(t_img *img, t_control control, int move_y);
void			reset_ptrack(t_img *img, t_control control, int move_x, int move_y);
t_mat			get_obj_mat(t_on *on);
void			set_pknobs1(t_img *img, t_control control, t_on *on);
void			reset_prtrack(t_img *img, t_img *track, int move_x, int move_y);
//pos
int				position_press(int x, int y, t_trace *trace, t_control control);
void			set_pos_dials(t_trace *trace);
void		 	set_posknobs(t_trace *trace, t_control control);
void			reset_postracks(t_img *img, t_control control);
void			reset_rottracks(t_img *img, t_control control);
void			set_pos(t_trace *trace);
void			set_pos_vals(void *mlx_con, void *win, t_trace *trace);
void			update_pos(t_trace *trace);
//rot
int				rotation_press(int x, int y, t_trace *trace, t_control control);
void			set_rot_dials(t_trace *trace);
void			set_rot_vals(void *mlx_con, void *win, t_trace *trace);
t_vec3			get_rot(t_on *on, t_trace *trace);
t_vec3			get_rots(t_type type, t_trace *trace);
void			set_rotpos(t_trace *trace);
t_mat4			get_rottran(t_type type, t_trace *trace);
void 			set_rotknobs(t_trace *trace, t_control control);
//sca
int				scale_press(int x, int y, t_trace *trace, t_control control);
void			set_sca_dials(t_trace *trace);
void			reset_scatrack(t_img *img, t_control control, int move_y);
void 			set_scaknobs(t_trace *trace, t_control control);
void			set_sca(t_trace *trace);
void			set_sca_vals(void *mlx_con, void *win, t_trace *trace);
t_vec3			get_scale(t_on *on, t_trace *trace);
t_vec3			get_init_scale(t_on *on, t_trace *trace);
void			gui_scale_object(t_trace *trace, t_on * on, t_vec3 scale);
void			set_waistknob(t_img *img, t_control control, t_on *on);
//--
double			get_bump_level(t_on *on);
void			set_bumpknob(t_img *img, t_control control, t_on *on);
void			set_fovknob(t_img *img, t_control control, t_on *on);
void			set_lt_widlen(t_light *lt, double wid, double len);
void			img_copyto(t_img *to, t_img *from, int xs, int ys, int width, int height);
int				set_pieces_menu(t_trace *trace, t_piece piece[][trace->num_cols]);
void			prev_list(t_trace *trace, t_on *on);
void			next_list(t_trace *trace, t_on *on);
t_vec3			quat_get_rot(t_on *on, t_trace *trace);
t_vec3			extract_rot(t_mat4 rotmat);
void			reset_window(t_trace *trace);
void			update(void *con, void *win, t_trace *trace);
void			update_no_low(void *con, void *win, t_trace *trace);


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
void			free_curr_tx(void *mlx_con, t_tx *curr);

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
void			print_cylinders(t_cylinder *cylinder);
void			print_hyperboloids(t_hyperboloid *hyperboloid);
void			print_planes(t_plane *plane);
void			print_spheres(t_sphere *spheres);
void			print_cubes(t_cube *cube);
void			print_obj_nums(t_obj_counts *counts);
void			print_3d_array(char ***array);
long			get_time(void);
void			print_times(long start, long end, char *msg);
void			cast_single_ray(t_trace *trace, t_point origin, t_vec3 dir);
void			set_thread_colors(t_trace *trace, t_piece piece[][trace->num_cols]);
void			print_threads(t_trace *trace, t_piece piece[][trace->num_cols]);
void			print_thread (t_piece piece);

#endif