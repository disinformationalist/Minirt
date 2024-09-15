/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_cylinders.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 18:11:21 by arybarsk          #+#    #+#             */
/*   Updated: 2024/09/02 18:11:23 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//JUST NEED TO DO END CAPS

//this function will check each cylinder passed to it
// a, b and c got combined into one t_vec3 because of the norm 
static inline bool	check_trunk_solutions(t_vec3 abc, double *t1, double *t2)//changed to calc sq_discrim
{
	double	discrim;
	double	inv_2a;
	double 	sq_discrim;

	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 0)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	*t1 = (-abc.y - sq_discrim) * inv_2a;
	*t2 = (-abc.y + sq_discrim) * inv_2a;
	return (true);
}

bool	is_within_height(double t_val, t_cylinder cylinder, t_vec3 ray_dir, t_vec3 ray_orig)
{
	t_vec3	int_pnt;
	t_vec3	axis;
	double	projection_len;

	if (t_val < 1e-5)
		return (false);
	int_pnt = add_vec(ray_orig, scale_vec(t_val, ray_dir));
	axis = norm_vec(cylinder.norm);//normed already
	projection_len = dot_product(subtract_vec(int_pnt, cylinder.center), axis);
	//if (projection_len >= 0 && projection_len <= cylinder.height)
	if (projection_len >= -cylinder.height / 2 && projection_len <= cylinder.height / 2)
		return (true);
	else
		return (false);
}

static inline void	compute_abc(t_cylinder cylinder, t_vec3 ray_dir, t_vec3 ray_orig, t_vec3 *abc)
{
	t_vec3	oc;//ray origin to center
	t_vec3	axis;//cylinder axis
	t_vec3	oc_proj;//oc projected on a plane perpendicular to axis
	t_vec3	ray_proj;//ray_dir projected on a plane perpendicular to axis

	oc = subtract_vec(ray_orig, cylinder.center);
	axis = norm_vec(cylinder.norm);//should already be normed, adjust parser
	oc_proj = subtract_vec(oc, scale_vec(dot_product(oc, axis), axis));
	ray_proj = subtract_vec(ray_dir, scale_vec(dot_product(ray_dir, axis), axis));
	abc->x = dot_product(ray_proj, ray_proj); //a
	abc->y = 2.0 * dot_product(ray_proj, oc_proj); //b
	abc->z = dot_product(oc_proj, oc_proj) - cylinder.radius * cylinder.radius; //c
}
static inline bool	is_within_cap(double t_val, t_cylinder cylinder, t_vec3 int_pnt, t_vec3 cap_center)
{
	double	int_to_center;

	if (t_val < 1e-5)
		return (false);
	int_to_center = magnitude(subtract_vec(int_pnt, cap_center));
	if (int_to_center <= cylinder.radius)
		return (true);
	else
		return (false);
}

static inline bool	check_caps_solutions(t_cylinder cylinder, t_vec3 ray_dir, t_vec3 ray_orig, double *t3)
{
	t_vec3	bottom_cap_center;
	t_vec3	top_cap_center;
	t_vec3	axis;
	t_vec3	int_pnt;

	axis = norm_vec(cylinder.norm);
<<<<<<< HEAD
	top_cap_center = add_vec(cylinder.center, scale_vec(cylinder.height, axis));
=======
	top_cap_center = add_vec(cylinder.center, scale_vec(cylinder.height / 2, axis));
>>>>>>> main
	*t3 = dot_product(subtract_vec(top_cap_center, ray_orig), axis) / dot_product(ray_dir, axis);
	int_pnt = add_vec(ray_orig, scale_vec(*t3, ray_dir));
	if (*t3 > 1e-5 && is_within_cap(*t3, cylinder, int_pnt, top_cap_center))//TODO
		return (true);
<<<<<<< HEAD
	bottom_cap_center = cylinder.center;
=======
	bottom_cap_center = subtract_vec(cylinder.center, scale_vec(cylinder.height / 2, axis));
>>>>>>> main
	*t3 = dot_product(subtract_vec(bottom_cap_center, ray_orig), axis) / dot_product(ray_dir, axis);
	int_pnt = add_vec(ray_orig, scale_vec(*t3, ray_dir));
	if (*t3 > 1e-5 && is_within_cap(*t3, cylinder, int_pnt, bottom_cap_center))//TODO
		return (true);
	return (false);
}

bool	ray_cylinder_intersect(t_cylinder cylinder, t_vec3 ray_dir, t_vec3 ray_orig, double *t)
{
	t_vec3	abc;//coefficients of quadratic; a is abc.x, b is abc.y, c is abc.z
	double	t1;
	double	t2;
	double	t3;

	compute_abc(cylinder, ray_dir, ray_orig, &abc);
	if (check_trunk_solutions(abc, &t1, &t2))
	{
		if (is_within_height(t1, cylinder, ray_dir, ray_orig) && t1 < INFINITY)
			*t = t1;
		if (is_within_height(t2, cylinder, ray_dir, ray_orig) && t2 < *t)
			*t  = t2;
	}
	if (check_caps_solutions(cylinder, ray_dir, ray_orig, &t3))
	{
		if (t3 > 1e-5 && t3 < *t)
			*t = t3;
	}
	if (*t < INFINITY)
		return (true);
	return (false);
}

void	check_cylinders(t_cylinder *cylinders, t_track_hits *closest, t_ray ray, double *t)
{
	t_cylinder	*curr_cy;

	if (cylinders == NULL)
		return ;
	curr_cy = cylinders;
	while (true)
	{
		if (ray_cylinder_intersect(*curr_cy, ray.dir, ray.origin, t))
		{
			if (*t < closest->t && *t > 0)
			{
				closest->t = *t;
				closest->object = curr_cy;
				closest->object_type = CYLINDER;
			}
		}
		curr_cy = curr_cy->next;
		if (curr_cy == cylinders)
			break;
	}
}

unsigned int color_cylinder(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_vec3			int_pnt;//light intersect with surface
	t_vec3			normal;
	t_vec3			light_dir;
	//t_vec3			view_dir;
	t_vec3			ci;//center to intersect
	t_vec3			axis;
	double			light_int;
	//double			cos_angle;
	t_cylinder		*cylinder;

	cylinder = (t_cylinder *)closest->object;

	light_int = 0;
	if (trace->lights)
	{
		//plug closest->t back into ray eq for intersect point;
		int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		ci = subtract_vec(int_pnt, cylinder->center);
		axis = norm_vec(cylinder->norm);//already normed unless a non normed entry. have parser check these.
<<<<<<< HEAD
		
		//must adjust normal handling for caps.
=======
>>>>>>> main
		normal = norm_vec(subtract_vec(ci, scale_vec(dot_product(ci, axis), axis)));
//		light_dir = norm_vec(subtract_vec(int_pnt, trace->lights->center));
		light_dir = norm_vec(subtract_vec(trace->lights->center, int_pnt));//swapped args
		if (dot_product(normal, r.dir) > 0)//if (dot_product(normal, view_dir) < 0)//orig, r.dir is normed at inital raycast now.
			normal = neg(normal);
		if (!obscured(trace, int_pnt, light_dir, normal))
			light_int = trace->lights->brightness * get_light_int(normal, light_dir, neg(r.dir));//diff + spec here
			
			/* cos_angle = dot_product(normal, light_dir);
			light_int	= trace->lights->brightness * fmax(cos_angle, 0.0); */
	}
	return (get_final_color(trace, cylinder->color, light_int));
}
