#include "minirt.h"

static inline unsigned char	pixel_gray_get_h(int x, int y, t_img *img)
{
	unsigned char	gray;
	int				offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	gray = *(unsigned char *)(img->pixels_ptr + offset);
	return (gray);
}

t_vec3	bump_h(t_position pos, t_img *img, double strength)
{
	t_vec3			perturb;
	unsigned char	curr;
	double			dfdx;
	double			dfdy;
	double			norm_factor;

	curr = pixel_gray_get_h(pos.i, pos.j, img);
	dfdx = strength * ((double)(pixel_gray_get_h((pos.i + 1),
					pos.j, img) - curr)) / 255.0;
	dfdy = strength * ((double)(pixel_gray_get_h(pos.i, (pos.j + 1),
					img) - curr)) / 255.0;
	norm_factor = 1.0 / (sqrt(dfdx * dfdx + dfdy * dfdy + 1.0));
	perturb.x = -dfdx * norm_factor;
	perturb.z = -dfdy * norm_factor;
	perturb.y = 1 - norm_factor;
	perturb.w = 0;
	return (perturb);
}

static inline t_vec3	set_hytbn(t_point obj_pnt, t_vec3 bumpv, t_vec3 *bumpp)
{
	t_vec3			tan;
	t_vec3			bitan;
	t_mat4	tbn;
	t_vec3			norm;

	norm = vec(2 * obj_pnt.x, -2 * obj_pnt.y, 2 * obj_pnt.z, 0);
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

static inline void	hy_bump_norm(t_vec3 bumpv, t_vec3 bumpp,
				t_hyperboloid hyp, t_comps *comps)
{
	bumpv.w = 0;
	comps->normal = mat_vec_mult(hyp.t_transform, bumpv);
	comps->normal.w = 0;
	comps->normal = norm_vec(comps->normal);
	bumpp.w = 1;
	comps->point = mat_vec_mult(hyp.i_transform, bumpp);
}

void	bump_hy(t_point obj_pnt, t_hyperboloid hyp, t_comps *comps)
{
	t_vec3			bumpv;
	t_vec3			bumpp;

	bumpv = bump_h(comps->pos, hyp.texture->bump_map, hyp.bump_level);
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
		bumpv = set_hytbn(obj_pnt, bumpv, &bumpp);
	if (comps->inside)
	{
		bumpv = neg(bumpv);
		bumpp = neg(bumpp);
	}
	bumpp = add_vec(obj_pnt, bumpp);
	hy_bump_norm(bumpv, bumpp, hyp, comps);
}
