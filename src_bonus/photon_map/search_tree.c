#include "minirt.h"

// get dist
float	sq_dist(t_photon pho, float x, float y, float z)
{
	float distx;
	float disty;
	float distz;

	distx = (pho.pos.x - x) * (pho.pos.x - x);
	disty = (pho.pos.y - y) * (pho.pos.y - y);
	distz = (pho.pos.z - z) * (pho.pos.z - z);
	return (distx + disty + distz);
}

//init a list (photon map struct) to hold nodes in radius
//check if photon is with distanc of query pnt, add to list if true
//query tree for these and compute irradiance or radiance..
//use in get_final_color calc..
//rad2 = radius * radius

void	rad_search(t_kdnode *node, t_vec3 pnt, float rad2, t_photon_map *list)
{
	float	dist;
	int		axis;
	float	q_val;//
	float	p_val;

	if (!node || list->count >= list->size)
		return ;
	dist = sq_dist(node->photon, pnt.x, pnt.y, pnt.z);
	if (dist <= rad2 && list->count < list->size)
		list->photons[list->count++] =  node->photon;
	axis = node->split_dim;
	if (axis == 0)
	{
		q_val = pnt.x;
		p_val = node->photon.pos.x;
	}
	else if (axis == 1)
	{
		q_val = pnt.y;
		p_val = node->photon.pos.y;
	}
	else
	{
		q_val = pnt.z;
		p_val = node->photon.pos.z;
	}
	if (q_val < p_val)
	{
		rad_search(node->left, pnt, rad2, list);
		if ((p_val - q_val) * (p_val - q_val) <= rad2)
			rad_search(node->right, pnt, rad2, list);
	}
	else
	{
		rad_search(node->right, pnt, rad2, list);
		if ((p_val - q_val) * (p_val - q_val) <= rad2)
			rad_search(node->left, pnt, rad2, list);
	}
}

//compute using the total photon power divided by the search area..
//search area is of sphere using r = search radius.

t_norm_color	irradiance(t_photon_map *list, float area)
{
	float			totalr;
	float			totalg;
	float			totalb;
	t_norm_color	out;
	int				i;

	totalr = 0;
	totalb = 0;
	totalg = 0;
	i = -1;
	while (++i < list->count)
	{
		totalr += list->photons[i].r;
		totalg += list->photons[i].g;
		totalb += list->photons[i].b;
	}
	out.r = totalr / area;
	out.g = totalg / area;
	out.b = totalb / area;
	return (out);
}

t_norm_color irradiance_at(t_trace *trace, t_point pnt, t_kdnode *tree)
{
	t_norm_color	irrad;
	t_photon_map	*list;
	
	list = create_photon_map(trace->photnum);//create list for storing within radius must do this outside of threads put in pieces
	//protect list creation
	rad_search(tree, pnt, trace->rad2, list);//store those photons inside of radius
	irrad = irradiance(list, trace->area);// get irradiance using list. apply val in getfinalcolor3//place val in comps and compute there if needed
	free(list->photons);
	free(list);
	//printf("%f\n", irrad);
	return (irrad);
}