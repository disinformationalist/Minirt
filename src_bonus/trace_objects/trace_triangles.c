#include "minirt.h"

void	ray_tri_intersect(t_tri *tri, t_ray ray, t_intersects *intersects)
{
	t_vec3		dir_cross_edge2;
	t_vec3		p1_to_ori;
	t_vec3		ori_cross_edge1;
	t_moller	vals;
	double		t;

	//ray = transform(ray, tri->transform);
	dir_cross_edge2 = cross_prod(ray.dir, tri->edge2);
	vals.det = dot_product(tri->edge1, dir_cross_edge2);
	if (fabs(vals.det) < 1e-6)
		return ;
	p1_to_ori = subtract_vec(ray.origin, tri->p1);
	vals.f = 1.0 / vals.det;
	vals.u = vals.f * dot_product(p1_to_ori, dir_cross_edge2);
	if (vals.u < 0 || vals.u > 1)
		return ;
	ori_cross_edge1 = cross_prod(p1_to_ori, tri->edge1);
	vals.v = vals.f * dot_product(ray.dir, ori_cross_edge1);
	if (vals.v < 0 || (vals.u + vals.v) > 1)
		return ;
	t = vals.f * dot_product(tri->edge2, ori_cross_edge1);
	intersect(intersects, tri, t, TRI);
}

void	check_triangles(t_tri *tris, t_intersects *intersects, t_ray ray)
{
	t_tri	*curr_tri;

	if (tris == NULL)
		return ;
	curr_tri = tris;
	while (true)
	{
		ray_tri_intersect(curr_tri, ray, intersects);
		curr_tri = curr_tri->next;
		if (curr_tri == tris)
			break;
	}
}

void	ray_mesh_tri(t_triangle *tri, t_ray ray, t_intersects *intersects, t_point v0)
{
	t_vec3		dir_cross_edge2;
	t_vec3		p1_to_ori;
	t_vec3		ori_cross_edge1;
	t_moller	vals;
	double		t;

	//ray = transform(ray, tri->transform);
	dir_cross_edge2 = cross_prod(ray.dir, tri->edge2);
	vals.det = dot_product(tri->edge1, dir_cross_edge2);
	if (fabs(vals.det) < 1e-6)
		return ;
	p1_to_ori = subtract_vec(ray.origin, v0);
	vals.f = 1.0 / vals.det;
	vals.u = vals.f * dot_product(p1_to_ori, dir_cross_edge2);
	if (vals.u < 0 || vals.u > 1)
		return ;
	ori_cross_edge1 = cross_prod(p1_to_ori, tri->edge1);
	vals.v = vals.f * dot_product(ray.dir, ori_cross_edge1);
	if (vals.v < 0 || (vals.u + vals.v) > 1)
		return ;
	t = vals.f * dot_product(tri->edge2, ori_cross_edge1);
	intersect(intersects, tri, t, TRI);
}
//maybe make a doublelinked list of meshes later for use of multiple obj files


void	check_mesh(t_mesh *mesh, t_intersects *intersects, t_ray ray)
{
	//t_tri	*curr_tri;
	int		i;

	if (mesh == NULL)
		return ;
	/* curr_tri = tris;
	while (true)
	{
		ray_tri_intersect(curr_tri, ray, intersects);
		curr_tri = curr_tri->next;
		if (curr_tri == tris)
			break; 
	} */
	i = -1;
	while (++i < mesh->num_tris)
	{
		ray_mesh_tri(&mesh->triangles[i], ray, intersects, mesh->verts[mesh->triangles[i].v0]);
	}
}