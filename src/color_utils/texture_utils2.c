#include "minirt.h"

static inline t_norm_color	pixel_color_get2(int x, int y, t_img *img)
{
	unsigned int	pixcolor;
	t_norm_color	pixel_color;
	int				offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	pixcolor = *(unsigned int *)(img->pixels_ptr + offset);
	pixel_color.r = (pixcolor >> 16) & 0xFF;
	pixel_color.g = (pixcolor >> 8) & 0xFF;
	pixel_color.b = pixcolor & 0xFF;
	return (pixel_color);
}

t_norm_color	texture_cy_at(t_point obj_pnt, t_cylinder cyl, t_comps *comps)
{
	t_norm_color	col;
	t_position		pos;
	t_position		dims;
	t_map			map;

	dims.i = cyl.texture->i_width;
	dims.j = cyl.texture->i_height;
	map = cylinder_map(obj_pnt, dims.i == dims.j * 2, \
	comps->is_top, comps->is_bot);
	map.v = 1 - map.v;
	pos.i = ft_round(map.u * (double)(dims.j * 2 - 1));
	pos.j = ft_round(map.v / 2 * (double)(dims.j - 1));
	col = pixel_color_get2(pos.i, pos.j, cyl.texture->image);
	comps->dims = dims;
	comps->pos = pos;
	comps->map = map;
	return (col);
}

t_norm_color	texture_hy_at(t_point obj_pnt, t_hyperboloid hy, t_comps *comps)
{
	t_norm_color	col;
	t_position		pos;
	t_position		dims;
	t_map			map;

	dims.i = hy.texture->i_width;
	dims.j = hy.texture->i_height;
	map = hyperbolic_map(obj_pnt, dims.i == dims.j * 2, comps);
	map.v = 1 - map.v;
	pos.i = ft_round(map.u * (double)(dims.j * 2 - 1));
	pos.j = ft_round(map.v / 2 * (double)(dims.j - 1));
	col = pixel_color_get2(pos.i, pos.j, hy.texture->image);
	comps->dims = dims;
	comps->pos = pos;
	comps->map = map;
	return (col);
}
