#include "minirt.h"

void	write_amb(t_amb *amb, int fd)
{
	int		r;
	int		g;
	int		b;
	char 	line[200];
	double	ratio;

	if (amb == NULL)
		return ;
	ft_putstr_fd("#Ambient:   Intensity 																      R | G | B\n", fd);
	ratio = amb->ratio;
	r = round((1.0 / ratio) * amb->color.r * 255.0);
	g = round((1.0 / ratio) * amb->color.g * 255.0);
	b = round((1.0 / ratio) * amb->color.b * 255.0);
	snprintf(line, sizeof(line), "A           %.3f                                                                         %d,%d,%d", ratio, r, g, b);
	write(fd, line, ft_strlen(line));
	write(fd, "\n\n\n", 3);
}

void	write_cam(t_cam *cam, int fd)
{
	char 	line[200];
	t_point	cen;
	t_vec3	ori;
	int 	spaces;
	int		spaces2;

//	must track upward vector later when bones is made
	cen = cam->center;
	ori = cam->orient;
	ft_putstr_fd("#Camera:    Cen_x | Cen_y | Cen_z      Ori_x | Ori_y | Ori_z                 FOV\n", fd);

	spaces = 13 - count_chars(cen.x) - count_chars(cen.y) - count_chars(cen.z); 
	spaces2 = 24 - count_chars(ori.x) - count_chars(ori.y) - count_chars(ori.z); 

	snprintf(line, sizeof(line), "C           %.3f,%.3f,%.3f%*s%.3f,%.3f,%.3f%*s%d", cen.x, cen.y, cen.z, spaces, "", ori.x, ori.y, ori.z, spaces2, "", cam->fov);
	write(fd, line, ft_strlen(line));
	write(fd, "\n\n\n", 3);
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
	write_amb(trace->amb, fd);
	write_cam(trace->cam, fd);
	write_lights(trace->lights, fd);
	write_splights(trace->lights, fd);
	write_arealights(trace->lights, fd);
	write_spheres(trace->spheres, fd);
	write_planes(trace->planes, fd);
	write_cylinders(trace->cylinders, fd);
	write_cubes(trace->cubes, fd);
	write_lenses(trace->lenses, fd);
	write_textures(trace->textures, fd);
	close(fd);
}