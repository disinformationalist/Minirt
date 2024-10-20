#include "minirt.h"

//diff plus specular for sp

static inline double	get_splight_int(t_vec3 norm, t_vec3 light_dir,
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

//compute normal at each intersection point using transform

static inline t_vec3	sp_normal_at(t_point int_pnt, t_matrix_4x4 transform)
{
	t_vec3	norm;
	t_point	obj_pnt;

	obj_pnt = mat_vec_mult(transform, int_pnt);
	norm = mat_vec_mult(transpose(transform), obj_pnt);
	norm.w = 0;
	return (norm_vec(norm));
}

//this function will check each sphere passed to it

t_norm_color	color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_sphere		*sphere;
	t_vec3			norm;
	t_point			int_pnt;
	t_vec3			light_dir;
	double			light_int;

	sphere = (t_sphere *)closest->object;
	light_int = 0;
	if (trace->lights)
	{
		int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		norm = sp_normal_at(int_pnt, sphere->transform);
		if (dot_product(norm, r.dir) > 0)
			norm = neg(norm);
		light_dir = norm_vec(subtract_vec(trace->lights->center, int_pnt));
		if (!obscured(trace, int_pnt, light_dir, norm))
			light_int = trace->lights->brightness
				* get_splight_int(norm, light_dir, neg(r.dir));
	}
	return (get_final_color(trace, sphere->color, light_int));
}
