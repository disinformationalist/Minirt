#include "minirt.h"

static inline t_vec3 cyl_normal_at(t_point int_pnt, t_matrix_4x4 transform, double half_h)
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

static inline t_comps	set_cycomps(t_cylinder *cylinder, double t, t_ray r)
{
	t_comps	comps;
	
	comps.t = t;
	comps.point = add_vec(r.origin, scale_vec(t, r.dir));
	comps.normal = cyl_normal_at(comps.point, cylinder->transform, cylinder->height / 2.0);
	comps.eyev = neg(r.dir);
	comps.mat = cylinder->mat;
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
			handle_light(trace, &comps, &lt_color, curr_lt);
			curr_lt = curr_lt->next;
			if (curr_lt == trace->lights)
				break;
		}	
	//cylinder->color = stripe_at(int_pnt, cylinder->transform);//trying color function
	}
	color1 = cylinder->color;
	return (get_final_color1(trace, color1, lt_color, comps.mat.amb));
}