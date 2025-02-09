#include "minirt.h"

static inline unsigned char	gray_val(int x, int y, t_img *img)
{
	unsigned char	gray;
	int				offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	gray = *(unsigned char *)(img->pixels_ptr + offset);
	return (gray);
}

//used for edge correction of plane bumping

static inline int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

static inline t_vec3	bumpv_cu(t_position pos, t_img *img, \
	double strength, t_position dims)
{
	t_vec3			perturb;
	unsigned char	curr;
	double			dfdx;
	double			dfdy;
	double			norm_factor;

	curr = gray_val(pos.i, pos.j, img);
	dfdx = strength * ((double)(gray_val(min(pos.i + 1, \
		dims.i - 1), pos.j, img) - curr)) / 255.0;
	dfdy = strength * ((double)(gray_val(pos.i, \
		min(pos.j + 1, dims.j - 1), img) - curr)) / 255.0;
	norm_factor = 1.0 / (sqrt(dfdx * dfdx + dfdy * dfdy + 1.0));
	perturb.x = -dfdx * norm_factor;
	perturb.z = -dfdy * norm_factor;
	perturb.y = norm_factor;
	perturb.w = 0;
	return (perturb);
}

static inline void	adjust_bumpv(t_vec3 *bumpv, t_face face)
{
	if (face == UP)
		*bumpv = *bumpv;
	else if (face == DOWN)
		*bumpv = vec(-bumpv->z, -bumpv->y, bumpv->x, 0);
	else if (face == LEFT)
		*bumpv = vec(-bumpv->y, bumpv->z, -bumpv->x, 0);
	else if (face == RIGHT)
		*bumpv = vec(bumpv->y, bumpv->z, bumpv->x, 0);
	else if (face == FRONT)
		*bumpv = vec(bumpv->x, bumpv->z, -bumpv->y, 0);
	else
		*bumpv = vec(-bumpv->x, bumpv->z, bumpv->y, 0);
}

void	bump_cu(t_point obj_pnt, t_cube cube, t_comps *comps, t_face face)
{
	t_vec3	bumpv;
	t_point	bumpp;

	bumpv = bumpv_cu(comps->pos, cube.texture->bump_map, cube.bump_level, comps->dims);
	adjust_bumpv(&bumpv, face);
	bumpp = bumpv;
	if (comps->inside)
	{
		bumpv = neg(bumpv);
		bumpp = neg(bumpp);
	}
	if (face == UP || face == DOWN)
		bumpp = add_vec(vec(obj_pnt.x, 0, obj_pnt.z, 1), bumpp);
	else if (face == LEFT || face == RIGHT)
		bumpp = add_vec(vec(0, obj_pnt.y, obj_pnt.z, 1), bumpp);
	else
		bumpp = add_vec(vec(obj_pnt.x, obj_pnt.y, 0, 1), bumpp);
	comps->normal = mat_vec_mult(cube.t_transform, bumpv);
	comps->normal.w = 0;
	comps->normal = norm_vec(comps->normal);
	bumpp = add_vec(vec(obj_pnt.x, 0, obj_pnt.z, 1), bumpp);
	comps->point = mat_vec_mult(cube.i_transform, bumpp);
}
