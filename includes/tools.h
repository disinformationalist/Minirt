#ifndef TOOLS_H
# define TOOLS_H

# include "minirt.h"

typedef struct s_vec2
{
	double x;
	double y;
}	t_vec_2;

typedef struct s_vec3
{
	double x;
	double y;
	double z;
}	t_vec3;

/***SINGLE OBJECTS***/

typedef struct s_amb
{
	double	ratio;
	t_color	amb_color;
}	t_amb;

typedef struct s_cam
{
	t_vec3	center;
	t_vec3	orient;
	int		fov;
}	t_cam;

typedef struct s_light
{
	t_vec3 center;
	double	brightness;
	t_color	light_color;//rbg colors t_color for the bones
}	t_light;



#endif