#include "minirt.h"

void	set_lt_widlen(t_light *lt, double wid, double len)
{
	t_cube *emitter;

	//scale cube emitter
	emitter = lt->emitter;
	emitter->curr_scale = inv_scaling(wid, \
			emitter->h_height, len);
	emitter->transform = (mat_mult(emitter->curr_scale, \
			emitter->curr_rottran));
	emitter->i_transform = inverse(emitter->transform);
	emitter->t_transform = transpose(emitter->transform);
	set_arealt(lt);
}

void	gui_scale_object(t_trace *trace, t_on * on, t_vec3 scale)
{
	t_type type = on->type;
	(void)trace;
	if (type == SPHERE)
	{
		t_sphere *sp = (t_sphere *)on->object;
		sp->curr_scale = inv_scaling(scale.x, scale.y, scale.z);
		sp->transform = mat_mult(sp->curr_scale, sp->curr_rottran);
		sp->t_transform = transpose(sp->transform);
		sp->i_transform = inverse(sp->transform);
	}
	else if (type == PLANE)
	{
		t_plane *pl = (t_plane *)on->object;
		pl->curr_scale = inv_scaling(scale.x, scale.y, scale.z);
		pl->transform = mat_mult(pl->curr_scale, pl->curr_rottran);
		pl->t_transform = transpose(pl->transform);
		pl->i_transform = inverse(pl->transform);
	}
	else if (type == CYLINDER)
	{
		t_cylinder *cy = (t_cylinder *)on->object;
		cy->curr_scale = inv_scaling(scale.x, scale.y, scale.z);
		cy->transform = mat_mult(cy->curr_scale, cy->curr_rottran);
		cy->t_transform = transpose(cy->transform);
		cy->i_transform = inverse(cy->transform);
	}
	else if (type == HYPERBOLOID)
	{
		t_hyperboloid *hy = (t_hyperboloid *)on->object;
		hy->curr_scale = inv_scaling(scale.x, scale.y, scale.z);
		hy->transform = mat_mult(hy->curr_scale, hy->curr_rottran);
		hy->t_transform = transpose(hy->transform);
		hy->i_transform = inverse(hy->transform);
	}
	else if (type == CUBE)
	{
		t_cube *cu = (t_cube *)on->object;
		cu->curr_scale = inv_scaling(scale.x, scale.y, scale.z);
		cu->transform = mat_mult(cu->curr_scale, cu->curr_rottran);
		cu->t_transform = transpose(cu->transform);
		cu->i_transform = inverse(cu->transform);
	}
}
