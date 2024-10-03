#include "minirt.h"

//go through sphere list adding each sphere to the file.

void	write_lenses(t_lens *lenses, int fd)
{
	t_lens			*curr_le;
	char			*line;
	
	if (lenses == NULL)
		return ;
	ft_putstr_fd("#Lens sphere_1:   Cen_x | Cen_y | Cen_z         								Diameter      		  R | G | B\n", fd);
	curr_le = lenses;
	while (true)
	{
		line = build_sp_line(&curr_le->sphere_1);
		write(fd, line, ft_strlen(line));
		line = build_sp_line(&curr_le->sphere_2);
		write(fd, line, ft_strlen(line));
		curr_le = curr_le->next;
		if (curr_le == lenses)
			break;
	}
	write(fd, "\n\n", 2);
}
