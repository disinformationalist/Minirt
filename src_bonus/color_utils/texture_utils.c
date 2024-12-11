#include "minirt.h"

t_norm_color	texture_plane_at(t_point obj_pnt, t_plane plane, t_comps *comps)
{
	t_norm_color	col;
	t_vec2			uv;
	t_position		dims;
	t_position		pos;

	dims.j = plane.texture->i_height;
	dims.i = plane.texture->i_width;
	uv = set_plane_uv(obj_pnt, plane.texture->img_iasp);
	uv.y = 1 - uv.y;
	pos.i = ft_round(((double)(dims.i - 1)) * uv.x);
	pos.j = ft_round(((double)(dims.j - 1)) * uv.y);
	col = pixel_color_get(pos.i, pos.j, plane.texture->image);
	comps->dims = dims;
	comps->pos = pos;
	return (col);
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
	col = pixel_color_get(pos.i, pos.j, cyl.texture->image);
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
	map = hyperbolic_map(obj_pnt, dims.i == dims.j * 2, \
	comps, hy.waist_val);
	map.v = 1 - map.v;
	pos.i = ft_round(map.u * (double)(dims.j * 2 - 1));
	pos.j = ft_round(map.v / 2 * (double)(dims.j - 1));
	col = pixel_color_get(pos.i, pos.j, hy.texture->image);
	comps->dims = dims;
	comps->pos = pos;
	comps->map = map;
	return (col);
}

t_norm_color	texture_sp_at(t_point obj_pnt, t_sphere sphere, t_comps *comps)
{
	t_norm_color	col;
	t_position		pos;
	t_position		dims;
	t_map			map;
	bool			flag;

	dims.i = sphere.texture->i_width;
	dims.j = sphere.texture->i_height;
	flag = (dims.i == dims.j * 2);
	map = sphere_map(obj_pnt, flag);
	map.v = 1 - map.v;
	pos.i = ft_round(map.u * (double)(dims.j * 2 - 1));
	pos.j = ft_round(map.v * (double)(dims.j - 1));
	col = pixel_color_get(pos.i, pos.j, sphere.texture->image);
	comps->dims = dims;
	comps->pos = pos;
	comps->map = map;
	return (col);
}

t_norm_color	texture_cube_at(t_point obj_pnt, t_cube cube, \
t_comps *comps, t_face face)
{
	t_norm_color	col;
	t_position		dims;
	t_position		pos;
	t_map			map;

	dims.j = cube.texture->i_height;
	dims.i = cube.texture->i_width;
	map = cube_map(obj_pnt, face);
	map.v = 1 - map.v;
	pos.i = ft_round(((double)(dims.i - 1)) * map.u);
	pos.j = ft_round(((double)(dims.j - 1)) * map.v);
	col = pixel_color_get(pos.i, pos.j, cube.texture->image);
	comps->dims = dims;
	comps->pos = pos;
	comps->map = map;
	return (col);
}
