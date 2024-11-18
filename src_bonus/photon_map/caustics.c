#include "minirt.h"

t_vec3	normal_align(t_vec3 dir, t_vec3 norm)
{
	t_vec3	tan;
	t_vec3	bitan;
	t_vec3	tool;
	t_vec3	v;

	if (fabs(norm.y) > .9)
		tool = vec(1, 0, 0, 0);
	else
		tool = vec(0, 1, 0, 0);
	tan = norm_vec(cross_prod(tool, norm));
		
	/* if (fabs(norm.x) > fabs(norm.z))
		tan = norm_vec(vec(-norm.y, norm.x, 0, 0));
	else
		tan = norm_vec(vec(0, -norm.z, norm.y, 0)); */
	bitan = cross_prod(norm, tan);
	v = add_vec(scale_vec(dir.x, tan), scale_vec(dir.y, bitan));
	return (add_vec(v, scale_vec(dir.z, norm)));
}

t_vec3 get_hemisphere_dir(t_vec3 norm)
{
	double r1;
	double r2;
	double theta;
	double phi;
	t_vec3 dir;

	r1 = randf();
	r2 = randf();
	theta = acos(sqrt(1.0 - r1));
	phi = 2.0 * M_PI * r2;
	dir = vec(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta), 0);
	return (normal_align(dir, norm));
}





void	emit_point_c(t_trace *trace, t_photon_map *map, double total, t_light *light)
{
	t_vec3	dir;
	int		emitted;
	double	x;
	double	y;
	double	z;
	t_photon pho;

	emitted = 0;
	while (emitted < total)
	{
		x = randf2();
		y = randf2();
		z = randf2();
		if ((x * x + y * y + z * z) <= 1)
		{
			dir = vec(x, y, z, 0);
			pho = photon(light->center, dir, mult_color(255.0 * light->brightness , light->color));
			trace_caustic_photon(trace, pho, map);
			emitted++;
		}
	}
	scale_photon_power(map->photons, 1.0 / total, map->count);
}
	
t_vec3	get_hemidir(t_vec3 lt_dir)
{
	double x;
	double y;
	double z;
	t_vec3 dir;

	x = randf2();
	y = randf2();
	z = randf2();
	while ((x * x + y * y + z * z) > 1)
	{
		x = randf2();
		y = randf2();
		z = randf2();
	}
	dir = norm_vec(vec(x, y, z, 0));
	if (dot_product(dir, lt_dir) < 0)
		dir = neg(dir);

	return (dir); 
}

t_vec3	get_ltdir(t_vec3 norm)
{
	double u;
	double v;
	t_vec3 dir;

	u = randf();
	v = 2 * M_PI * randf();
	dir = vec(cos(v) * sqrt(u), sin(v) * sqrt(u), sqrt(1 - u), 0);
	
	if (dot_product(dir, norm) < 0)
		dir = neg(dir);
	return (dir);
}


static inline t_vec3	pnt_on_arealight(t_light light, double u, double v)
{
	t_vec3		move_u;
	t_vec3		move_v;
	t_point		pos;

	move_u = scale_vec(u + randf(), light.uvec);
	move_v = scale_vec(v + randf(), light.vvec);
	pos = add_vec(light.corner, add_vec(move_u, move_v));
	return (pos);
}

//emit from an area light

void emit_area_c(t_trace * trace, t_photon_map *map, t_light light, double total)
{
	t_vec3		dir;
	t_point		pos;
	t_position	p;
	int			k;
	int			cell_photons;
	t_photon	pho;

	cell_photons = (int)(total / light.samples);
	p.j = -1;
	while (++p.j < light.vsteps)
	{
		p.i = -1;
		while(++p.i < light.usteps)
		{
			k = -1;
			while (++k < cell_photons)
			{
				pos = pnt_on_arealight(light, p.i, p.j);
				//pos = add_vec(scale_vec(light.emitter->h_height + 1e-6, light.dir), pos);//get below lightcube
				pos = add_vec(pos, scale_vec(light.emitter->h_height + 1e-4, light.dir));//get below lightcube
				
				//dir = norm_vec(get_hemisphere_dir(light.dir));
				//dir = get_hemidir(light.dir);//try something different here...
				dir = get_ltdir(light.dir);
				pho = photon(pos, dir, mult_color(light.brightness * 255.0, light.color));
				trace_caustic_photon(trace, pho, map);
			}
		}
	}
	scale_photon_power(map->photons, 1.0 / total, map->count);
}


int	trace_caustic_photon(t_trace *trace, t_photon photon, t_photon_map *caustic_map)
{
	int 			bounces;
	int				max;
	t_pcomps		comps;
	t_ray 			pho;
	t_intersects	*intersects;

	max = 4;
	intersects = create_ints(trace->total_ints);
	if (!intersects)
		return (1);
	pho = pho_ray(photon);
	//pho.dir = vec(0, -1, 0, 0);//straight down with low rad is giving caustic
	bounces = -1;
	while (++bounces < max)
	{
		find_closest(trace, pho, intersects);
		if (intersects->closest->object == NULL)
			break ; //photon left scene
		comps = set_comps(intersects->closest, pho);
		if (!comps.interact)
			break ;//photon absorbed.
		if (comps.interact == 1)//options should be 3 only. reflect. transmit. absorb.
		{
			if (bounces > 0)// && (comps.interact == 1 || comps.interact == 2))
				add_photon(caustic_map, photon);
			pho.dir = reflect_pho(pho.dir, comps.norm);
			comps.over_start = add_vec(comps.pnt, scale_vec(1e-6, comps.norm));//overp
		}
		else if (comps.interact == 2)//transmit
		{			
			set_indicies(intersects, &comps.n1, &comps.n2);
			if (!refract_pho(comps.n1 / comps.n2, neg(pho.dir), comps.norm, &pho.dir))
			{
				pho.dir = reflect_pho(pho.dir, comps.norm);
				comps.over_start = add_vec(comps.pnt, scale_vec(1e-6, comps.norm));//overp
			}
			else
			{
				if (bounces == 2 && intersects->closest->object_type == SPHERE)
					{
						/* printf("%f\n", comps.n1);
						printf("%f\n", comps.n2); */
					}
				comps.over_start = subtract_vec(comps.pnt, scale_vec(1e-6, comps.norm));//underp
			}
		}
		pho.origin = comps.over_start;
		photon.pos.x = (float)comps.pnt.x;
		photon.pos.y = (float)comps.pnt.y;
		photon.pos.z = (float)comps.pnt.z;
	//free_closests(trace, piece, trace->num_rows - 1, trace->num_cols);
	}
	free(intersects->closest);
	free(intersects->hits);
	free (intersects);
	return (0);
}

t_photon_map	*build_caustic_map(t_trace *trace, int photons)
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
			emit_point_c(trace, photon_map, curr_lt->photons, curr_lt);
		else if (curr_lt->type == AREA)
			emit_area_c(trace, photon_map, *curr_lt, curr_lt->photons);
		/* if (curr_lt->type == SPOT)
			emit_spot(trace, photon_map, curr_lt->photons, curr_lt); */
		curr_lt = curr_lt->next;
		if (curr_lt == trace->lights)
			break ;
	}
	return (photon_map);
}
