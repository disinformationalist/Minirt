/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_all_objects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:01:14 by jhotchki          #+#    #+#             */
/*   Updated: 2024/12/16 14:01:16 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	print_spheres(t_sphere *spheres)
{
	t_sphere	*curr_sp;

	if (spheres == NULL)
		return ;
	curr_sp = spheres;
	printf("\n----------------------SPHERES-----------------------\n");
	while (true)
	{
		printf("\n----------------sphere id: %d ----------- \n", curr_sp->id);
		printf("centerx: %f\n", curr_sp->center.x);
		printf("centery: %f\n", curr_sp->center.y);
		printf("centerz: %f\n\n", curr_sp->center.z);
		printf("sphere diam: %f\n", curr_sp->radius * 2);
		printf("sphere color rgb: r: %f, g: %f, b %f\n", curr_sp->color.r,
			curr_sp->color.g, curr_sp->color.b);
		curr_sp = curr_sp->next;
		if (curr_sp == spheres)
			break ;
	}
}

void	print_amb(t_amb *amb)
{
	printf("\n----------------------AMB-----------------------\n");
	printf("intensity ratio:%f\n", amb->ratio);
	printf("ambient color rgb: r: %f, g: %f, b %f\n", amb->color.r,
		amb->color.g, amb->color.b);
}

void	print_cam(t_cam *cam)
{
	printf("\n----------------------CAM-----------------------\n");
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
	printf("\n----------------------LIGHTS-----------------------\n");
	printf("\n----------------light id: %d ----------- \n", curr_lt->id);
	printf("centerx: %f\n", curr_lt->center.x);
	printf("centery: %f\n", curr_lt->center.y);
	printf("centerz: %f\n\n", curr_lt->center.z);
	printf("brightness ratio:%f\n", curr_lt->brightness);
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
