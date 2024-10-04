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

void ft_swap(double *a, double *b)
{
	double temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static inline bool	check_trunk_solutions(t_vec3 abc, double *t1, double *t2)
{
	double	discrim;
	double	inv_2a;
	double 	sq_discrim;
	
	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-5)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	*t1 = (-abc.y - sq_discrim) * inv_2a;
	*t2 = (-abc.y + sq_discrim) * inv_2a;
	//if (*t1 > *t2)//from buck, just sorting i think
	//	ft_swap(t1, t2);
	return (true);
} 

void	compute_abc(t_vec3 *abc, t_ray ray)
{
	abc->x = ray.dir.x * ray.dir.x + ray.dir.z * ray.dir.z;
	abc->y = 2 * (ray.origin.x * ray.dir.x + ray.origin.z * ray.dir.z);
	abc->z = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - 1;
}

bool	check_cap(t_ray ray, double t)
{
	double x;
	double z;

	x = ray.origin.x + t * ray.dir.x;
	z = ray.origin.z + t * ray.dir.z;
	if ((x * x + z * z) <= 1)
		return (true);
	return (false);	
}

//later return false if cyl.closed == false

bool	intersect_caps(t_ray ray, double half_h, double *t3, double *t4)
{
	double	t;
	bool	hit1;
	bool	hit2;

	*t3 = INFINITY;
	*t4 = INFINITY;
	if (fabs(ray.dir.y) < 1e-5)
		return (false);
	t = (-half_h - ray.origin.y) / ray.dir.y;
	hit1 = check_cap(ray, t);
	if (hit1)
		*t4 = t;
	t = (half_h - ray.origin.y) / ray.dir.y;
	hit2 = check_cap(ray, t);
	if (hit2)
		*t3 = t;
	if (hit1 || hit2)
		return (true);
	return (false);
}

bool	within_height(t_ray ray, double min, double max, double t)
{
	double y;
	
	if (t < 1e-5)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	if (y > min && y < max)
		return (true);
	return (false);
}

bool	ray_cylinder_intersect(t_cylinder cylinder, t_ray ray, double *t)
{
	t_vec3	abc;
	double	t1;
	double	t2;
	double 	t3;
	double 	t4;
	double	half_h;

	half_h = cylinder.height / 2.0;// store this as cyl height  at parse?
	ray = transform(ray, cylinder.transform);
	compute_abc(&abc, ray);
	if (abc.x != 0)
	{
		if (check_trunk_solutions(abc, &t1, &t2))
		{
			if (within_height(ray, -half_h, half_h, t1) && t1 < *t)
					*t = t1;
			if (within_height(ray, -half_h, half_h, t2) && t2 < *t)
					*t = t2;
		}
	}
	if (intersect_caps(ray, half_h, &t3, &t4))
	{
		if (t3 > 0 && t3 < *t)
			*t = t3;
		if (t4 > 0 && t4 < *t)
			*t = t4;
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
		//*t = INFINITY;
		if (ray_cylinder_intersect(*curr_cy, ray, t))
		{
			if (*t < closest->t)
			{
				closest->t = *t;//move these to outer function after checking all t's for a shape, let t track for each shape, close obj must stay..
				closest->object = curr_cy;
				closest->object_type = CYLINDER;
			}
		}
		curr_cy = curr_cy->next;
		if (curr_cy == cylinders)
			break;
	}
}


t_vec3 cyl_normal_at(t_point int_pnt, t_matrix_4x4 transform, double half_h)
{
	t_vec3	norm;
	double	dist;

	int_pnt = mat_vec_mult(transform, int_pnt);
	dist = int_pnt.x * int_pnt.x + int_pnt.z * int_pnt.z;
	if (dist < .99999 && int_pnt.y >= half_h - 1e-5)
		norm = vec(0, 1, 0, 0);
	else if (dist < .99999 && int_pnt.y <= -half_h + 1e-5)
		norm = vec(0, -1, 0, 0);
	else
		norm = vec(int_pnt.x, 0, int_pnt.z, 0);
	norm = mat_vec_mult(transpose(transform), norm);
	norm.w = 0;
	return (norm_vec(norm));
}

t_comps	set_cycomps(t_cylinder *cylinder, double t, t_ray r)
{
	t_comps	comps;
	
	comps.t = t;
	comps.point = add_vec(r.origin, scale_vec(t, r.dir));
	comps.normal = cyl_normal_at(comps.point, cylinder->transform, cylinder->height / 2.0);
	comps.eyev = neg(r.dir);
	if (dot_product(comps.normal, comps.eyev) < 0)
	{
		comps.inside = true;
		comps.normal = neg(comps.normal);
	}
	else
		comps.inside = false;
	return (comps);
}


t_norm_color color_cylinder(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_cylinder		*cylinder;
	t_comps			comps;
	t_norm_color	lt_color;
	t_norm_color	color1;
	t_light			*curr_lt;

	cylinder = (t_cylinder *)closest->object;
	lt_color = color(0, 0, 0);
	if (trace->lights)
	{
		comps = set_cycomps(cylinder, closest->t, r);
		curr_lt = trace->lights;
		while (true)
		{
			comps.light_dir = norm_vec(subtract_vec(curr_lt->center, comps.point));
			comps.cos_angle = dot_product(comps.normal, comps.light_dir);
			comps.reflectv = subtract_vec(scale_vec(2 * comps.cos_angle, comps.normal), comps.light_dir);
			if (!obscured_b(trace, ray(comps.light_dir, add_vec(comps.point, scale_vec(1e-5, comps.normal))), curr_lt->center, comps.point))
				lt_color = sum_rgbs(lt_color, mult_color(curr_lt->brightness * get_light_int(comps, cylinder->mat), curr_lt->color));
			curr_lt = curr_lt->next;
			if (curr_lt == trace->lights)
				break;
		}	
	//cylinder->color = stripe_at(int_pnt, cylinder->transform);//trying color function
	}
	color1 = cylinder->color;
	return (get_final_color1(trace, color1, lt_color));
	//return (get_final_color(trace, cylinder->color, light_int));
}

/* 
t_norm_color color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)//working. now make lights have color WORKING! SEND IT! then do spotlights...
{
	t_sphere		*sphere;
	t_comps			comps;
	t_norm_color	lt_color;
	t_norm_color	color1;
	t_light			*curr_lt;

	sphere = (t_sphere *)closest->object;
	lt_color = color(0, 0, 0);
	if (trace->lights)
	{
		comps = set_spcomps(sphere, closest->t, r);
		curr_lt = trace->lights;
		while (true)
		{
			comps.light_dir = norm_vec(subtract_vec(curr_lt->center, comps.point));
			comps.cos_angle = dot_product(comps.normal, comps.light_dir);
			comps.reflectv = subtract_vec(scale_vec(2 * comps.cos_angle, comps.normal), comps.light_dir);
			if (!obscured_b(trace, ray(comps.light_dir, add_vec(comps.point, scale_vec(1e-5, comps.normal))), curr_lt->center, comps.point))
				lt_color = sum_rgbs(lt_color, mult_color(curr_lt->brightness * get_light_int(comps, sphere->mat), curr_lt->color));
			curr_lt = curr_lt->next;
			if (curr_lt == trace->lights)
				break;
		}	

	//sphere->color = stripe(int_pnt);//trying color function
	//sphere->color = stripe_at(int_pnt, sphere->transform);//trying color function
	//color1 = checker_at(int_pnt, sphere->transform);
	//color1 = gradient_at(int_pnt, sphere->transform, color(0, 255, 0), color(0, 0, 255));

	}
	color1 = sphere->color;
	return (get_final_color1(trace, color1, lt_color));
} */


//old
/* unsigned int color_cylinder(t_trace *trace, t_ray r, t_track_hits *closest)
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
		normal = norm_vec(subtract_vec(ci, scale_vec(dot_product(ci, axis), axis)));
//		light_dir = norm_vec(subtract_vec(int_pnt, trace->lights->center));
		light_dir = norm_vec(subtract_vec(trace->lights->center, int_pnt));//swapped args
		if (dot_product(normal, r.dir) > 0)//if (dot_product(normal, view_dir) < 0)//orig, r.dir is normed at inital raycast now.
			normal = neg(normal);
		if (!obscured(trace, int_pnt, light_dir, normal))
			light_int = trace->lights->brightness * get_light_int(normal, light_dir, neg(r.dir));//diff + spec here
			
	}
	return (get_final_color(trace, cylinder->color, light_int));
} */