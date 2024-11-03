#ifndef PHOTON_H
# define PHOTON_H

#include "tools.h"

typedef struct s_vec3f
{
	float	x;
	float	y;
	float	z;
	//float	w;
}	t_vec3f;

typedef struct s_photon
{
	t_vec3f		pos;

	float	r;//power
	float	g;
	float	b;

	t_vec3f		dir;

	/* char phi;//compressed incident dir
	char theta;// */
	short	flag;//used in kdtree
} 	t_photon;

typedef struct s_photon_map
{
	t_photon	*photons;
	int			count;
	int			size;
}	t_photon_map;

typedef struct s_kdnode
{
	t_photon		photon;
	char			split_dim;//x, y, z. enum?
	struct s_kdnode	*left;
	struct s_kdnode	*right;
}	t_kdnode;

typedef struct s_pcomps
{
	t_vec3	norm;
	t_vec3	pnt;
	t_vec3	over_start;
	t_mat 	mat;
	int		interact;
	double	n1;
	double	n2;

}	t_pcomps;


/***PHOTON UTILS***/

t_vec3f			vecf(float x, float y, float z);//, float w);
double			randf2(void);
void			add_photon(t_photon_map *map, t_photon photon);
t_photon		photon(t_vec3 pos, t_vec3 dir, t_norm_color power);
t_ray			pho_ray(t_photon photon);
t_pcomps		set_comps(t_track_hits *closest, t_ray pho);
void			rad_search(t_kdnode *node, t_vec3 pnt, float rad2, t_photon_map *list);

t_intersects 	*create_ints(int total);
t_photon_map	*create_photon_map(int size);
void			free_tree(t_kdnode *tree);
t_kdnode 		*build_tree(t_photon *photons, int start, int end, int depth);

void 			scale_photon_power(t_photon *photons, double scale, int count);
t_photon 		scale_power(double scale, t_photon photon);
t_vec3			reflect_pho(t_vec3 in, t_vec3 normal);
bool			refract_pho(double n_ratio, t_vec3 neg_dir, t_vec3 normal, t_vec3 *refr_dir);



#endif