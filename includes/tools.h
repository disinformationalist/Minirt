#ifndef TOOLS_H
# define TOOLS_H

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
# include "ansi_colors.h"
# include "limits.h"
# include <stdint.h>//uint8_t

# include "matrix.h"
# include "materials.h"

# include <pthread.h>
# include <png.h>

// (M_PI / 180.0)
# define DEG_TO_RAD  0.01745329251

typedef struct s_norm_color
{
	double	r;
	double	g;
	double	b;
}	t_norm_color;

typedef struct s_vec2
{
	double	x;
	double	y;
}	t_vec2;

typedef struct s_depths
{
	int	refl;
	int	refr;
}	t_depths;

typedef struct s_position
{
	int	i;
	int	j;
}	t_position;

typedef struct s_ray
{
	t_point	origin;
	t_vec3	dir;
}	t_ray;

typedef enum e_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	HYPERBOLOID,
	CUBE,
	TRI,
	LIGHT,
	CSG,
	GROUP,
	CAM,
	MESH,
	VOID
}	t_type;

typedef struct s_triangle
{
	int		v0;
	int		v1;
	int		v2;
	t_vec3	edge1;
	t_vec3	edge2;
	t_vec3	norm;
}	t_triangle;

//mesh not currently used

typedef struct s_mesh
{
	t_vec3			*verts;
	t_triangle		*triangles;
	int				num_verts;
	int				num_tris;
	t_mat			mat;
	t_norm_color	color;
	t_type			mesh;
}	t_mesh;

typedef enum e_ltype
{
	POINT,
	SPOT,
	AREA,
}	t_ltype;

/***UTILS***/

typedef struct s_img
{
	void	*img_ptr;
	char	*pixels_ptr;
	int		bpp;
	int		endian;
	int		line_len;
}	t_img;

typedef struct s_color
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
}	t_color;

typedef struct s_on
{
	void	*object;
	t_type	type;
	void	*prev;
	void	*next;
}	t_on;

typedef struct s_cons
{
	void			*obj;
	t_type			type;
}	t_cons;

// holds the info for an intersection

typedef struct s_track_hits
{
	double	t;
	void	*object;
	t_type	object_type;
}	t_track_hits;

typedef struct s_intersects
{
	t_track_hits	*hits;
	int				count;
	int				size;
	t_track_hits	*closest;
}	t_intersects;

/***SINGLE OBJECTS***/

typedef struct s_amb
{
	double			ratio;
	t_norm_color	color;
}	t_amb;

typedef struct s_cam
{
	t_vec3			center;
	t_vec3			orient;
	t_vec3			true_up;
	int				fov;
	t_mat4			transform;
	t_mat4			transform_up;
	t_mat4			rottran;
	t_vec3			rots;
}	t_cam;

/***GROUPS AND BVH***/

typedef struct s_box
{
	t_point	min;
	t_point	max;
}	t_box;

typedef struct s_boxes
{
	t_box	*left;
	t_box	*right;
	//int		axis;//0 = x, 1 = y, 2 = z 
}	t_boxes;

//using ll for this

typedef struct s_shape
{
	struct s_shape	*shapes;
	t_type			type;
	t_mat4			tran;
	t_box			*box;

	int				count;
	int				depth;

	void			*shape;
	void			*parent;
	t_mat4			i_tran;
	struct s_shape	*next;
	struct s_shape	*prev;
}	t_shape;

typedef t_shape	t_group;

// for möller-trumbore triangle intersect ft

typedef struct s_moller
{
	double	u;
	double	v;
	double	f;
	double	det;
}	t_moller;

// util for making a color wheel

typedef struct s_wheel
{
	int		*colors;
	double	hue_step;
	double	c;
	double	x;
	double	m;
	double	r_;
	double	g_;
	double	b_;
	int		hue;
}	t_wheel;

//pattern

typedef struct s_pattern
{
	t_norm_color	color1;
	t_norm_color	color2;

	//cube test
	t_norm_color	main;
	t_norm_color	ul;
	t_norm_color	ur;
	t_norm_color	bl;
	t_norm_color	br;
	float			width;
	float			height;
}	t_pattern;

typedef struct s_map
{
	double	u;
	double	v;
	double	phi;
	double	theta;
	double	dist2;
}	t_map;

//faces for cube mapping

typedef enum e_face
{
	RIGHT,
	LEFT,
	UP,
	DOWN,
	FRONT,
	BACK
}	t_face;

//parse util

typedef struct s_obj_counts
{
	int				amb_count;
	int				cam_count;
	int				light_count;
	int				sl_count;
	int				al_count;
	int				sphere_count;
	int				plane_count;
	int				cyl_count;
	int				hyp_count;
	int				cube_count;
	int				tx_count;
	int				tri_count;
}	t_obj_counts;

//some util functions

t_norm_color	*set_color_wheel(int num_colors, double saturation, \
				double lightness, int base_hue);
t_ray			transform(t_ray r, t_mat4 m);
void			ft_swap(double *a, double *b);
double			randf(void);
double			randf2(void);
double			random_double(double min, double max);
t_vec3			rand_vec(void);
t_vec3			random_vec(double min, double max);
t_vec3			random_unit_vec();


//used for map

t_norm_color	pattern_at(t_pattern pat, t_map map);
t_pattern		uv_checker(double width, double height, \
				t_norm_color color1, t_norm_color color2);
t_map			planar_map(t_point obj_pnt);
t_map			sphere_map(t_point obj_pnt, bool flag);
t_map			cylinder_map(t_point obj_pnt, bool flag, bool top, bool bot);

//cube map
t_map			cube_map(t_point obj_pnt, t_face face);
t_vec2			cu_uv_up(t_point pnt);
t_vec2			cu_uv_left(t_point pnt);
t_vec2			cu_uv_right(t_point pnt);
t_vec2			cu_uv_back(t_point pnt);
t_vec2			cu_uv_front(t_point pnt);
t_vec2			cu_uv_down(t_point pnt);
t_face			face_of_pnt(t_point pnt);

/***GROUPS***/

t_group			*group(void);
t_group			*copy_group(t_group *in);
void			free_group(t_group *group);
int				add_child(t_group *group, void *obj, t_type type,
					t_mat4 tran, t_mat4 i_tran, t_box *box_in);
void			check_group(t_group *group, t_intersects *intersects,
					t_ray ray);

/***BVH***/

void			build_hierarchy(t_group *top);
void			print_hierarchy(t_group *top);
void			check_hierarchy(t_group *top, t_intersects *intersects, t_ray ray);
void			divide(t_group *group_in, int threshold);
bool			ray_box_intersect(t_box *box, t_mat4 ctransform, t_ray ray);
t_box			*box(t_point min, t_point max);
t_boxes			*boxes(t_box *left, t_box *right);
t_box			*empty_box(void);
void			add_pnt(t_box *box, t_point to_add);
t_box			*bounds_of(t_type type);
void			add_to(t_box *b1, t_box b2);
bool			contains_pnt(t_box box, t_point point);
bool			contains_box(t_box b1, t_box b2);
t_box			*transform_box(t_box *box, t_mat4 transf);
t_box			*bops(t_shape *shape);
t_box 			*bog(t_group *group);

#endif
