#ifndef TYPES_H
# define TYPES_H

# include <stdint.h>

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
	double	w;
}	t_vec3;

typedef t_vec3	t_point;
typedef	t_vec3	t_quat;

typedef struct s_img
{
	void	*img_ptr;
	char	*pixels_ptr;
	int		bpp;
	int		endian;
	int		line_len;
}	t_img;

typedef struct s_norm_color
{
	double	r;
	double	g;
	double	b;
}	t_norm_color;

typedef struct s_color
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
}	t_color;


#endif