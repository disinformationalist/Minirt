#include "minirt.h"

/* static inline void	spcolor_option(t_trace *trace, t_point int_pnt, t_sphere *sphere)//see it obj_pnt is better precomp and use for normal at
{

} */

static inline t_vec3	sp_normal_at(t_point obj_pnt, t_matrix_4x4 transform)
{
	t_vec3	norm;

	norm = mat_vec_mult(transpose(transform), obj_pnt);
	norm.w = 0;
	return (norm_vec(norm));
}

static inline t_comps	set_spcomps(t_sphere *sphere, t_intersects *intersects, t_ray r, t_trace *trace)
{
	t_comps	comps;
	t_point obj_pnt;
	
	(void)trace;
	comps.t = intersects->closest->t;
	comps.ray = r;
	comps.point = add_vec(r.origin, scale_vec(comps.t, r.dir));
	obj_pnt = mat_vec_mult(sphere->transform, comps.point);
	comps.normal = sp_normal_at(obj_pnt, sphere->transform);
	
	comps.eyev = neg(r.dir);
	comps.mat = sphere->mat;
	set_indicies(intersects, &comps.n1, &comps.n2);
	if (dot_product(comps.normal, comps.eyev) < 0)
	{
		comps.inside = true;
		comps.normal = neg(comps.normal);
	}
	else
		comps.inside = false;
	comps.over_pnt = add_vec(comps.point, scale_vec(1e-6, comps.normal));
	comps.under_pnt = subtract_vec(comps.point, scale_vec(1e-6, comps.normal));
	
	//make a set color function to choose between color, pattern texture w/o bump
	//this must be down here
	if (sphere->texture)
	{
		comps.color = texture_sp_at(trace, obj_pnt, sphere, &comps);//if texturing
		
	}
	else 
		comps.color = sphere->color;
	return (comps);
}

t_norm_color color_sphere(t_trace *trace, t_ray r, t_intersects *intersects, t_depths depths)
{
	t_sphere		*sphere;
	t_comps			comps;
	t_norm_color	lt_color;
	t_light			*curr_lt;
	t_norm_color	refl_col;
	t_norm_color	refr_col;

	sphere = (t_sphere *)intersects->closest->object;
	lt_color = color(0, 0, 0);
	comps = set_spcomps(sphere, intersects, r, trace);
	if (trace->lights)
	{
		curr_lt = trace->lights;
		while (true)
		{
			comps.light_dir = norm_vec(subtract_vec(curr_lt->center, comps.point));
			handle_light(trace, &comps, &lt_color, curr_lt);
			curr_lt = curr_lt->next;
			if (curr_lt == trace->lights)
				break;
		}
	}
	refl_col = get_reflected(trace, comps, intersects, depths);
	refr_col = get_refracted(trace, comps, intersects, depths);
	return (get_final_color3(trace, comps, lt_color, refl_col, refr_col));
}


	//return (get_final_color1(trace, comps.color, lt_color, m.amb));
	//color1 = sphere->color;
	//sphere->color = stripe(int_pnt);//trying color function
	//sphere->color = stripe_at(int_pnt, sphere->transform);//trying color function
	//color1 = checker_at(int_pnt, sphere->transform);
	//color1 = gradient_at(int_pnt, sphere->transform, color(0, 255, 0), color(0, 0, 255));
