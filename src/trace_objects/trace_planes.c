#include "minirt.h"

bool	ray_plane_intersect(t_plane plane, t_vec3 ray_dir, t_vec3 ray_orig, double *t)
{
	float	denom;
	double	sol;
	t_vec3	op;//origin to point

	denom = dot_product(plane.norm, ray_dir);
	

	if (fabs(denom) < 1e-6)// close to parallel, no intersect
		return (false);

	
	op = subtract_vec(plane.point, ray_orig);
	sol = dot_product(op, plane.norm) / denom;
	if (sol > 0)//in front of ray origin, not behind
	{
		*t = sol;
		return (true);
	}
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
		if (ray_plane_intersect(*curr_pl, ray.dir, ray.origin, t))
		{
			if (*t < closest->t && *t > 0)
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
	t_vec3	int_pnt;
	t_vec3	light_dir; 
	float	light_int;
	//double	cos_angle;
	t_plane	*plane;

	plane = (t_plane *)closest->object;
	if (trace->lights)
	{
		//plug closest->t back into ray eq for intersect point;
		int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		light_dir = norm_vec(subtract_vec(trace->lights->center, int_pnt));
		if (dot_product(plane->norm, r.dir) > 0)//this seems to work for moving to other side dubchk
			plane->norm = neg(plane->norm);
		if (obscured(trace, int_pnt, light_dir, plane->norm))
				light_int = 0;
		else
		{
			light_int = trace->lights->brightness * get_light_int(plane->norm, light_dir, neg(r.dir));//diff + spec here
			
			/* cos_angle = dot_product(plane->norm, light_dir);
			light_int	= trace->lights->brightness * fmax(cos_angle, 0.0);
			light_int = fmin(light_int, 1.0); */
		}
	}
	else
		light_int = 0;
	//plane->color = stripe(int_pnt);//trying color function

	return (get_final_color(trace, plane->color, light_int));
}
//plane norm vector must point toward light for compute, check in parser and set norm vector sign there.