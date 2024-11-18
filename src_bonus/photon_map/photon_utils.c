#include "minirt.h"

t_photon photon(t_vec3 pos, t_vec3 dir, t_norm_color power)
{
	t_photon photon;

	photon.pos.x = (float)pos.x;
	photon.pos.y = (float)pos.y;
	photon.pos.z = (float)pos.z;
	photon.dir.x = (float)dir.x;
	photon.dir.y = (float)dir.y;
	photon.dir.z = (float)dir.z;
	photon.r = power.r;
	photon.g = power.g;
	photon.b = power.b;
	return (photon);
}

t_vec3f	vecf(float x, float y, float z)//, float w)
{
	t_vec3f v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

//rand double between -1 and 1
/* 
double	randf2(void)
{
	double hrmax = ((double)RAND_MAX / 2.0);//precompute this!!
	return (((double)rand() / hrmax) - 1.0);//check if double on rand needed
}
 */
//puts a photon into map, adjusts count

void	add_photon(t_photon_map *map, t_photon photon)
{
	if (map->count < map->size)
	{
		map->photons[map->count] = photon;
		map->count++;
	}
	else
		printf("map has reach capacity");
}

t_ray pho_ray(t_photon photon)
{
	t_ray pho;

	pho.dir.x = (double)photon.dir.x;
	pho.dir.y = (double)photon.dir.y;
	pho.dir.z = (double)photon.dir.z;
	pho.dir.w = 0;
	pho.origin.x = (double)photon.pos.x;
	pho.origin.y = (double)photon.pos.y;
	pho.origin.z = (double)photon.pos.z;
	pho.origin.w = 1;
	return (pho);
}
