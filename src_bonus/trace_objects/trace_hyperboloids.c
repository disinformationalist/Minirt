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
		- (ray.origin.y * ray.origin.y) / half_h_sq - 1;
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
