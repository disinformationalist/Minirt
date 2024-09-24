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

	if (fabs(ray.dir.y) < 1e-5)// close to parallel, no intersect
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

unsigned int	color_plane(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_plane	*plane;
	t_vec3	int_pnt;
	t_vec3	light_dir; 
	t_vec3	norm;
	double	light_int;

	plane = (t_plane *)closest->object;
	light_int = 0;
	if (trace->lights)
	{
		int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		light_dir = norm_vec(subtract_vec(trace->lights->center, int_pnt));
		//norm = norm_vec(mat_vec_mult(transpose(plane->transform), vec(0, 1, 0, 0)));		
		norm = plane->norm;
		if (dot_product(norm, r.dir) > 0)
			norm = neg(norm);
		if (!obscured(trace, int_pnt, light_dir, norm))
			light_int = trace->lights->brightness * get_light_int(norm, light_dir, neg(r.dir));
	//plane->color = stripe_at(int_pnt, plane->transform);//trying color function
	//plane->color = stripe(int_pnt);
	}
	return (get_final_color(trace, plane->color, light_int));
}

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


	//old light calc.
			/* cos_angle = dot_product(plane->norm, light_dir);
			light_int	= trace->lights->brightness * fmax(cos_angle, 0.0);
			light_int = fmin(light_int, 1.0); */