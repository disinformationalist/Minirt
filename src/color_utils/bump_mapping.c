#include "minirt.h"

static inline unsigned char	gray_val(int x, int y, t_img *img)
{
	unsigned char	gray;
	int				offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	gray = *(unsigned char *)(img->pixels_ptr + offset);
	return (gray);
}

static inline t_vec3	bump(t_position pos, t_img *img, double stren)
{
	t_vec3			perturb;
	unsigned char	curr;
	double			dfdx;
	double			dfdy;
	double			norm_factor;

	curr = gray_val(pos.i, pos.j, img);
	dfdx = stren * ((double)(gray_val(pos.i + 1, pos.j, img) - curr)) / 255.0;
	dfdy = stren * ((double)(gray_val(pos.i, pos.j + 1, img) - curr)) / 255.0;
	norm_factor = 1.0 / (sqrt(dfdx * dfdx + dfdy * dfdy + 1.0));
	perturb.x = -dfdx * norm_factor;
	perturb.z = -dfdy * norm_factor;
	perturb.y = 1 - norm_factor;
	perturb.w = 0;
	return (perturb);
}

static inline t_mat4	set_sptbn(t_point obj_pnt)
{
	t_mat4	tbn;
	t_vec3			tan;
	t_vec3			bitan;

	obj_pnt = norm_vec(obj_pnt);
	tan = norm_vec(vec(-obj_pnt.z, 0, obj_pnt.x, 0));
	bitan = norm_vec(cross_prod(obj_pnt, tan));
	tuple_to_col(&tbn, tan, 0);
	tuple_to_col(&tbn, bitan, 1);
	tuple_to_col(&tbn, obj_pnt, 2);
	tuple_to_col(&tbn, vec(0, 0, 0, 1), 3);
	return (tbn);
}

void	bump_sp(t_point obj_pnt, t_sphere sphere, t_comps *comps)
{
	t_mat4	tbn;
	t_vec3			bumpv;
	t_point			bumpp;

	bumpv = bump(comps->pos, sphere.texture->bump_map, sphere.bump_level);
	obj_pnt.w = 0;
	tbn = set_sptbn(obj_pnt);
	bumpv = mat_vec_mult(tbn, bumpv);
	bumpp = bumpv;
	bumpv = norm_vec(add_vec(obj_pnt, bumpv));
	if (comps->inside)
		bumpp = subtract_vec(obj_pnt, bumpp);
	else
		bumpp = bumpv;
	if (comps->inside)
		bumpv = neg(bumpv);
	comps->normal = mat_vec_mult(sphere.t_transform, bumpv);
	comps->normal.w = 0;
	comps->normal = norm_vec(comps->normal);
	bumpp.w = 1;
	comps->point = mat_vec_mult(sphere.i_transform, bumpp);
}
