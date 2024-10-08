#include "minirt.h"

static inline t_vec3	sp_normal_at(t_point int_pnt, t_matrix_4x4 transform)
{
	t_vec3	norm;
	t_point obj_pnt;

	obj_pnt = mat_vec_mult(transform, int_pnt);
	norm = mat_vec_mult(transpose(transform), obj_pnt);
	norm.w = 0;
	return (norm_vec(norm));
}

static inline t_comps	set_spcomps(t_sphere *sphere, double t, t_ray r)
{
	t_comps	comps;
	
	comps.t = t;
	comps.point = add_vec(r.origin, scale_vec(t, r.dir));
	comps.normal = sp_normal_at(comps.point, sphere->transform);
	comps.eyev = neg(r.dir);
	comps.mat = sphere->mat;
	if (dot_product(comps.normal, comps.eyev) < 0)
	{
		comps.inside = true;
		comps.normal = neg(comps.normal);
	}
	else
		comps.inside = false;
	comps.over_pnt = add_vec(comps.point, scale_vec(1e-5, comps.normal));
	return (comps);
}

t_norm_color color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)
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
			handle_light(trace, &comps, &lt_color, curr_lt);
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
	return (get_final_color1(trace, color1, lt_color, comps.mat.amb));
}
