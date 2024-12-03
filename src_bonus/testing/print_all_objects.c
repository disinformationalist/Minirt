#include "minirt.h"

void	print_amb(t_amb *amb)
{
	printf("\n----------------------------AMB-----------------------------\n");
	printf("intensity ratio:%f\n", amb->ratio);
	printf("ambient color rgb: r: %f, g: %f, b %f\n",
		amb->color.r, amb->color.g, amb->color.b);
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

void	print_type(t_light *curr_lt)
{
	if (curr_lt->type == SPOT)
	{
		printf("dir_x: %f\n", curr_lt->dir.x);
		printf("dir_y: %f\n", curr_lt->dir.y);
		printf("dir_z: %f\n\n", curr_lt->dir.z);
		printf("type: SPOT\n");
	}
	if (curr_lt->type == POINT)
		printf("type: POINT\n");
}

void	print_lights(t_light *lights)
{
	t_light	*curr_lt;

	if (lights == NULL)
		return ;
	curr_lt = lights;
	printf("\n---------------------------LIGHTS----------------------------\n");
	while (true)
	{
		printf("\n-----------------light id: %d ------------ \n", curr_lt->id);
		printf("centerx: %f\n", curr_lt->center.x);
		printf("centery: %f\n", curr_lt->center.y);
		printf("centerz: %f\n\n", curr_lt->center.z);
		printf("brightness ratio:%f\n", curr_lt->brightness);
		if (curr_lt->color.r || curr_lt->color.g || curr_lt->color.b)
			printf("light color rgb: r: %f, g: %f, b %f\n",
				curr_lt->color.r, curr_lt->color.g, curr_lt->color.b);
		print_type(curr_lt);
		curr_lt = curr_lt->next;
		if (curr_lt == lights)
			break ;
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
	if (trace->hyperboloids)
		print_hyperboloids(trace->hyperboloids);
	if (trace->cubes)
		print_cubes(trace->cubes);
	printf("\n\n");
}
