#include "minirt.h"

void	print_cylinders(t_cylinder *cylinder)
{
	t_cylinder	*curr_cy;

	if (cylinder == NULL)
		return ;
	curr_cy = cylinder;
	printf("\n-------------------------CYLINDERS--------------------------\n");
	while (true)
	{
		printf("\n---------------cylinder id: %d ---------- \n", curr_cy->id);
		printf("centerx: %f\n", curr_cy->center.x);
		printf("centery: %f\n", curr_cy->center.y);
		printf("centerz: %f\n\n", curr_cy->center.z);
		printf("norm_vecx: %f\n", curr_cy->norm.x);
		printf("norm_vecy: %f\n", curr_cy->norm.y);
		printf("norm_vecz: %f\n\n", curr_cy->norm.z);
		printf("cylinder diam: %f\n", curr_cy->radius * 2);
		printf("cylinder height: %f\n", curr_cy->height);
		printf("cylinder color rgb: r: %f, g: %f, b %f\n",
			curr_cy->color.r, curr_cy->color.g, curr_cy->color.b);
		curr_cy = curr_cy->next;
		if (curr_cy == cylinder)
			break ;
	}
}

void	print_hyperboloids(t_hyperboloid *hyperboloid)
{
	t_hyperboloid	*curr_hy;

	if (hyperboloid == NULL)
		return ;
	curr_hy = hyperboloid;
	printf("\n-----------------------HYPERBOLOIDS------------------------\n");
	while (true)
	{
		printf("\n-------------hyperboloid id: %d -------- \n", curr_hy->id);
		printf("centerx: %f\n", curr_hy->center.x);
		printf("centery: %f\n", curr_hy->center.y);
		printf("centerz: %f\n\n", curr_hy->center.z);
		printf("norm_vecx: %f\n", curr_hy->norm.x);
		printf("norm_vecy: %f\n", curr_hy->norm.y);
		printf("norm_vecz: %f\n\n", curr_hy->norm.z);
		printf("hyperboloid rad: %f\n", curr_hy->rad);
		printf("hyperboloid waist: %f\n", curr_hy->waist_val);
		printf("hyperboloid height: %f\n", curr_hy->height);
		printf("hyperboloid color rgb: r: %f, g: %f, b %f\n", curr_hy->color.r,
			curr_hy->color.g, curr_hy->color.b);
		curr_hy = curr_hy->next;
		if (curr_hy == hyperboloid)
			break ;
	}
}

void	print_planes(t_plane *plane)
{
	t_plane	*curr_pl;

	if (plane == NULL)
		return ;
	curr_pl = plane;
	printf("\n--------------------------PLANES---------------------------\n");
	while (true)
	{
		printf("\n----------------plane id: %d ----------- \n", curr_pl->id);
		printf("pointx: %f\n", curr_pl->point.x);
		printf("pointy: %f\n", curr_pl->point.y);
		printf("pointz: %f\n", curr_pl->point.z);
		printf("norm_vecx: %f\n", curr_pl->norm.x);
		printf("norm_vecy: %f\n", curr_pl->norm.y);
		printf("norm_vecz: %f\n", curr_pl->norm.z);
		printf("plane color rgb: r: %f, g: %f, b %f\n",
			curr_pl->color.r, curr_pl->color.g, curr_pl->color.b);
		curr_pl = curr_pl->next;
		if (curr_pl == plane)
			break ;
	}
}

void	print_spheres(t_sphere *spheres)
{
	t_sphere	*curr_sp;

	if (spheres == NULL)
		return ;
	curr_sp = spheres;
	printf("\n--------------------------SPHERES---------------------------\n");
	while (true)
	{
		printf("\n-----------------sphere id: %d ------------ \n", curr_sp->id);
		printf("centerx: %f\n", curr_sp->center.x);
		printf("centery: %f\n", curr_sp->center.y);
		printf("centerz: %f\n\n", curr_sp->center.z);
		printf("sphere diam: %f\n", curr_sp->radius * 2);
		printf("sphere color rgb: r: %f, g: %f, b %f\n",
			curr_sp->color.r, curr_sp->color.g, curr_sp->color.b);
		curr_sp = curr_sp->next;
		if (curr_sp == spheres)
			break ;
	}
}

void	print_cubes(t_cube *cube)
{
	t_cube	*curr_cu;

	if (cube == NULL)
		return ;
	curr_cu = cube;
	printf("\n--------------------------CUBES---------------------------\n");
	while (true)
	{
		printf("\n----------------cube id: %d ----------- \n", curr_cu->id);
		printf("pointx: %f\n", curr_cu->center.x);
		printf("pointy: %f\n", curr_cu->center.y);
		printf("pointz: %f\n", curr_cu->center.z);
		printf("norm_vecx: %f\n", curr_cu->norm.x);
		printf("norm_vecy: %f\n", curr_cu->norm.y);
		printf("norm_vecz: %f\n", curr_cu->norm.z);
		printf("cube color rgb: r: %f, g: %f, b %f\n",
			curr_cu->color.r, curr_cu->color.g, curr_cu->color.b);
		curr_cu = curr_cu->next;
		if (curr_cu == cube)
			break ;
	}
}
