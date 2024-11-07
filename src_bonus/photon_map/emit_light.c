#include "minirt.h"

//power of photon = power of light / number of photons.
//raise quality by raising total emitted, 

//always try to emit evenly with strat.
//projection map to keep from wasting photons that dont hit anything.. try after working version.
//emit form a spot light, like point but within angle and angle with attenuation(soft spot)


bool	refract_pho(double n_ratio, t_vec3 neg_dir, t_vec3 normal, t_vec3 *refr_dir)
{
	double sin2_t;
	double cos_i;
	double cos_t;

	cos_i = dot_product(neg_dir, normal);
	sin2_t = n_ratio * n_ratio * ( 1 - cos_i * cos_i);
	if (sin2_t > 1.0)
		return (false);
	cos_t = sqrt(1.0 - sin2_t);
	*refr_dir = scale_vec(n_ratio * cos_i - cos_t, normal); 
	*refr_dir = norm_vec(subtract_vec(*refr_dir, scale_vec(n_ratio, neg_dir)));
	return (true);
}

t_vec3	reflect_pho(t_vec3 in, t_vec3 normal)
{
	double cos_a;

	cos_a = dot_product(in, normal);
	return (subtract_vec(in, scale_vec(2 * cos_a, normal)));
}



t_photon scale_power(double scale, t_photon photon)
{
	photon.r *= scale;
	photon.g *= scale;
	photon.b *= scale;
	return (photon);
}

void scale_photon_power(t_photon *photons, double scale, int count)
{
	int i;

	i = -1;
	while (++i < count)	
	{
		photons[i].r *= scale;
		photons[i].g *= scale;
		photons[i].b *= scale;
	}
}

//trace the photons
//do multithread here? using sep intersect lists?no earlier, must divi up photon casting...
int	trace_photon(t_trace *trace, t_photon photon, t_photon_map *map)
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
	bounces = -1;
	while (++bounces < max)
	{
		find_closest(trace, pho, intersects);
		if (intersects->closest->object == NULL)
			break ; //photon left scene
		comps = set_comps(intersects->closest, pho);
		if (!comps.interact)
			break ;//photon absorbed.
		if (comps.interact == 1)
		{
			add_photon(map, photon);
			//photon = scale_power(comps.mat.diffref, photon);

		}
		else if (comps.interact == 2)
		{			
			//reflect
			pho.dir = reflect_pho(pho.dir, comps.norm);
			photon = scale_power(comps.mat.spec, photon);
		}
		else if (comps.interact == 3)
		{
			//refract
			set_indicies(intersects, &comps.n1, &comps.n2);
			if (!refract_pho(comps.n1 / comps.n2, neg(pho.dir), comps.norm, &pho.dir))
				pho.dir = reflect_pho(pho.dir, comps.norm);
			photon = scale_power(comps.mat.transp, photon);
		}
		pho.origin = add_vec(comps.pnt, comps.over_start);
		photon.pos.x = (float)pho.origin.x;
		photon.pos.y = (float)pho.origin.y;
		photon.pos.z = (float)pho.origin.z;

	}
	//free_closests(trace, piece, trace->num_rows - 1, trace->num_cols);
	free(intersects->closest);
	free(intersects->hits);
	free (intersects);
	return (0);
}

//emit from a point light, total is the photons allocated for that src.

void	emit_point(t_trace *trace, t_photon_map *map, double total, t_light *light)
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
			trace_photon(trace, pho, map);
			emitted++;
		}
	}
	scale_photon_power(map->photons, 1.0 / total, map->count);
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

void emit_area(t_trace * trace, t_photon_map *map, t_light light, double total)
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
				dir = norm_vec(vec(randf(), randf(), randf(), 0));
				pho = photon(pos, dir, mult_color(light.brightness * 255.0, light.color));
				trace_photon(trace, pho, map);
			}
		}
	}
	scale_photon_power(map->photons, 1.0 / total, map->count);
}
