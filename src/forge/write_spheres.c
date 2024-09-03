#include "minirt.h"

int	count_chars(int num)
{
	int	count;

	count = 1;
	if (num < 0)
	{
		count++;
		num = -num;
	}
	while (num / 10)
	{
		count++;
		num /= 10;
	}
	return (count);
}


void	add_sp_color(t_sphere *sphere, char *line, int spaces2)
{
	int r;
	int g;
	int b;

	r = sphere->color.r;
	g = sphere->color.g;
	b = sphere->color.b;

	snprintf(line + ft_strlen(line), 200 - ft_strlen(line), "%*s%d,%d,%d\n", spaces2, "", r, g, b);
}

char	*build_sp_line(t_sphere *sphere)
{
	static char 	line[200];
	t_point			cen;
	double			d;
	int 			spaces;
	int				spaces2;
	//get spaces in between to align, can pass in.
	
	cen = sphere->center;
	d = (2.0 * sphere->radius);
	spaces = 46 - count_chars(cen.x) - count_chars(cen.y) - count_chars(cen.z); 
	spaces2 = 18 - count_chars(d);
	snprintf(line, sizeof(line), "sp          %.3f,%.3f,%.3f%*s%.3f", cen.x, cen.y, cen.z, spaces, "",d);
	
	add_sp_color(sphere, line, spaces2);

	return (line);
}

//go through sphere list adding each sphere to the file.

void	write_spheres(t_sphere *spheres, int fd)
{
	t_sphere		*curr_sp;
	char			*line;
	
	if (spheres == NULL)
		return ;
	ft_putstr_fd("#Spheres:   Cen_x | Cen_y | Cen_z         								Diameter      		  R | G | B\n", fd);
	curr_sp = spheres;
	while (true)
	{
		line = build_sp_line(curr_sp);
		write(fd, line, ft_strlen(line));
		curr_sp = curr_sp->next;
		if (curr_sp == spheres)
			break;
	}
	write(fd, "\n\n", 2);
}
