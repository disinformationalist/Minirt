#include "minirt.h"

//random between -1 and 1
//precomp hrmax?

double	randf2(void)
{
	double	hrmax;

	hrmax = ((double)RAND_MAX / 2.0);
	return (((double)rand() / hrmax) - 1.0);
}

int	ft_round(double num)
{
	return ((int)floor(num + 0.5));
}

//random between 0 and 1

double	randf(void)
{
	return ((double)rand() / (double)(RAND_MAX + 1.0));
}

// Returns a random [min,max).
double random_double(double min, double max) 
{
    return min + (max - min) * randf();
}

// between 0 and 1 for each

t_vec3	rand_vec(void)
{
	return (vec(randf(), randf(), randf(), 0));
}

//between min and max for each

t_vec3 random_vec(double min, double max)
{
	return (vec(random_double(min, max), random_double(min, max), random_double(min, max), 1));
}

t_vec3 random_unit_vec()
{
    t_vec3 vec;

	while (true)
	{
        vec = random_vec(-1,1);
        double lensq = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
        if (1e-160 < lensq && lensq <= 1)
            return (div_vec(sqrt(lensq), vec));
    }
}

t_vec3 random_on_hemi(t_vec3 norm)
{
    t_vec3 vec;

	vec = random_unit_vec();
	if (dot_product(vec, norm) > 0.0)
		return (vec);
	else
		return (neg(vec));
}


void	error_exit(char *msg)
{
	ft_putstr_color_fd(2, msg, RED);
	exit(EXIT_FAILURE);
}
