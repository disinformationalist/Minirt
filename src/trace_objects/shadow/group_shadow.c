#include "minirt.h"

static inline bool	check_cap(t_ray r, double t)
{
	double	x;
	double	z;

	x = r.origin.x + t * r.dir.x;
	z = r.origin.z + t * r.dir.z;
	if ((x * x + z * z) <= 1)
		return (true);
	return (false);
}

static inline bool	intersect_caps(t_ray ray, double dist, bool flag)
{
	double	t;
	bool	hit;

	if (fabs(ray.dir.y) < 1e-6)
		return (false);
	if (!flag)
	{
		t = (-1 - ray.origin.y) / ray.dir.y;
		hit = check_cap(ray, t);
		if (hit && t > 0 && t < dist)
			return (true);
	}
	t = (1 - ray.origin.y) / ray.dir.y;
	hit = check_cap(ray, t);
	if (hit && t > 0 && t < dist)
		return (true);
	return (false);
}

static inline bool	within_height(t_ray r, double t, bool flag)
{
	double	y;
	double	lim;

	if (t < 1e-6)
		return (false);
	if (flag)
		lim = 0;
	else
		lim = -1;
	y = r.origin.y + t * r.dir.y;
	if (y > lim && y < 1)
		return (true);
	return (false);
}

static inline bool	check_trunk_solutions(t_vec3 abc, \
t_ray ray, double dist, bool flag)
{
	double	discrim;
	double	inv_2a;
	double	sq_discrim;
	double	t;

	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-5)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	t = (-abc.y - sq_discrim) * inv_2a;
	if (within_height(ray, t, flag) && t < dist)
		return (true);
	t = (-abc.y + sq_discrim) * inv_2a;
	if (within_height(ray, t, flag) && t < dist)
		return (true);
	return (false);
}

static inline void	compute_abc(t_vec3 *abc, t_ray r, double waist)
{
	double	factor;

	factor = waist + 1;
	abc->x = (r.dir.x * r.dir.x + r.dir.z * r.dir.z) * factor - r.dir.y * r.dir.y;
	abc->y = 2 * ((r.origin.x * r.dir.x + r.origin.z * r.dir.z) * factor - r.origin.y * r.dir.y);
	abc->z = (r.origin.x * r.origin.x + r.origin.z * r.origin.z) * factor - r.origin.y * r.origin.y - waist;
}

static inline bool	ray_hype_intersect2(t_hyperboloid hype, \
t_ray ray, double dist, bool flag)
{
	t_vec3	abc;

	ray = transform(ray, hype.transform);
	compute_abc(&abc, ray, hype.waist3);
	if (check_trunk_solutions(abc, ray, dist, flag))
		return (true);
	if (hype.caps && intersect_caps(ray, dist, flag))
		return (true);
	return (false);
}

static inline bool	check_solutions2(double a, double b, double c, double dist)
{
	double	discrim;
	double	inv_2a;
	double	sq_discrim;
	double	sol;

	discrim = b * b - 4 * a * c;
	if (discrim < 1e-5)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / a;
	sol = (-b - sq_discrim) * inv_2a;
	if (sol > 0 && sol < dist)
		return (true);
	sol = (-b + sq_discrim) * inv_2a;
	if (sol > 0 && sol < dist)
		return (true);
	return (false);
}

static inline bool	ray_sphere_intersect2(t_sphere sphere, \
t_ray ray, double dist)
{
	double	a;
	double	b;
	double	c;

	ray = transform(ray, sphere.transform);
	a = dot_product(ray.dir, ray.dir);
	b = 2.0 * dot_product(ray.origin, ray.dir);
	c = dot_product(ray.origin, ray.origin) - 1;
	if (check_solutions2(a, b, c, dist))
		return (true);
	return (false);
}

static inline bool	ray_plane_intersect2(t_plane plane, t_ray ray, double dist)
{
	double	t;

	ray = transform(ray, plane.transform);
	if (fabs(ray.dir.y) < 1e-6)
		return (false);
	t = -ray.origin.y / ray.dir.y;
	if (t > 0 && t < dist)
		return (true);
	return (false);
}


static inline void	check_axis2(double origin, double dir, \
double *min, double *max)
{
	double	t_min_num;
	double	t_max_num;

	t_min_num = -(1 + origin);
	t_max_num = (1 - origin);
	if (fabs(dir) > 0)
	{
		*min = t_min_num / dir;
		*max = t_max_num / dir;
	}
	else
	{
		*min = t_min_num * INFINITY;
		*max = t_max_num * INFINITY;
	}
	if (*min > *max)
		ft_swap(min, max);
}

static inline bool	ray_cube_intersect2(t_mat4 ctransform, \
t_ray ray, double dist)
{
	t_vec3	mins;
	t_vec3	maxs;
	double	t_min;
	double	t_max;

	ray = transform(ray, ctransform);
	check_axis2(ray.origin.x, ray.dir.x, &mins.x, &maxs.x);
	check_axis2(ray.origin.y, ray.dir.y, &mins.y, &maxs.y);
	check_axis2(ray.origin.z, ray.dir.z, &mins.z, &maxs.z);
	t_min = fmax(fmax(mins.x, mins.y), mins.z);
	t_max = fmin(fmin(maxs.x, maxs.y), maxs.z);
	if (t_min > t_max)
		return (false);
	if ((t_min > 0 && t_min < dist) || (t_max > 0 && t_max < dist))
		return (true);
	return (false);
}

//place all shadow check fts above then use in checks here and return bool

bool	ray_obj_dist(t_shape *curr, t_ray ray, double dist)
{
	if (curr->type == SPHERE)
	{
		t_sphere *sp = (t_sphere *)curr->shape;
		if (sp->shadow && ray_sphere_intersect2(*sp, ray, dist))
			return (true);
	}
	else if (curr->type == CYLINDER)
	{
		t_cylinder *cy = (t_cylinder *)curr->shape;
		if (cy->shadow && ray_cylinder_intersect2(*cy, ray, dist))
			return (true);
	}
	else if (curr->type == HYPERBOLOID)
	{
		t_hyperboloid *hy = (t_hyperboloid *)curr->shape;
		if (hy->shadow && ray_hype_intersect2(*hy, ray, dist, hy->single))
			return (true);
	}
	else if (curr->type == CUBE)
	{
		t_cube *cu = (t_cube *)curr->shape;
		if (cu->shadow && ray_cube_intersect2(cu->transform, ray, dist))
			return (true);
	}
	else if (curr->type == PLANE)
	{
		t_plane *pl = (t_plane *)curr->shape;
		if (pl->shadow && ray_plane_intersect2(*pl, ray, dist))
			return (true);
	}
	else
		return (false);
	return (false);
}

//function to check the intersections of a group

bool	check_group_dist(t_group *group, double dist, t_ray ray)
{
	t_shape	*curr;

	if (!group->shapes)
		return (false);
	ray = transform(ray, group->tran);
	curr = group->shapes;
	while (true)
	{
		if (curr->type == GROUP)
			check_group_dist((t_group *)curr, dist, ray);
		else if (ray_obj_dist(curr, ray, dist))
			return (true);
		curr = curr->next;
		if (curr == group->shapes)
			break ;
	}
	return (false);
}

//check_hierarchy() recursively shadow checks on a bvh.

bool	check_hier_dist(t_group *top, double dist, t_ray ray)
{
	t_shape	*curr;

	if (!top || !top->shapes)
		return (false);
	if (!ray_box_intersect(top->box, top->tran, ray))
		return (false);
	curr = top->shapes;
	while (true)
	{
		if (curr->type == GROUP)
		{
			if (check_hier_dist((t_group *)curr, dist, ray))
				return (true);
		}
		else if (ray_obj_dist(curr, ray, dist))
			return (true);
		curr = curr->next;
		if (curr == top->shapes)
			break ;
	}
	return (false);
}