#include "minirt.h"

static inline unsigned char	pixel_gray_get(int x, int y, t_img *img)
{
	unsigned char	gray;
	int				offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	gray = *(unsigned char *)(img->pixels_ptr + offset);
	return (gray);
}

static inline t_vec3	bump(t_position pos, t_img *img, double strength)
{
	t_vec3			perturb;
	unsigned char	curr;
	double			dfdx;
	double			dfdy;
	double			norm_factor;

	curr = pixel_gray_get(pos.i, pos.j, img);
	dfdx = strength * ((double)(pixel_gray_get((pos.i + 1), \
		pos.j, img) - curr)) / 255.0;
	dfdy = strength * ((double)(pixel_gray_get(pos.i, (pos.j + 1), \
		img) - curr)) / 255.0;
	norm_factor = 1.0 / (sqrt(dfdx * dfdx + dfdy * dfdy + 1.0));
	perturb.x = -dfdx * norm_factor;
	perturb.z = -dfdy * norm_factor;
	perturb.y = 1 - norm_factor;
	perturb.w = 0;
	return (perturb);
}

static inline t_vec3	set_cytbn(t_point obj_pnt, t_vec3 bumpv, t_vec3 *bumpp)
{
	t_vec3			tan;
	t_vec3			bitan;
	t_mat4	tbn;
	t_vec3			norm;

	norm = vec(obj_pnt.x, 0, obj_pnt.z, 0);
	tan = vec(-norm.z, 0, norm.x, 0);
	bitan = vec(0, 1, 0, 0);
	tuple_to_col(&tbn, tan, 0);
	tuple_to_col(&tbn, bitan, 1);
	tuple_to_col(&tbn, norm, 2);
	tuple_to_col(&tbn, vec(0, 0, 0, 1), 3);
	bumpv = mat_vec_mult(tbn, bumpv);
	*bumpp = bumpv;
	bumpv = norm_vec(add_vec(bumpv, norm));
	return (bumpv);
}

static inline void	cy_bump_norm(t_vec3 bumpv, t_vec3 bumpp, \
	t_cylinder cyl, t_comps *comps)
{
	bumpv.w = 0;
	comps->normal = mat_vec_mult(cyl.t_transform, bumpv);
	comps->normal.w = 0;
	comps->normal = norm_vec(comps->normal);
	bumpp.w = 1;
	comps->point = mat_vec_mult(cyl.i_transform, bumpp);
}

void	bump_cy(t_point obj_pnt, t_cylinder cyl, t_comps *comps)
{
	t_vec3			bumpv;
	t_vec3			bumpp;

	bumpv = bump(comps->pos, cyl.texture->bump_map, cyl.bump_level);
	bumpp = bumpv;
	if (comps->is_top)
		bumpv.y += 1;
	else if (comps->is_bot)
	{
		bumpv = neg(bumpv);
		bumpv.y -= 1;
		bumpp = neg(bumpp);
	}
	else
		bumpv = set_cytbn(obj_pnt, bumpv, &bumpp);
	if (comps->inside)
	{
		bumpv = neg(bumpv);
		bumpp = neg(bumpp);
	}
	bumpp = add_vec(obj_pnt, bumpp);
	cy_bump_norm(bumpv, bumpp, cyl, comps);
}
