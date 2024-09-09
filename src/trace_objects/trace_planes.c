#include "minirt.h"

bool	ray_plane_intersect(t_plane plane, t_vec3 ray_dir, t_vec3 ray_orig, double *t)
{
	float	denom;
	double	sol;
	t_vec3	op;//origin to point

	denom = dot_product(plane.norm_vector, ray_dir);
	

	if (fabs(denom) < 1e-6)// close to parallel, no intersect
		return (false);

	
	op = subtract_vec(plane.point, ray_orig);
	sol = dot_product(op, plane.norm_vector) / denom;
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
		if (ray_plane_intersect(*curr_pl, ray.direction, ray.origin, t))
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
	t_vec3	intersect_pnt;
	t_vec3	light_dir; 
	double	light_intensity;
	double	cos_angle;
	t_plane	*plane;

	plane = (t_plane *)closest->object;
	if (trace->lights)
	{
		//plug closest->t back into ray eq for intersect point;
		intersect_pnt = add_vec(r.origin, scalar_mult_vec(closest->t, r.direction));
		light_dir = normalize_vec(subtract_vec(trace->lights->center, intersect_pnt));
		if (obscured(trace, intersect_pnt, light_dir, plane->norm_vector))
				light_intensity = 0;
		else
		{
			cos_angle = dot_product(plane->norm_vector, light_dir);
			light_intensity	= trace->lights->brightness * fmax(cos_angle, 0.0);
			light_intensity = fmin(light_intensity, 1.0);
			
		}
	}
	else
		light_intensity = 0;
	return (get_final_color(trace, plane->color, light_intensity));
}
//plane norm vector must point toward light for compute, check in parser and set norm vector sign there.