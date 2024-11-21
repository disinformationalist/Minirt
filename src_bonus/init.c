#include "minirt.h"
/*-------------VIEWPORT DIAGRAM-----------Best I could manage in ascii chars :(
			 |\
 			 | \				Using LEFT Handed Coordinates(for everything)
 			 |  \ 
 			 |   \								Y+ (up screen)
 			 |	  \==> viewport width			|
			/|\    \							|	
 		   / | \	\	+-----------------+		| 
 		  /	 |	\	 \	| SCENE GOES HERE |		\--------Z+ (into screen)
		 /	 |	 \	  \	+-----------------+	     \
(cam){o}----------\	  |		   				 	  \
  		  |   \	   \  |							   \
   		  |    \	\ | 							X+ (rightward along screen)
    	  | 	\	 \|
      focal_len	 \	  |==> viewport height
 (dash from cam)  \	  |
	   			   \  |
					\ |
					 \|

 view_width = 2 * focal_len * tan(horizontal_FOV / 2)// moved focal len adj.
 ----------------------------------------------------------------------------*/
//use the fov horizontal and aspect ratio to set view width, height;

void info_init(t_trace *trace)
{
	trace->width = 1080;
	trace->height = (int)((double)trace->width / ASPECT);
	trace->color_i = 0;
	trace->num_colors = 384;
	trace->curr_sp = trace->spheres;
	trace->curr_pl = trace->planes;
	trace->curr_cy = trace->cylinders;
	trace->curr_hy = trace->hyperboloids;
	trace->curr_lt = trace->lights;
	trace->curr_cu = trace->cubes;
	trace->w_colors = NULL;
	trace->m_colors = NULL;
	trace->supersample = false;
	trace->layer = false;
	trace->n = 5.0;
	trace->depths.refl = 6;
	trace->depths.refr = 6;

/* 	trace->global_map = NULL;
	trace->gl_tree = NULL;
	trace->caustic_map = NULL;
	trace->c_tree = NULL;
	
	trace->photnum = 100000;
	trace->rad = .8;
	trace->rad2 = trace->rad * trace->rad;
	trace->area = M_PI * trace->rad2; */

	//trace->spheres->mat = get_mat(GLASS);
	//trace->spheres->next->mat = get_mat(CHROME);

	init_viewing(trace);
}

static void events_init(t_trace *trace)
{
	trace->on->object = trace->spheres;
	trace->on->type = SPHERE;
	
	mlx_hook(trace->mlx_win, KeyPress, KeyPressMask, key_press, trace);
	mlx_hook(trace->mlx_win, DestroyNotify, StructureNotifyMask, close_win, trace);
	mlx_hook(trace->mlx_win, ButtonPress, \
	ButtonPressMask, mouse_handler, trace);
}

void init_transforms(t_trace *trace)
{
	set_sp_transforms(trace);
	set_pl_transforms(trace);
	set_cy_transforms(trace);
	set_hy_transforms(trace);
	set_cu_transforms(trace);
}

void trace_init(t_trace *trace)
{
	info_init(trace);
	trace->mlx_connect = mlx_init();
	if (trace->mlx_connect == NULL)
		clear_few(trace);
	trace->mlx_win = mlx_new_window(trace->mlx_connect, trace->width, trace->height, "***MegaRT***");
	if (trace->mlx_win == NULL)
		clear_some(trace);
	if (new_img_init(trace->mlx_connect, &trace->img, trace->width, trace->height) == -1)
		clear_all(trace);
	trace->on = (t_on *)malloc(sizeof(t_on));
	if (!trace->on)
		clear_all(trace);
	trace->w_colors = set_color_wheel(trace->num_colors, 1.0, 0.5, 202);
	if (!trace->w_colors)
		clear_all(trace);
	trace->m_colors = set_metal_colors();
	if (!trace->m_colors)
		clear_all(trace);
	events_init(trace);
	init_transforms(trace);
	if (import_textures(trace->mlx_connect, trace->textures))
		clear_all(trace);

	//test
	//trace->mesh = parse_obj("simple_dia.obj");
	//trace->mesh = parse_obj("dodecahedron.obj");
	//trace->mesh = parse_obj("simple_diamond.obj");


	//testing...
	//set the map(s) will have to move into rerender except when only moving cam
	/* trace->global_map = build_map(trace, trace->photnum);
	if (!trace->global_map)
		clear_all(trace);
	trace->gl_tree = build_tree(trace->global_map->photons, 0, trace->photnum, 0);
	if (!trace->gl_tree)
	{
		free(trace->global_map->photons);
		free(trace->global_map);
		free_tree(trace->gl_tree);
	}
	if (!trace->gl_tree)
	{
		free_mapping(trace);
		clear_all(trace);
	} */


//PHOTON MAP TESTING..
	/* trace->caustic_map = build_caustic_map(trace, trace->photnum);
	if (!trace->caustic_map)
		clear_all(trace);
	trace->c_tree = build_tree(trace->caustic_map->photons, 0, trace->photnum, 0);
	if (!trace->c_tree)
	{
		free(trace->caustic_map->photons);
		free(trace->caustic_map);
		free_tree(trace->c_tree);
	}
	if (!trace->c_tree)
	{
		free_mapping(trace);
		clear_all(trace);
	}
	printf("maps done\n"); */


//group and subgroup test. WORKING no leaks.//lookout when pushing and poping objs at runtime though
	/* 	trace->group = group();//pro this and add_childs, clear all frees group as well automat
		if (add_child(trace->group, trace->spheres, SPHERE, trace->spheres->transform))
			clear_all(trace);
		add_child(trace->group, trace->spheres->next, SPHERE, trace->spheres->next->transform);
		add_child(trace->group, trace->spheres->next->next, SPHERE, trace->spheres->next->next->transform);

		t_group *group2 = group();//free_group cannot free this until it is added.. if failur befor then, explicit free.
		if (!group2)
			clear_all(trace);
		if (add_child(group2, trace->cylinders, CYLINDER, trace->cylinders->transform))
		{
			free_group(group2);//beautiful works.groups!
			clear_all(trace);
		}
		add_child(group2, trace->cylinders->next, CYLINDER, trace->cylinders->next->transform);
		add_child(group2, trace->cylinders->next->next, CYLINDER, trace->cylinders->next->next->transform);
		add_child(trace->group, group2, GROUP, group2->transform); */

} 


/* todo
- group transform;
- add a set group transforms, adapt the group transforms at run same as the others. group willneed rotran etc....
-normals on children of group


*/



/* check_group(group2, intersects, ray);
free_group(group2); */