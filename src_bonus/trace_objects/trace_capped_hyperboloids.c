#include "minirt.h"

static inline bool	check_trunk_solutions(t_vec3 abc, double *t1, double *t2)
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

// Equation: x * x / a * a + z * z / b * b - y * y / c * c = 1
// will need to parse a hyperboloid center, height, half_h, ray_max and ray_min

void	compute_abc_hy(t_vec3 *abc, t_ray ray, t_hyperboloid *hyperboloid)
{
	double	rad1_sq;
	double	rad2_sq;
	double	half_h_sq;
	
	rad1_sq = hyperboloid->rad1 * hyperboloid->rad1;
	rad2_sq = hyperboloid->rad2 * hyperboloid->rad2;
	half_h_sq = hyperboloid->half_h * hyperboloid->half_h;
	abc->x = (ray.dir.x * ray.dir.x) / rad1_sq
		+ (ray.dir.z * ray.dir.z) / rad2_sq
		- (ray.dir.y * ray.dir.y) / half_h_sq;
	abc->y = 2 * ((ray.origin.x * ray.dir.x) / rad1_sq
		+ (ray.origin.z * ray.dir.z) / rad2_sq
		- (ray.origin.y * ray.dir.y) / half_h_sq);
	abc->z = (ray.origin.x * ray.origin.x) / rad1_sq
		+ (ray.origin.z * ray.origin.z) / rad2_sq
		- (ray.origin.y * ray.origin.y) / half_h_sq - 0.01;
}

bool	check_cap_hy(t_ray ray, double t, t_hyperboloid *hyperboloid, double y)
{
	double	x;
	double	z;
	double	rad1;
	double	rad2;
	double	cap_rad_sq;

	rad1 = hyperboloid->rad1;
	rad2 = hyperboloid->rad2;
	cap_rad_sq = (y * y) / (hyperboloid->half_h * hyperboloid->half_h) + 0.01;
	x = ray.origin.x + t * ray.dir.x;
	z = ray.origin.z + t * ray.dir.z;
	if (((x * x) / (rad1 *rad1) + (z * z) / (rad2 *rad2)) <= cap_rad_sq)
		return (true);
	return (false);	
}

//later return false if cyl.closed == false

bool	intersect_caps_hy(t_ray ray, t_hyperboloid *hyperboloid, double *t3, double *t4)
{
	double	t;
	bool	hit1;
	bool	hit2;
	double	half_h;

	*t3 = INFINITY;
	*t4 = INFINITY;
	half_h = hyperboloid->half_h;
	if (fabs(ray.dir.y) < 1e-5)
		return (false);
	t = (-half_h - ray.origin.y) / ray.dir.y;
	hit1 = check_cap_hy(ray, t, hyperboloid, -half_h);
	if (hit1)
		*t4 = t;
	t = (half_h - ray.origin.y) / ray.dir.y;
	hit2 = check_cap_hy(ray, t, hyperboloid, half_h);
	if (hit2)
		*t3 = t;
	if (hit1 || hit2)
		return (true);
	return (false);
}

bool	within_height_hy(t_ray ray, double min, double max, double t)
{
	double y;
	
	if (t < 1e-5)
		return (false);
	y = ray.origin.y + t * ray.dir.y;
	if (y > min && y < max)
		return (true);
	return (false);
}

void	ray_hyperboloid_intersect(t_hyperboloid *hyperboloids, t_ray ray, t_intersects *intersects)
{
	t_vec3	abc;
	double	t1;
	double	t2;
	double 	t3;
	double 	t4;
	double	half_h;

	half_h = hyperboloids->half_h;
	ray = transform(ray, hyperboloids->transform);
	compute_abc_hy(&abc, ray, hyperboloids);
	if (abc.x != 0)
	{
		if (check_trunk_solutions(abc, &t1, &t2))
		{
			if (within_height_hy(ray, -half_h, half_h, t1))
				intersect(intersects, hyperboloids, t1, HYPERBOLOID);
			if (within_height_hy(ray, -half_h, half_h, t2))
				intersect(intersects, hyperboloids, t2, HYPERBOLOID);
		}
	}
	if (intersect_caps_hy(ray, hyperboloids, &t3, &t4))
	{
		if (t3 < INFINITY)
			intersect(intersects, hyperboloids, t3, HYPERBOLOID);
		if (t4 < INFINITY)
			intersect(intersects, hyperboloids, t4, HYPERBOLOID);
	}
}

void	check_hyperboloids(t_hyperboloid *hyperboloids, t_intersects *intersects, t_ray ray)
{
	t_hyperboloid	*curr_hy;

	if (hyperboloids == NULL)
		return ;
	curr_hy = hyperboloids;
	while (true)
	{
		ray_hyperboloid_intersect(curr_hy, ray, intersects);
		curr_hy = curr_hy->next;
		if (curr_hy == hyperboloids)
			break;
	}
}
