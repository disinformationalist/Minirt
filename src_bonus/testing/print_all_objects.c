#include "minirt.h"

void	print_cylinders(t_cylinder *cylinder)
{
	t_cylinder	*curr_cy;

	if (cylinder == NULL)
		return ;
	curr_cy = cylinder;
	printf("\n----------------------------CYLINDERS-----------------------------\n");
	while (true)
	{
		printf("\n----------------------cylinder id: %d ----------------- \n", curr_cy->id);
		printf("centerx: %f\n", curr_cy->center.x);
		printf("centery: %f\n", curr_cy->center.y);
		printf("centerz: %f\n\n", curr_cy->center.z);
		printf("norm_vecx: %f\n", curr_cy->norm.x);
		printf("norm_vecy: %f\n", curr_cy->norm.y);
		printf("norm_vecz: %f\n\n", curr_cy->norm.z);
		printf("cylinder diam: %f\n", curr_cy->radius * 2);
		printf("cylinder height: %f\n", curr_cy->height);
		printf("cylinder color rgb: r: %f, g: %f, b %f\n", curr_cy->color.r, curr_cy->color.g, curr_cy->color.b);
		curr_cy = curr_cy->next;
		if (curr_cy == cylinder)
			break;
	}
}

void	print_planes(t_plane *plane)
{
	t_plane	*curr_pl;

	if (plane == NULL)
		return ;
	curr_pl = plane;
	printf("\n----------------------------PLANES-----------------------------\n");
	while (true)
	{
		printf("\n----------------------plane id: %d ----------------- \n", curr_pl->id);
		printf("pointx: %f\n", curr_pl->point.x);
		printf("pointy: %f\n", curr_pl->point.y);
		printf("pointz: %f\n", curr_pl->point.z);
		printf("norm_vecx: %f\n", curr_pl->norm.x);
		printf("norm_vecy: %f\n", curr_pl->norm.y);
		printf("norm_vecz: %f\n", curr_pl->norm.z);
		printf("plane color rgb: r: %f, g: %f, b %f\n", curr_pl->color.r, curr_pl->color.g, curr_pl->color.b);
		curr_pl = curr_pl->next;
		if (curr_pl == plane)
			break;
	}
}

void	print_spheres(t_sphere *spheres)
{
	t_sphere	*curr_sp;

	if (spheres == NULL)
		return ;
	curr_sp = spheres;
	printf("\n----------------------------SPHERES-----------------------------\n");
	while (true)
	{
		printf("\n----------------------sphere id: %d ----------------- \n", curr_sp->id);
		printf("centerx: %f\n", curr_sp->center.x);
		printf("centery: %f\n", curr_sp->center.y);
		printf("centerz: %f\n\n", curr_sp->center.z);
		printf("sphere diam: %f\n", curr_sp->radius * 2);
		printf("sphere color rgb: r: %f, g: %f, b %f\n", curr_sp->color.r, curr_sp->color.g, curr_sp->color.b);
		curr_sp = curr_sp->next;
		if (curr_sp == spheres)
			break;
	}
}

void	print_amb(t_amb *amb)
{
	printf("\n----------------------------AMB-----------------------------\n");
	printf("intensity ratio:%f\n", amb->ratio);
	printf("ambient color rgb: r: %f, g: %f, b %f\n", amb->color.r, amb->color.g, amb->color.b);
}

void	print_cam(t_cam *cam)
{
	printf("\n----------------------------CAM-----------------------------\n");
	printf("centerx: %f\n", cam->center.x);
	printf("centery: %f\n", cam->center.y);
	printf("centerz: %f\n\n", cam->center.z);
	printf("centerx: %f\n", cam->orient.x);
	printf("centery: %f\n", cam->orient.y);
	printf("centerz: %f\n\n", cam->orient.z);
	printf("fov:%d\n", cam->fov);
}

void	print_lights(t_light *lights)
{
	t_light	*curr_lt;

	if (lights == NULL)
		return ;
	curr_lt = lights;
	printf("\n----------------------------LIGHTS-----------------------------\n");
	while (true)
	{
		printf("\n----------------------light id: %d ----------------- \n", curr_lt->id);
		printf("centerx: %f\n", curr_lt->center.x);
		printf("centery: %f\n", curr_lt->center.y);
		printf("centerz: %f\n\n", curr_lt->center.z);
		printf("brightness ratio:%f\n", curr_lt->brightness);
		if (curr_lt->color.r || curr_lt->color.g || curr_lt->color.b)
			printf("light color rgb: r: %f, g: %f, b %f\n", curr_lt->color.r, curr_lt->color.g, curr_lt->color.b);
		if (curr_lt->type == SPOT)
		{
			printf("dir_x: %f\n", curr_lt->dir.x);
			printf("dir_y: %f\n", curr_lt->dir.y);
			printf("dir_z: %f\n\n", curr_lt->dir.z);
			printf("type: SPOT\n");
		}
		if (curr_lt->type == POINT)
			printf("type: POINT\n");
		curr_lt = curr_lt->next;
		if (curr_lt == lights)
			break;
	}
}

void	print_all_objects(t_trace *trace)
{
	if (trace->amb)
		print_amb(trace->amb);
	if (trace->cam)
		print_cam(trace->cam);
	if (trace->lights)
		print_lights(trace->lights);
	if (trace->spheres)
		print_spheres(trace->spheres);
	if (trace->planes)
		print_planes(trace->planes);
	if (trace->cylinders)
		print_cylinders(trace->cylinders);
	printf("\n\n");
}
