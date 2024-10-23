#include "minirt.h"

bool	hit_allowed(t_csg_op op, bool lhit, bool inl, bool inr)
{
	if (op == UNION)
		return ((lhit && !inr) || (!lhit && !inl));
	else if (op == INTERSECTION)
		//return ((lhit && inr) || (!lhit && inl));
		return ((lhit && inl && inr) || (lhit && !inl && inr) || (!lhit && inl && inr) || (!lhit && inl && !inr));
	else if (op == DIFFERENCE)
		return ((lhit && !inr) || (!lhit && inl));
	return (false);
}

t_intersects	*filter_intersections(t_csg *csg, t_intersects *intersects, t_ray ray)
{
	t_intersects	*filtered;
	bool			inl;
	bool			inr;
	bool			lhit;
	int				i;
	(void) ray;

	//printf("intersects->count before: %d", intersects->count);
	filtered = malloc(sizeof(t_intersects));
	if (!filtered)
		return (NULL); //TODO manage memory
	filtered->hits = malloc(intersects->count * sizeof(t_track_hits));
	if (!filtered->hits)
	{
		free(filtered);
		return (NULL); //TODO manage memory
	}
	filtered->count = 0;
	inl = false;
	inr = false;
	i = 0;
	while (i < intersects->count)
	{
		lhit = (intersects->hits[i].object == csg->left);
		if (lhit)
			inl = !inl;
		if (intersects->hits[i].object == csg->right)
			inr = !inr;
		printf("csg->op: %d\n", csg->op);
		printf("lhit: %d, inl: %d, inr: %d\n", lhit, inl, inr);
		if (hit_allowed(csg->op, lhit, inl, inr))
		{
			filtered->hits[filtered->count] = intersects->hits[i];
			filtered->count++;
			printf("filtered->count after: %d", filtered->count);
		}
		i++;
	}
	intersects = filtered;
	intersects->hits = filtered->hits;
	intersects->count = filtered->count;
	return (intersects);
}

static inline bool check_solutions(t_vec3 abc, double *t1, double *t2)//change  store vals
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

void	intersect_csg(t_intersects *intersects, void *object, double t, t_type type)
{
	int i;
	(void) object;
	(void) type;

	i = intersects->count - 1;
	while (i >= 0 && intersects->hits[i].t > t)
	{
		intersects->hits[i + 1] = intersects->hits[i];
		i--;
	}
	i++;
	intersects->hits[i].t = t;
	intersects->hits[i].object = object;
	intersects->hits[i].object_type = type;
	intersects->count++;
}

bool	is_inside_the_other(t_sphere *sphere2, t_ray ray, double t)
{
	t_vec3	intersect_point;
	t_vec3	to_center;
	double	distance_squared;
	double	radius_squared;
	t_ray	transformed_ray;

	distance_squared = 0.0;
	radius_squared = 0.0;
	transformed_ray = transform(ray, inverse(sphere2->transform));
	intersect_point = add_vec(transformed_ray.origin, scale_vec(t, transformed_ray.dir));
	to_center = subtract_vec(intersect_point, sphere2->center);
	distance_squared = dot_product(to_center, to_center);
	radius_squared = sphere2->radius * sphere2->radius;
	return (distance_squared <= radius_squared + 1e-5);
}

void	ray_sphere_intersect_csg(t_csg *csg, t_ray ray, t_intersects *intersects)
{
	double	t1;
	double	t2;
	double	a;
	double	b;
	double	c;
	t_sphere	*sphere1;
	t_sphere	*sphere2;
	t_ray original_ray;

	sphere1 = (t_sphere *)csg->left;
	sphere2 = (t_sphere *)csg->right;
	original_ray = ray;
	ray = transform(ray, sphere1->transform);
	a = dot_product(ray.dir, ray.dir);
	b = 2.0 * dot_product(ray.origin, ray.dir);
	c = dot_product(ray.origin, ray.origin) - 1;
	if (check_solutions(vec(a, b, c, 0), &t1, &t2))
	{
		if (is_inside_the_other(sphere2, ray, t1))
			intersect_csg(intersects, sphere1, t1, SPHERE);
		if (is_inside_the_other(sphere2, ray, t2))
			intersect_csg(intersects, sphere1, t2, SPHERE);
	}
	ray = transform(original_ray, sphere2->transform);
	a = dot_product(ray.dir, ray.dir);
	b = 2.0 * dot_product(ray.origin, ray.dir);
	c = dot_product(ray.origin, ray.origin) - 1;
	if (check_solutions(vec(a, b, c, 0), &t1, &t2))
	{
		if (is_inside_the_other(sphere1, ray, t1))
			intersect_csg(intersects, sphere2, t1, SPHERE);
		if (is_inside_the_other(sphere1, ray, t2))
			intersect_csg(intersects, sphere2, t2, SPHERE);
	}
	intersects = filter_intersections(csg, intersects, ray);
}

void	get_csg_intersections(t_csg *csg, t_intersects *intersects, t_ray ray)
{

	//ray_sphere_intersect_csg((t_sphere *)csg->left, ray, intersects);
	//ray_sphere_intersect_csg((t_sphere *)csg->right, ray, intersects);
	//ray_sphere_intersect_csg((t_sphere *)csg->left, (t_sphere *)csg->right, csg->op, ray, intersects);
	//ray_sphere_intersect_csg((t_sphere *)csg->right, (t_sphere *)csg->left, csg->op, ray, intersects);
	//ray_sphere_intersect_csg_1(csg, ray, intersects);
	//ray_sphere_intersect_csg_2(csg, ray, intersects);
	ray_sphere_intersect_csg(csg, ray, intersects);
}

t_csg	*make_new_csg(t_sphere *left, t_sphere *right, t_csg_op op)
{
	t_csg	*new_csg;

	new_csg = (t_csg*)malloc(sizeof(t_csg));
	if (!new_csg)
		return (NULL); //TODO: handle malloc failure
	new_csg->left = left;
	new_csg->right = right;
	new_csg->op = op;
	return (new_csg);
}

void	check_csg_test(t_sphere *spheres, t_intersects *intersects, t_ray ray)
{
	t_csg		*test_csg;

	if (spheres == NULL)
		return ;
	test_csg = make_new_csg(spheres, spheres->next, INTERSECTION);
	get_csg_intersections(test_csg, intersects, ray);
	//filter_intersections(test_csg, intersects, ray);
	free(test_csg);
}

/* static inline bool check_solutions(t_vec3 abc, double *t1, double *t2)//change  store vals
{
	double	discrim;
	double	inv_2a;
	double 	sq_discrim;
	double	sol_1;
	double	sol_2;
	
	discrim = abc.y * abc.y - 4 * abc.x * abc.z;
	if (discrim < 1e-5)
		return (false);
	sq_discrim = sqrt(discrim);
	inv_2a = 0.5 / abc.x;
	sol_1 = (-abc.y - sq_discrim) * inv_2a;
	if (sol_1 > 0)
	{
		*t1 = sol_1;
		return (true);
	}
	sol_2 = (-abc.y + sq_discrim) * inv_2a;
	if (sol_2 > 0)
	{
		*t2 = sol_2;
		return (true);
	}
	return (false);
}

static inline bool check_solutions(t_vec3 abc, double *t1, double *t2)//change  store vals
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

void	intersect_csg(t_intersects *intersects, void *object, double t, t_type type)
{
	int i;
	(void) object;
	(void) type;

	i = intersects->count - 1;
	while (i >= 0 && intersects->hits[i].t > t)
	{
		intersects->hits[i + 1] = intersects->hits[i];
		i--;
	}
	i++;
	intersects->hits[i].t = t;
	intersects->hits[i].object = object;
	intersects->hits[i].object_type = type;
	intersects->count++;
}

bool	is_inside_the_other(t_sphere *sphere2, t_ray ray, double t)
{
	t_vec3	intersect_point;
	t_vec3	to_center;
	double	distance_squared;
	double	radius_squared;
	t_ray	transformed_ray;

	distance_squared = 0.0;
	radius_squared = 0.0;
	transformed_ray = transform(ray, inverse(sphere2->transform));
	intersect_point = add_vec(transformed_ray.origin, scale_vec(t, transformed_ray.dir));
	to_center = subtract_vec(intersect_point, sphere2->center);
	distance_squared = dot_product(to_center, to_center);
	radius_squared = sphere2->radius * sphere2->radius;
	printf("Intersect Point: (%f, %f, %f)\n", intersect_point.x, intersect_point.y, intersect_point.z);
	printf("Distance Squared: %f, Radius Squared: %f\n", distance_squared, radius_squared);
	return (distance_squared <= radius_squared + 1e-5);
}

void	ray_sphere_intersect_csg(t_sphere *sphere1, t_sphere *sphere2, t_csg_op op, t_ray ray, t_intersects *intersects)
{
	double	t1;
	double	t2;
	double	a;
	double	b;
	double	c;
	(void) op;

	ray = transform(ray, sphere1->transform);
	a = dot_product(ray.dir, ray.dir);
	b = 2.0 * dot_product(ray.origin, ray.dir);
	c = dot_product(ray.origin, ray.origin) - 1;
	if (check_solutions(vec(a, b, c, 0), &t1, &t2))
	{
		if (is_inside_the_other(sphere2, ray, t1))
			intersect_csg(intersects, sphere1, t1, SPHERE);
		if (is_inside_the_other(sphere2, ray, t2))
			intersect_csg(intersects, sphere1, t2, SPHERE);
	}
}

void	ray_sphere_intersect_csg_1(t_csg *csg, t_ray ray, t_intersects *intersects)
{
	double	t1;
	double	t2;
	double	a;
	double	b;
	double	c;
	t_sphere *sphere1;
	t_sphere *sphere2;

	sphere1 = (t_sphere *)csg->left;
	sphere2 = (t_sphere *)csg->right;
	ray = transform(ray, sphere1->transform);
	a = dot_product(ray.dir, ray.dir);
	b = 2.0 * dot_product(ray.origin, ray.dir);
	c = dot_product(ray.origin, ray.origin) - 1;
	if (check_solutions(vec(a, b, c, 0), &t1, &t2))
	{
		if (!is_inside_the_other(sphere2, ray, t1))
			intersect_csg(intersects, sphere1, t1, SPHERE);
		if (is_inside_the_other(sphere2, ray, t2))
			intersect_csg(intersects, sphere1, t2, SPHERE);
	}
}

void	ray_sphere_intersect_csg_2(t_csg *csg, t_ray ray, t_intersects *intersects)
{
	double	t1;
	double	t2;
	double	a;
	double	b;
	double	c;
	t_sphere *sphere1;
	t_sphere *sphere2;

	sphere2 = (t_sphere *)csg->left;
	sphere1 = (t_sphere *)csg->right;
	ray = transform(ray, sphere1->transform);
	a = dot_product(ray.dir, ray.dir);
	b = 2.0 * dot_product(ray.origin, ray.dir);
	c = dot_product(ray.origin, ray.origin) - 1;
	if (check_solutions(vec(a, b, c, 0), &t1, &t2))
	{
		if (is_inside_the_other(sphere2, ray, t1))
			intersect_csg(intersects, sphere1, t1, SPHERE);
		if (is_inside_the_other(sphere2, ray, t2))
			intersect_csg(intersects, sphere1, t2, SPHERE);
	}
}

//add an intersection to the list

void	get_csg_intersections(t_csg *csg, t_intersects *intersects, t_ray ray)
{

	//ray_sphere_intersect_csg((t_sphere *)csg->left, ray, intersects);
	//ray_sphere_intersect_csg((t_sphere *)csg->right, ray, intersects);
	//ray_sphere_intersect_csg((t_sphere *)csg->left, (t_sphere *)csg->right, csg->op, ray, intersects);
	//ray_sphere_intersect_csg((t_sphere *)csg->right, (t_sphere *)csg->left, csg->op, ray, intersects);
	ray_sphere_intersect_csg_1(csg, ray, intersects);
	ray_sphere_intersect_csg_2(csg, ray, intersects);
}

t_csg	*make_new_csg(t_sphere *left, t_sphere *right, t_csg_op op)
{
	t_csg	*new_csg;

	new_csg = (t_csg*)malloc(sizeof(t_csg));
	if (!new_csg)
		return (NULL); //TODO: handle malloc failure
	new_csg->left = left;
	new_csg->right = right;
	new_csg->op = op;
	return (new_csg);
}

void	check_csg_test(t_sphere *spheres, t_intersects *intersects, t_ray ray)
{
	t_csg		*test_csg;

	if (spheres == NULL)
		return ;
	test_csg = make_new_csg(spheres, spheres->next, INTERSECTION);
	get_csg_intersections(test_csg, intersects, ray);
	free(test_csg);
}
 */