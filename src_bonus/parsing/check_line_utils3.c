#include "minirt.h"

/* int	spheres_intersect(t_vec3 xyz_1, double r_1, t_vec3 xyz_2, double r_2)
{
	double	distance;
	double	rad_sum;
	double	rad_diff;

	distance = fabs(magnitude(subtract_vec(xyz_1, xyz_2)));
	rad_sum = r_1 + r_2;
	rad_diff = fabs(r_1 - r_2);
	if (distance >= rad_sum || distance <= rad_diff)
		return (0);
	return (1);
}

int	check_spheres_intersect(char *center_str_1, char *diam_str_1,
	char *center_str_2, char *diam_str_2)
{
	t_vec3		xyz_1;
	double		radius_1;
	t_vec3		xyz_2;
	double		radius_2;

	xyz_1.x = get_double(&center_str_1);
	center_str_1++;
	xyz_1.y = get_double(&center_str_1);
	center_str_1++;
	xyz_1.z = get_double(&center_str_1);
	radius_1 = get_double(&diam_str_1) / 2;
	xyz_2.x = get_double(&center_str_2);
	center_str_2++;
	xyz_2.y = get_double(&center_str_2);
	center_str_2++;
	xyz_2.z = get_double(&center_str_2);
	radius_2 = get_double(&diam_str_2) / 2;
	if (!spheres_intersect(xyz_1, radius_1, xyz_2, radius_2))
		return (1);
	return (0);
}
 */