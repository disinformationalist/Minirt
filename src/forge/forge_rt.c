#include "minirt.h"

//utility to prevent "-0" truncation error.

void	check_tolerance(t_vec3 *vec)
{
	if (fabs((*vec).x) < 1e-6)
		(*vec).x = 0.0;
	if (fabs((*vec).y) < 1e-6)
		(*vec).y = 0.0;
	if (fabs((*vec).z) < 1e-6)
		(*vec).z = 0.0;
}

void	write_amb(t_amb *amb, int fd)
{
	int		r;
	int		g;
	int		b;
	char	line[200];
	double	ratio;

	if (amb == NULL)
		return ;
	ft_putstr_fd("#Ambient:   Intensity 							"\
		"									      R | G | B\n", fd);
	ratio = amb->ratio;
	if (ratio == 0)
		ratio = 1e-8;
	r = round((1.0 / ratio) * amb->color.r * 255.0);
	g = round((1.0 / ratio) * amb->color.g * 255.0);
	b = round((1.0 / ratio) * amb->color.b * 255.0);
	snprintf(line, sizeof(line), "A           %.3f                      "\
		"                                       "
		"            %d,%d,%d", ratio, r, g, b);
	write(fd, line, ft_strlen(line));
	write(fd, "\n\n\n", 3);
}

/* for correct behavior in all cases,must track upward vector
as well and set in rt_file */

void	write_cam(t_cam *cam, int fd)
{
	char	line[300];
	t_point	cen;
	t_vec3	ori;
	t_vec3	up;
	int		sp;
	int		sp2;
	int		sp3;

	cen = cam->center;
	ori = cam->orient;
	up = cam->true_up;

	ft_putstr_fd("#Camera:    Cen_x | Cen_y | Cen_z      "
		"Ori_x | Ori_y | Ori_z      "
		"Up_x | Up_y | Up_z         FOV\n", fd);

	sp = 13 - count_chars(cen.x) - count_chars(cen.y) - count_chars(cen.z);
	sp2 = 13 - count_chars(ori.x) - count_chars(ori.y) - count_chars(ori.z);
	sp3 = 13 - count_chars(up.x) - count_chars(up.y) - count_chars(up.z);

	snprintf(line, sizeof(line),
		"C           %.3f,%.3f,%.3f%*s"
		"%.3f,%.3f,%.3f%*s"
		"%.3f,%.3f,%.3f%*s"
		"%d",
		cen.x, cen.y, cen.z, sp, "",
		ori.x, ori.y, ori.z, sp2, "",
		up.x, up.y, up.z, sp3, "",
		cam->fov);

	write(fd, line, ft_strlen(line));
	write(fd, "\n\n\n", 3);
}

void	write_key(int fd)
{

ft_putstr_fd("#Material and option string usage key:\n\n\
#A D S H R T I B Q P O F (C S) Xf\n\n\
#A = ambient, 00–100 \n\
#D = diffuse, 00–100 \n\
#S = specular, 00–100 \n\
#H = shine, 000–1000 \n\
#R = reflective, 00–100 \n\
#T = transparent, 00–100 \n\
#I = refractive index, store (value - 1.0) * 100 → 000–400 \n\
#B = bump level, 000–100 \n\
#Q = object casts shadow, 0 or 1 \n\
#P = bump enabled, 0 or 1 \n\
#O = which option active(color, texture, checker)  0,1,2 \n\
#F = frost 0 or 1 \n\n\
#W = wave(planes only) \n\
#C = caps for cylinder and hyperboloid \n\
#S = single or double for the hyperboid/cone 0 0r 1 \n\n\
#Xf = used to store base64 packed transform matrices(rottran and scale, others are derived from these at runtime), \n\n\n", fd);
}

void	forge_rt(const char *path, t_trace *trace)
{
	int			fd;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("Error creating file");
		return ;
	}
	write_key(fd);
	write_amb(trace->amb, fd);
	write_cam(trace->cam, fd);
	write_lights(trace->lights, fd);
	write_splights(trace->lights, fd);
	write_arealights(trace->lights, fd);
	write_spheres(trace->spheres, fd);
	write_planes(trace->planes, fd);
	write_cylinders(trace->cylinders, fd);
	write_hyperboloids(trace->hyperboloids, fd);
	write_cubes(trace->cubes, fd);
	write_textures(trace->textures, fd);
	close(fd);
}
