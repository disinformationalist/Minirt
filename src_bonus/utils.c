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

void	error_exit(char *msg)
{
	ft_putstr_color_fd(2, msg, RED);
	exit(EXIT_FAILURE);
}
