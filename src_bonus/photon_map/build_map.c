#include "minirt.h"

/* using the photon count scaling method, this keeps photon power even. best.
1)norm across lights the brightness, ltweight = lt_bright / total_bright.
2)photon allocation is weighted by brightness, lt_photon num = total * ltweight
3)profit */

t_photon_map *create_photon_map(int size)
{
	t_photon_map *new;

	new = (t_photon_map *)malloc(sizeof(t_photon_map));
	if (!new)
		return (NULL);
	new->photons = (t_photon *)malloc(size * sizeof(t_photon));
	if (!new->photons)
	{
		free(new);
		return (NULL);
	}
	new->count = 0;
	new->size = size;
	return (new);
}

//adjust allocations to prevent rounding errors creating segfault later

void	correct_rounding(t_light *lt, int remain)
{
	int	adjust;

	if (remain == 0)
		return ;
	if (remain > 0)
		adjust = 1;
	else
		adjust = -1;
	while (remain != 0)
	{
		lt->photons += adjust;
		remain -= adjust;
		lt = lt->next;
	}
}

static inline double get_contribution(t_light *lt)
{
	double con;

	if (lt->type == AREA)
		con = lt->brightness * lt->area;
	else if (lt->type == POINT)
		con = lt->brightness;
	//handle spot corretly here
	else
		return (0);
	return (con);
}

//allocate the photons to the lights based on their intensity
//SPOT LIGHTS  will need special handling here. prop based on angles and inten, only emitted in angles.

void	allocate_photons(t_light *lights, int tot_phot)
{
	t_light	*curr_lt;
	double 	sum;
	int		remain;

	sum = 0.0;
	remain = tot_phot;
	curr_lt = lights;	
	if (lights == NULL || tot_phot <= 0)
		return ;
	while (true)
	{
		sum += get_contribution(lights);
		curr_lt = curr_lt->next;
		if (curr_lt == lights)
			break ;
	}
	while (true)
	{
		curr_lt->photons = ft_round((curr_lt->brightness / sum) * tot_phot);
		remain -= curr_lt->photons;
		curr_lt = curr_lt->next;
		if (curr_lt == lights)
			break ;
	}
	correct_rounding(lights, remain);
}

//loop through lights each contributing to the map, takes the total number of photons to be mapped

t_photon_map	*build_map(t_trace *trace, int photons)
{
	t_light			*curr_lt;
	t_photon_map	*photon_map;

	photon_map = create_photon_map(photons * 3);
	if (!photon_map)
		return (NULL);
	allocate_photons(trace->lights, photons);
	curr_lt = trace->lights;
	while (true)
	{
		if (curr_lt->type == POINT)
			emit_point(trace, photon_map, curr_lt->photons, curr_lt);
		else if (curr_lt->type == AREA)
			emit_area(trace, photon_map, *curr_lt, curr_lt->photons);
		/* if (curr_lt->type == SPOT)
			emit_spot(trace, photon_map, curr_lt->photons, curr_lt); */
		curr_lt = curr_lt->next;
		if (curr_lt == trace->lights)
			break ;
	}
	return (photon_map);
}
