#include "minirt.h"

t_tx		*get_tx(char *i_name, t_tx *textures)
{
	
	t_tx *curr_tx = NULL;
	if (textures)
	{
		if (!i_name)
			return (textures);
		char *name = ft_strjoin("textures/", i_name);
		curr_tx = textures;
		while (true)
		{
			if (!strcmp(curr_tx->i_name, name))
				return (free(name), curr_tx);
			curr_tx = curr_tx->next;
			if (curr_tx == textures)
				break ;
		}
	}
	return (curr_tx);
}

static inline void	build_sp_transforms(t_sphere *curr_sp)
{
	t_mat4		inv_trans;
	t_mat4		inv_rot;

	inv_rot = rot_to(curr_sp->norm, vec(0, 1, 0, 0));
	curr_sp->curr_scale = inv_scaling(curr_sp->radius, \
	curr_sp->radius, curr_sp->radius);
	inv_trans = translation(-curr_sp->center.x, \
	-curr_sp->center.y, -curr_sp->center.z);
	curr_sp->curr_rottran = mat_mult(inv_rot, inv_trans);
}


//set all transforms to account for position and orientation.

void	set_sp_transforms(t_trace *trace)
{
	t_sphere	*curr_sp;

	if (trace->spheres)
	{
		curr_sp = trace->spheres;
		while (true)
		{
			//if not read via parser
			if (curr_sp->params == 0)
				build_sp_transforms(curr_sp);

			curr_sp->transform = (mat_mult(curr_sp->curr_scale, \
				curr_sp->curr_rottran));
			curr_sp->init_scale = vec(1.0 / curr_sp->curr_scale.mat[0], 1.0 / curr_sp->curr_scale.mat[5], 1.0 / curr_sp->curr_scale.mat[10], 0);
				
			curr_sp->t_transform = transpose(curr_sp->transform);
			curr_sp->i_transform = inverse(curr_sp->transform);

			curr_sp->pattern = uv_checker(20, 10, color(40, 40, 40), \
			color(255, 255, 255));
			curr_sp->texture = get_tx(curr_sp->i_name, trace->textures);
			if (curr_sp->i_name)
				free(curr_sp->i_name);

			curr_sp->rots = vec(0, 0, 0, 0);
			add_child(trace->bvh, curr_sp, SPHERE, curr_sp->transform, curr_sp->i_transform, NULL);
			curr_sp = curr_sp->next;
			if (curr_sp == trace->spheres)
				break ;
		}
	}
}

static inline void	build_hy_transforms(t_hyperboloid *curr_hy)
{
	t_mat4		inv_trans;
	t_mat4		inv_rot;

	inv_trans = translation(-curr_hy->center.x,
			-curr_hy->center.y, -curr_hy->center.z);
	inv_rot = rot_to(curr_hy->norm, vec(0, 1, 0, 0));
	curr_hy->curr_rottran = mat_mult(inv_rot, inv_trans);
	curr_hy->curr_scale = inv_scaling(curr_hy->rad,
		curr_hy->half_h, curr_hy->rad);
	
}


void	set_curr_hy(t_trace *trace, t_hyperboloid *curr_hy)
{

	curr_hy->init_scale = vec(1.0 / curr_hy->curr_scale.mat[0], 1.0 / curr_hy->curr_scale.mat[5], 1.0 / curr_hy->curr_scale.mat[10], 0);
	curr_hy->transform = mat_mult(curr_hy->curr_scale,
			curr_hy->curr_rottran);
	curr_hy->i_transform = inverse(curr_hy->transform);
	curr_hy->t_transform = transpose(curr_hy->transform);
	curr_hy->pattern = uv_checker(18, 9 / M_PI, color(40, 40, 40), \
	color(255, 255, 255));
	curr_hy->texture = get_tx(curr_hy->i_name, trace->textures);
	if (curr_hy->i_name)
		free(curr_hy->i_name);
	//curr_hy->rots = extract_rot(inv_rot);
	curr_hy->rots = extract_rot(curr_hy->curr_rottran);
	
	add_child(trace->bvh, curr_hy, HYPERBOLOID, curr_hy->transform, curr_hy->i_transform, NULL);
}

void	set_hy_transforms(t_trace *trace)
{
	t_hyperboloid	*curr_hy;

	if (trace->hyperboloids)
	{
		curr_hy = trace->hyperboloids;
		while (true)
		{
			if (curr_hy->params == 0)
				build_hy_transforms(curr_hy);
			set_curr_hy(trace, curr_hy);
			curr_hy = curr_hy->next;
			if (curr_hy == trace->hyperboloids)
				break ;
		}
	}
}

void	set_cu_transforms2(t_trace *trace, t_cube *curr_cu)
{
	curr_cu->i_transform = inverse(curr_cu->transform);
	curr_cu->t_transform = transpose(curr_cu->transform);
	curr_cu->pattern = uv_checker(6, 6, color(30, 30, 30), \
	color(255, 255, 255));
	add_child(trace->bvh, curr_cu, CUBE, curr_cu->transform, curr_cu->i_transform, NULL);
}

static inline void	build_cu_transforms(t_cube *curr_cu)
{
	t_mat4		inv_trans;
	t_mat4		inv_rot;

	inv_trans = translation(-curr_cu->center.x, \
			-curr_cu->center.y, -curr_cu->center.z);
	inv_rot = rot_to(curr_cu->norm, vec(0, 1, 0, 0));
			curr_cu->curr_rottran = mat_mult(inv_rot, inv_trans);
}

void	set_cu_transforms(t_trace *trace)
{
	t_cube			*curr_cu;

	if (trace->cubes)
	{
		curr_cu = trace->cubes;
		while (true)
		{
			if (curr_cu->params == 0)
				build_cu_transforms(curr_cu);
			curr_cu->curr_scale = inv_scaling(curr_cu->h_width, \
			curr_cu->h_height, curr_cu->h_depth);
			
			curr_cu->transform = (mat_mult(curr_cu->curr_scale, \
			curr_cu->curr_rottran));
			curr_cu->init_scale = vec(1.0 / curr_cu->curr_scale.mat[0], 1.0 / curr_cu->curr_scale.mat[5], 1.0 / curr_cu->curr_scale.mat[10], 0);

//			curr_cu->rots = extract_rot(inv_rot);
			curr_cu->rots = extract_rot(curr_cu->curr_rottran);

			curr_cu->texture = get_tx(curr_cu->i_name, trace->textures);
			if (curr_cu->i_name)
				free(curr_cu->i_name);
			set_cu_transforms2(trace, curr_cu);
			curr_cu = curr_cu->next;
			if (curr_cu == trace->cubes)
				break ;
		}
	}
}
