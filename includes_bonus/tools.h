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
# include "matrix_ops.h"

# include <pthread.h>
# include <png.h>

# define DEG_TO_RAD  (M_PI / 180.0)


typedef struct s_norm_color
{
	double r;
	double g;
	double b;
}	t_norm_color;

typedef struct s_vec2
{
	double x;
	double y;
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
	LENS,
	PLANE,
	CYLINDER,
	CUBE,
	LIGHT,
	GROUP,
	CAM
} 	t_type;

typedef enum e_ltype
{
	POINT,
	SPOT,
	AREA,
} 	t_ltype;

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
	uint8_t r;
	uint8_t g;
	uint8_t b;
}	t_color;

typedef struct s_on
{
	void	*object;
	t_type	type;
	int		tx_id;
	void	*prev;
	void	*next;
}	t_on;

typedef struct s_cons
{
	void			*obj;
	t_type			type;
}	t_cons;

/***SINGLE OBJECTS***/

typedef struct s_amb
{
	double	ratio;
	t_norm_color	color;
}	t_amb;

typedef struct s_cam
{
	t_vec3			center;
	t_vec3			orient;
	t_vec3			true_up;
	int				fov;
	t_matrix_4x4	transform;
	t_matrix_4x4	transform_up;
}	t_cam;

/***GROUPS AND BVH***/

//using ll for this

typedef	struct s_shape
{
	void			*shape;
	void			*parent;
	t_type			type;
	t_matrix_4x4	transform;
	struct s_shape	*next;
	struct s_shape	*prev;

}	t_shape;

typedef struct s_group
{
	t_shape			*shapes;
	t_type			type;
	t_matrix_4x4	transform;
}	t_group;

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

//parse util

typedef struct s_obj_counts
{
	int				amb_count;
	int				cam_count;
	int				light_count;
	int				sphere_count;
	int				lens_count;
	int				plane_count;
	int				cyl_count;
	int				cube_count;
	int				tx_count;
}	t_obj_counts;


//some util functions

t_norm_color	*set_color_wheel(int num_colors, double saturation, double lightness, int base_hue);
t_ray			transform(t_ray r, t_matrix_4x4 m);
void			ft_swap(double *a, double *b);
unsigned char	pixel_gray_get(int x, int y, t_img *img);



#endif