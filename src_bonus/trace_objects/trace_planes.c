#include "minirt.h"
/* 		   soft
		   spot
		  light 
		 / 	|  \
		/  / \  \
	   /  /   \  \
	  /  /     \  \
	 /  /	    \  \
			^	  ^
			|	  |
	inner cone    outer cone
(full intensity)  (attenuating toward outer edge)	 */	
//playing with spotlight adjustment here
//check the condition first to see if there is any need for the compute of diff/spec to opti.

double spotlight(t_vec3 light_dir)
{
	t_vec3	ln;//spot light pointing direction
	double	cos_theta;
	double	inner_cone = cos(M_PI / 12);
	double	outer_cone = cos(M_PI / 8);

	ln = norm_vec(vec(0, 1, 0, 0));//neg orientation 
	cos_theta = dot_product(ln, light_dir);
	if (cos_theta > inner_cone)
		return (1.0);
	else if (cos_theta > outer_cone)
	{
		return ((cos_theta - outer_cone) / (inner_cone - outer_cone));
	}
	else
		return (0);
}

bool	ray_plane_intersect(t_plane plane, t_ray ray, double *t)
{
	ray = transform(ray, plane.transform);

	if (fabs(ray.dir.y) < 1e-5)
		return (false);
	*t = -ray.origin.y / ray.dir.y;
	if (*t > 0)
		return (true);
	return (false);
}

void	check_planes(t_plane *planes, t_track_hits *closest, t_ray ray, double *t)
{
	t_plane		*curr_pl;

	if (planes == NULL)
		return ;
	curr_pl = planes;
	while (true)
	{
		if (ray_plane_intersect(*curr_pl, ray, t))
		{
			if (*t < closest->t)
			{
				closest->t = *t;
				closest->object = curr_pl;
				closest->object_type = PLANE;
			}
		}
		curr_pl = curr_pl->next;
		if (curr_pl == planes)
			break;
	}
}

/* double	get_splight_int(t_comps comps, t_mat mat)
{
	t_vec3	ref;
	double	spec;
	double	light_int;
	
	ref = subtract_vec(scale_vec(2 * comps.cos_angle, comps.normal), comps.light_dir);
	spec = pow(fmax(dot_product(ref, comps.eyev), 0), mat.shine);
	light_int = mat.diff * fmax(comps.cos_angle, 0.0) + mat.spec * spec;
	//return (spotlight(light_dir) * light_int);//trying sp_light
	return (light_int);
} */

/* typedef struct t_comps
{
	double	t;
	//void	*object;
	//t_type	object_type;
	t_vec3	point;
	t_vec3	eyev;
	t_vec3	normal;
	t_vec3	reflectv;
	bool	inside;
}	t_comps; */
//this function will check each sphere passed to it


t_comps	set_plcomps(t_plane *plane, double t, t_ray r)
{
	t_comps	comps;
	
	comps.t = t;
	comps.point = add_vec(r.origin, scale_vec(t, r.dir));
	comps.normal = plane->norm;
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

t_norm_color	color_plane(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_plane			*plane;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*curr_lt;

	/* t_vec3	int_pnt;
	t_vec3	light_dir; 
	t_vec3	norm;
	double	light_int; */
	t_norm_color	color1;

	plane = (t_plane *)closest->object;
	lt_color = color(0, 0, 0);
	if (trace->lights)
	{
		comps = set_plcomps(plane, closest->t, r);
		curr_lt = trace->lights;
		while (true)
		{
			comps.light_dir = norm_vec(subtract_vec(curr_lt->center, comps.point));
			comps.cos_angle = dot_product(comps.normal, comps.light_dir);
			comps.reflectv = subtract_vec(scale_vec(2 * comps.cos_angle, comps.normal), comps.light_dir);
			if (!obscured_b(trace, ray(comps.light_dir, add_vec(comps.point, scale_vec(1e-5, comps.normal))), curr_lt->center, comps.point))
				lt_color = sum_rgbs(lt_color, mult_color(curr_lt->brightness * get_light_int(comps, plane->mat), curr_lt->color));
			curr_lt = curr_lt->next;
			if (curr_lt == trace->lights)
				break;
		}	
		/* int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		light_dir = norm_vec(subtract_vec(trace->lights->center, int_pnt));
		norm = plane->norm;
		if (dot_product(norm, r.dir) > 0)
			norm = neg(norm);
		if (!obscured(trace, int_pnt, light_dir, norm))
			light_int = trace->lights->brightness * get_light_int(norm, light_dir, neg(r.dir)); */

	//plane->color = stripe_at(int_pnt, plane->transform);//trying color function
	//color1 = checker_at(int_pnt, plane->transform);//works passing in to get_fin_col
	//color1 = ring_at(int_pnt, plane->transform);
	//color1 = gradient_at(int_pnt, plane->transform, color(0, 255, 0), color(0, 0, 255));
	}
	color1 = plane->color;
	return (get_final_color1(trace, color1, lt_color));
	//return (get_final_color(trace, color1, light_int));
}

//do planes like spheres..
/* _norm_color color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)//working. now make lights have color WORKING! SEND IT! then do spotlights...
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
		//loop here for multiple lights. sum total lights * lt_colors *intensity, return a total color due to colored light// SEP FUNCITON?
		curr_lt = trace->lights;
		while (true)
		{
			comps.light_dir = norm_vec(subtract_vec(curr_lt->center, comps.point));
			comps.cos_angle = dot_product(comps.normal, comps.light_dir);
			comps.reflectv = subtract_vec(scale_vec(2 * comps.cos_angle, comps.normal), comps.light_dir);
			if (!obscured_b(trace, ray(comps.light_dir, add_vec(comps.point, scale_vec(1e-5, comps.normal))), curr_lt->center, comps.point))
				lt_color = sum_rgbs(lt_color, mult_color(curr_lt->brightness * get_splight_int(comps, sphere->mat), curr_lt->color));
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
	return (get_final_color1(trace, color1, lt_color));//send in material here? mult mat.amb * ambcomponent....
} */


//for multiple lights later
/* unsigned int	color_plane(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_vec3	int_pnt;
	t_vec3	light_dir; 
	double	light_int;
	t_plane	*plane;

	plane = (t_plane *)closest->object;

	t_light			*curr_lt;

	light_int = 0;
	if (trace->lights)
	{
		int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		if (dot_product(plane->norm, r.dir) > 0)
			plane->norm = neg(plane->norm);
		curr_lt = trace->lights;
		while (true)
		{
			light_dir = norm_vec(subtract_vec(curr_lt->center, int_pnt));
			//hard shadows...
			//if (!obscured(trace, int_pnt, light_dir, plane->norm))
			if (!obscured_b(trace, ray(light_dir, add_vec(int_pnt, scale_vec(1e-5, plane->norm))), curr_lt->center, int_pnt))
				light_int += curr_lt->brightness * get_light_int(plane->norm, light_dir, neg(r.dir));//diff + spec here for each light
			curr_lt = curr_lt->next;
			if (curr_lt == trace->lights)
				break;
		}	
	}
	//plane->color = stripe(int_pnt);//trying color function
	return (get_final_color(trace, plane->color, light_int));
} */
