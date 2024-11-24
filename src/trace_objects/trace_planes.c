#include "minirt.h"

bool	ray_plane_intersect(t_plane plane, t_ray ray, double *t)
{
	ray = transform(ray, plane.transform);
	if (fabs(ray.dir.y) < 1e-6)
		return (false);
	*t = -ray.origin.y / ray.dir.y;
	if (*t > 0)
		return (true);
	return (false);
}

void	check_planes(t_plane *planes, t_track_hits *closest,
		t_ray ray, double *t)
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
			break ;
	}
}

//diff plus specular for sp

static inline double	get_pllight_int(t_vec3 norm, t_vec3 light_dir,
				t_vec3 view_dir)
{
	t_vec3	ref;
	double	spec;
	double	light_int;
	double	cos_a;

	cos_a = dot_product(norm, light_dir);
	ref = subtract_vec(scale_vec(2 * cos_a, norm), light_dir);
	spec = pow(fmax(dot_product(ref, view_dir), 0), 200);
	light_int = fmax(cos_a, 0.0) + .5 * spec;
	return (light_int);
}

t_norm_color	color_plane(t_trace *trace, t_ray r, t_track_hits *closest)
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
		norm = plane->norm;
		if (dot_product(norm, r.dir) > 0)
			norm = neg(norm);
		if (!obscured(trace, int_pnt, light_dir, norm))
			light_int = trace->lights->brightness * get_pllight_int(norm,
					light_dir, neg(r.dir));
	}
	return (get_final_color(trace, plane->color, light_int));
}
