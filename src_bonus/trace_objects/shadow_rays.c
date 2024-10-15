#include "minirt.h"

 static inline bool check_solutions2(t_vec3 abc, double *t1, double *t2)//change  store vals
{
	double	discrim;
	double	inv_2a;
	double 	sq_discrim;
	
	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-5)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	*t1 = (-abc.y - sq_discrim) * inv_2a;
	*t2 = (-abc.y + sq_discrim) * inv_2a;
	return (true);
}

//using object space

bool	ray_sphere_intersect2(t_sphere sphere, t_ray ray, double *t)//do opti return as original
{
	double	t1;
	double	t2;
	double	a;
	double	b;
	double	c;

	ray = transform(ray, sphere.transform);
	a = dot_product(ray.dir, ray.dir);
	b = 2.0 * dot_product(ray.origin, ray.dir);
	c = dot_product(ray.origin, ray.origin) - 1;
	if (check_solutions2(vec(a, b, c, 0), &t1, &t2))
	{
		if (t1 > 0 && t1 < *t)
			*t = t1;
		if (t2 > 0 && t2 < *t)
			*t = t2;
	}
	if (*t < INFINITY)
		return (true);
	return (false);
}

//check spheres for intersection within dist

static inline bool	check_sp_dist(t_sphere *spheres, double dist, t_ray ray, double t)
{
	t_sphere	*curr_sp;

	if (spheres == NULL)
		return (false);
	curr_sp = spheres;
	while (true)
	{
		if (ray_sphere_intersect2(*curr_sp, ray, &t))
		{
			if (t < dist)
				return (true);
		}
		curr_sp = curr_sp->next;
		if (curr_sp == spheres)
			break;
	}
	return (false);
}


bool	ray_plane_intersect2(t_plane plane, t_ray ray, double *t)
{
	ray = transform(ray, plane.transform);

	if (fabs(ray.dir.y) < 1e-6)
		return (false);
	*t = -ray.origin.y / ray.dir.y;
	if (*t > 0)
		return (true);
	return (false);
}

//check planes for intersection within dist

static inline bool	check_pl_dist(t_plane *planes, double dist, t_ray ray, double t)
{
	t_plane	*curr_pl;

	if (planes == NULL)
		return (false);
	curr_pl = planes;
	while (true)
	{
		if (ray_plane_intersect2(*curr_pl, ray, &t))
		{
			if (t < dist)
				return (true);
		}
		curr_pl = curr_pl->next;
		if (curr_pl == planes)
			break;
	}
	return (false);
}


static inline bool	check_trunk_solutions2(t_vec3 abc, double *t1, double *t2)
{
	double	discrim;
	double	inv_2a;
	double 	sq_discrim;
	
	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-5)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	*t1 = (-abc.y - sq_discrim) * inv_2a;
	*t2 = (-abc.y + sq_discrim) * inv_2a;
	/* if (*t1 > *t2)
		ft_swap(t1, t2); */
	return (true);
} 

void	compute_abc2(t_vec3 *abc, t_ray ray)
{
	abc->x = ray.dir.x * ray.dir.x + ray.dir.z * ray.dir.z;
	abc->y = 2 * (ray.origin.x * ray.dir.x + ray.origin.z * ray.dir.z);
	abc->z = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - 1;
}

bool	check_cap2(t_ray ray, double t)
{
	double x;
	double z;

	x = ray.origin.x + t * ray.dir.x;
	z = ray.origin.z + t * ray.dir.z;
	if ((x * x + z * z) <= 1)
		return (true);
	return (false);	
}

//later return false if cyl.closed == false

bool	intersect_caps2(t_ray ray, double half_h, double *t3, double *t4)
{
	double	t;
	bool	hit1;
	bool	hit2;

	*t3 = INFINITY;
	*t4 = INFINITY;
	if (fabs(ray.dir.y) < 1e-5)
		return (false);
	t = (-half_h - ray.origin.y) / ray.dir.y;
	hit1 = check_cap2(ray, t);
	if (hit1)
		*t4 = t;
	t = (half_h - ray.origin.y) / ray.dir.y;
	hit2 = check_cap2(ray, t);
	if (hit2)
		*t3 = t;
	if (hit1 || hit2)
		return (true);
	return (false);
}

bool	within_height2(t_ray ray, double min, double max, double t)
{
	double y;
	
	if (t < 1e-5)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	if (y > min && y < max)
		return (true);
	return (false);
}

bool	ray_cylinder_intersect2(t_cylinder cylinder, t_ray ray, double *t)
{
	t_vec3	abc;
	double	t1;
	double	t2;
	double 	t3;
	double 	t4;
	double	half_h;

	half_h = cylinder.height / 2.0;// store this as cyl height  at parse?
	ray = transform(ray, cylinder.transform);
	compute_abc2(&abc, ray);
	if (abc.x != 0)
	{
		if (check_trunk_solutions2(abc, &t1, &t2))
		{
			if (within_height2(ray, -half_h, half_h, t1) && t1 < *t)
					*t = t1;
			if (within_height2(ray, -half_h, half_h, t2) && t2 < *t)
					*t = t2;
		}
	}
	if (intersect_caps2(ray, half_h, &t3, &t4))
	{
		if (t3 > 0 && t3 < *t)
			*t = t3;
		if (t4 > 0 && t4 < *t)
			*t = t4;
	}
	if (*t < INFINITY)
		return (true);
	return (false);
}

//check cylinders for intersection within dist

static inline bool	check_cy_dist(t_cylinder *cylinders, double dist, t_ray ray, double t)
{
	t_cylinder	*curr_cy;

	if (cylinders == NULL)
		return (false);
	curr_cy = cylinders;
	while (true)
	{
		if (ray_cylinder_intersect2(*curr_cy, ray, &t))
		{
			if (t < dist)
				return (true);
		}
		curr_cy = curr_cy->next;
		if (curr_cy == cylinders)
			break;
	}
	return (false);
}

static inline bool	check_le_dist(t_lens *lenses, double dist, t_ray ray, double t)
{
	t_lens	*curr_le;

	if (lenses == NULL)
		return (false);
	curr_le = lenses;
	while (true)
	{
		if (ray_lens_intersect(*curr_le, ray, &t))
		{
			if (t < dist)
				return (true);
		}
		curr_le = curr_le->next;
		if (curr_le == lenses)
			break;
	}
	return (false);
}

//check it any object in any list blocks light

static inline bool	check_all_dist(t_trace *trace, double dist, t_ray s_ray)
{
	bool	hit;
	double	t;

	t = INFINITY;
	hit = check_sp_dist(trace->spheres, dist, s_ray, t);
	if (hit)
		return (true);
	t = INFINITY;
	hit = check_pl_dist(trace->planes, dist, s_ray, t);
	if (hit)
		return (true);
	t = INFINITY;
	hit = check_cy_dist(trace->cylinders, dist, s_ray, t);
	if (hit)
		return (true);
	t = INFINITY;
	hit = check_le_dist(trace->lenses, dist, s_ray, t);
	if (hit)
		return (true);
	return (false);
}

//function to check if any object blocks light

bool	obscured(t_trace *trace, t_point pnt, t_vec3 light_dir, t_vec3 normal)
{
	t_ray	s_ray;
	double	light_dist;

	s_ray.dir = light_dir;
	s_ray.origin = add_vec(pnt, scale_vec(1e-6, normal));	
	light_dist = magnitude(subtract_vec(trace->lights->center, pnt));// use in previous to get light dir test times.
	return (check_all_dist(trace, light_dist, s_ray));
}

//in bonus must use the center of the correct light.
bool	obscured_b(t_trace *trace, t_ray s_ray, t_point lt_pos, t_point int_pnt)
{
	double	light_dist;

	light_dist = magnitude(subtract_vec(lt_pos, int_pnt));
	return (check_all_dist(trace, light_dist, s_ray));
}