#include "minirt.h"

static inline unsigned char pixel_gray_get(int x, int y, t_img *img)
{
	unsigned char	gray;
	int 			offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	gray = *(unsigned char *)(img->pixels_ptr + offset);
	return (gray);
}

static inline t_vec3 bump(t_position pos, t_position dims, t_img *img, double strength)//remove dims?
{
	t_vec3 			perturb;
	unsigned char	curr;
	double			dfdx;
	double			dfdy;
	double			norm_factor;
	
	(void)dims;
	curr = pixel_gray_get(pos.i, pos.j, img);
	dfdx = strength * ((double)(pixel_gray_get((pos.i + 1), pos.j, img) - curr)) / 255.0;
	dfdy = strength * ((double)(pixel_gray_get(pos.i, (pos.j + 1), img) - curr)) / 255.0;
	/* dfdx = 20 * ((double)(pixel_gray_get((pos.i + 1) % dims.i, pos.j, img) - curr)) / 255.0;
	dfdy = 20 * ((double)(pixel_gray_get(pos.i, (pos.j + 1) % dims.j, img) - curr)) / 255.0; */
	norm_factor = 1.0 / (sqrt(dfdx * dfdx + dfdy * dfdy + 1.0));
	perturb.x = -dfdx * norm_factor;
	perturb.z = -dfdy * norm_factor;
	perturb.y = norm_factor; 
	perturb.w = 0;
	return (perturb);
}

void	bump_pl(t_point obj_pnt, t_plane plane, t_comps *comps)
{
	t_vec3	bumpv;

	bumpv = bump(comps->pos, comps->dims, plane.texture->bump_map, 40);
	if (comps->inside)
		bumpv = neg(bumpv);
	comps->normal = mat_vec_mult(plane.t_transform, bumpv);
	comps->normal.w = 0;
	comps->normal = norm_vec(comps->normal);
	bumpv = add_vec(vec(obj_pnt.x, 0, obj_pnt.z, 1), bumpv);
	comps->point = mat_vec_mult(plane.i_transform, bumpv);
}

void	bump_sp(t_point obj_pnt, t_sphere sphere, t_comps *comps)
{
	t_vec3	bumpv;
	
	bumpv = bump(comps->pos, comps->dims, sphere.texture->bump_map, 30);
		bumpv.y = 1 - bumpv.y;
	if (comps->inside)
		bumpv = neg(bumpv);
	bumpv = add_vec(obj_pnt, bumpv);
	bumpv.w = 0;
	comps->normal = mat_vec_mult(sphere.t_transform, bumpv);
	comps->normal.w = 0;
	comps->normal = norm_vec(comps->normal);
	bumpv.w = 1;
	comps->point = mat_vec_mult(sphere.i_transform, bumpv);
}