#include "minirt.h"

void	print_cylinders(t_cylinder *cylinder)
{
	int start_id;

	start_id = cylinder->id;
	printf("----------------------cylinder id: %d ----------------- \n", cylinder->id);
	printf("centerx: %f\n", cylinder->center.x);
	printf("centery: %f\n", cylinder->center.y);
	printf("centerz: %f\n", cylinder->center.z);
	printf("norm_vecx: %f\n", cylinder->norm_vector.x);
	printf("norm_vecy: %f\n", cylinder->norm_vector.y);
	printf("norm_vecz: %f\n", cylinder->norm_vector.z);
	printf("cylinder diam: %f\n", cylinder->radius * 2);
	printf("cylinder height: %f\n", cylinder->height);
	printf("cylinder color rgb: r: %f, g: %f, b %f\n", cylinder->color.r, cylinder->color.g, cylinder->color.b);
	cylinder = cylinder->next;
	while (cylinder->id != start_id)
	{
		printf("----------------------cylinder id: %d ----------------- \n", cylinder->id);
		printf("centerx: %f\n", cylinder->center.x);
		printf("centery: %f\n", cylinder->center.y);
		printf("centerz: %f\n", cylinder->center.z);
		printf("norm_vecx: %f\n", cylinder->norm_vector.x);
		printf("norm_vecy: %f\n", cylinder->norm_vector.y);
		printf("norm_vecz: %f\n", cylinder->norm_vector.z);
		printf("cylinder diam: %f\n", cylinder->radius * 2);
		printf("cylinder height: %f\n", cylinder->height);
		printf("cylinder color rgb: r: %f, g: %f, b %f\n", cylinder->color.r, cylinder->color.g, cylinder->color.b);
		cylinder = cylinder->next;
	}
}

void	print_planes(t_plane *plane)
{
	int start_id;

	start_id = plane->id;
	printf("----------------------plane id: %d ----------------- \n", plane->id);
	printf("pointx: %f\n", plane->point.x);
	printf("pointy: %f\n", plane->point.y);
	printf("pointz: %f\n", plane->point.z);
	printf("norm_vecx: %f\n", plane->norm_vector.x);
	printf("norm_vecy: %f\n", plane->norm_vector.y);
	printf("norm_vecz: %f\n", plane->norm_vector.z);
	printf("plane color rgb: r: %f, g: %f, b %f\n", plane->color.r, plane->color.g, plane->color.b);
	plane = plane->next;
	while (plane->id != start_id)
	{
		printf("----------------------plane id: %d ----------------- \n", plane->id);
		printf("pointx: %f\n", plane->point.x);
		printf("pointy: %f\n", plane->point.y);
		printf("pointz: %f\n", plane->point.z);
		printf("norm_vecx: %f\n", plane->norm_vector.x);
		printf("norm_vecy: %f\n", plane->norm_vector.y);
		printf("norm_vecz: %f\n", plane->norm_vector.z);
		printf("plane color rgb: r: %f, g: %f, b %f\n", plane->color.r, plane->color.g, plane->color.b);
		plane = plane->next;
	}
}

void	print_spheres(t_sphere *sphere)
{
	int start_id;

	start_id = sphere->id;
	printf("----------------------sphere id: %d ----------------- \n", sphere->id);
	printf("centerx: %f\n", sphere->center.x);
	printf("centery: %f\n", sphere->center.y);
	printf("centerz: %f\n", sphere->center.z);
	printf("sphere diam: %f\n", sphere->radius * 2);
	printf("sphere color rgb: r: %f, g: %f, b %f\n", sphere->color.r, sphere->color.g, sphere->color.b);
	sphere = sphere->next;
	while (sphere->id != start_id)
	{
		printf("----------------------sphere id: %d ----------------- \n", sphere->id);
		printf("centerx: %f\n", sphere->center.x);
		printf("centery: %f\n", sphere->center.y);
		printf("centerz: %f\n", sphere->center.z);
			printf("sphere diam: %f\n", sphere->radius * 2);
	printf("sphere color rgb: r: %f, g: %f, b %f\n", sphere->color.r, sphere->color.g, sphere->color.b);
		sphere = sphere->next;
	}
}

void	print_amb(t_amb *amb)
{
	printf("----------------------------amb-----------------------------\n");
	printf("intensity ratio:%f\n", amb->ratio);
	printf("ambient color rgb: r: %f, g: %f, b %f\n", amb->color.r, amb->color.g, amb->color.b);
}

void	print_cam(t_cam *cam)
{
	printf("----------------------------cam-----------------------------\n");
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
	printf("----------------------------lights-----------------------------\n");
	printf("centerx: %f\n", lights->center.x);
	printf("centery: %f\n", lights->center.y);
	printf("centerz: %f\n\n", lights->center.z);
	printf("brightness ratio:%f\n", lights->brightness);
	if (lights->light_color.r)
		printf("light color rgb: r: %f, g: %f, b %f\n", lights->light_color.r, lights->light_color.g, lights->light_color.b);
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
}
