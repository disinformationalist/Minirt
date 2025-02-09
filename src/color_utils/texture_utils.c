#include "minirt.h"

// pixel_color_get() retreives color of pixel(x, y) of image

static inline t_norm_color	pixel_color_get1(int x, int y, t_img *img)
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

/* the -5, 5 in this should be based on scene size(bounding box) in the future.
planes are using this to repeat and mirror texture infinitely. */

t_vec2	set_plane_uv(t_point obj_pnt, double img_iasp)
{
	double	xmin;
	double	xmax;
	double	zmin;
	double	zmax;
	t_vec2	uv;

	xmin = -5;
	xmax = 5;
	zmin = xmin * img_iasp;
	zmax = xmax * img_iasp;
	uv.x = (obj_pnt.x - xmin) / (xmax - xmin);
	uv.y = (obj_pnt.z - zmin) / (zmax - zmin);
	if (uv.x < 0)
		uv.x = -uv.x;
	if (uv.y < 0)
		uv.y = -uv.y;
	uv.x = 1 - fabs(fmod(uv.x, 2.0) - 1.0);
	uv.y = 1 - fabs(fmod(uv.y, 2.0) - 1.0);
	return (uv);
}

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
	col = pixel_color_get1(pos.i, pos.j, plane.texture->image);
	comps->dims = dims;
	comps->pos = pos;
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
	col = pixel_color_get1(pos.i, pos.j, sphere.texture->image);
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
	col = pixel_color_get1(pos.i, pos.j, cube.texture->image);
	comps->dims = dims;
	comps->pos = pos;
	comps->map = map;
	return (col);
}
