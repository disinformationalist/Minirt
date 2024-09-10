#include "minirt.h"

//this function will check each sphere passed to it

static inline bool check_solutions(double a, double b, double c, double *t)
{
	double	discrim;
	double	sol_1;
	double	sol_2;
	double	sqr_discrim;
	double	inv_2a;

	discrim = b * b - 4 * a * c;
	if (discrim < 0)
		return (false);
	sqr_discrim = sqrt(discrim);
	inv_2a = 0.5 / a;
	sol_1 = (-b - sqr_discrim) * inv_2a;
	if (sol_1 > 0)
	{
		*t = sol_1;
		return (true);
	}
	sol_2 = (-b + sqr_discrim) * inv_2a;
	if (sol_2 > 0)
	{
		*t = sol_2;
		return (true);
	}
	return (false);
}

bool	ray_sphere_intersect(t_sphere sphere, t_vec3 ray_dir, t_vec3 ray_orig, double *t)
{
	t_vec3	oc;//ray origin to center
	double	a;//coefficients of quadratic
	double	b;
	double	c;

	oc = subtract_vec(ray_orig, sphere.center);
	a = dot_product(ray_dir, ray_dir);
	b = 2.0 * dot_product(oc, ray_dir);
	c = dot_product(oc, oc) - sphere.radius * sphere.radius;
	if (check_solutions(a, b, c, t))
		return (true);
	return (false);
}

void	check_spheres(t_sphere *spheres, t_track_hits *closest, t_ray ray, double *t)
{
	t_sphere	*curr_sp;

	if (spheres == NULL)
		return ;
	curr_sp = spheres;

	while (true)
	{
		if (ray_sphere_intersect(*curr_sp, ray.dir, ray.origin, t))
		{
			if (*t < closest->t && *t > 0)
			{
				closest->t = *t;
				closest->object = curr_sp;
				closest->object_type = SPHERE;
			}
		}
		curr_sp = curr_sp->next;
		if (curr_sp == spheres)
			break;
	}
}

//diff plus specular for sp

float	get_light_int(t_vec3 norm, t_vec3 light_dir, t_vec3 view_dir)//, t_mat sphere->mat)
{
	t_vec3	ref;
	float	spec;
	float	light_int;
	float	cos_angle;

	cos_angle = dot_product(norm, light_dir);
	ref = subtract_vec(scale_vec(2 * cos_angle, norm), light_dir);
	spec = pow(fmax(dot_product(ref, view_dir), 0), 200);
	//spec = pow(fmax(dot_product(ref, view_dir), 0), mat.shine); //use these when mat is assigned
	light_int = .9 * fmax(cos_angle, 0.0) + .9 * spec;
	//light_int = mat.diff * fmax(cos_angle, 0.0) + mat.spec * spec;
	return (light_int);
}



unsigned int color_sphere(t_trace *trace, t_ray r, t_track_hits *closest)
{
	t_sphere		*sphere;
	t_point			int_pnt;//light intersect with surface
	t_vec3			norm;
	t_vec3			light_dir; 
	float			light_int;//total non amb lt intensity

	sphere = (t_sphere *)closest->object;

	if (trace->lights)// getting the light intensity at each intersection point
	{
		int_pnt = add_vec(r.origin, scale_vec(closest->t, r.dir));
		norm = norm_vec(subtract_vec(int_pnt, sphere->center));
		//loop here for multiple lights.
		light_dir = norm_vec(subtract_vec(trace->lights->center, int_pnt));
		if (dot_product(norm, r.dir) > 0)//this seems to work for correction.. moving light and cam inside of sphere
			norm = neg(norm);
		//hard shadows...
		if (obscured(trace, int_pnt, light_dir, norm))
			light_int = 0;
		else
			light_int = trace->lights->brightness * get_light_int(norm, light_dir, neg(r.dir));//diff + spec here
	}
	else
		light_int = 0;
	//sphere->color = stripe(int_pnt);//trying color function
	return (get_final_color(trace, sphere->color, light_int));
}

//cast shadow ray from intersect point toward light source
//s_ray.dir = norm(L - P) light cen - int point
//s.origin = P + e-6, prevent s_ray form intersecting surface of origination
//test intersects until light 
//light distance = mag(L -P)