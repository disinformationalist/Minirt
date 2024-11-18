#include "minirt.h"

//random between -1 and 1

double	randf2(void)
{
	double hrmax = ((double)RAND_MAX / 2.0);//precompute this!! and use bitshift
	return (((double)rand() / hrmax) - 1.0);//check if double on rand needed
}

int	ft_round(double num)
{
	return((int)floor(num + 0.5));
}

//random between 0 and 1

double	randf(void)
{
	return ((double)rand() / (double)(RAND_MAX + 1.0));
}
