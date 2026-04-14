#include "minirt.h"

static inline void	free_all(t_trace *trace, \
	t_piece piece[][trace->num_cols], int i, int j)
{
	free_closests(trace, piece, i, j);
	clear_all(trace);
}

//trying out adaptive double pass for super

void	render_scene_s(t_trace *trace)
{
	t_piece	piece[trace->num_rows][trace->num_cols];
	int		i;
	int		j;
	//for adaptive, first pass
	trace->centers = malloc(sizeof(t_norm_color) * trace->width * trace->height);//must maintain outside of threads
	if (!trace->centers)
		clear_all(trace);

	if (trace->menu_open)
		set_pieces_menu(trace, piece);

	i = -1;
	while (++i < trace->num_rows)
	{
		j = -1;
		while (++j < trace->num_cols)
		{
			if (!trace->menu_open)
				if (set_pieces(trace, piece, i, j))
					free_all(trace, piece, i, j);
			if (pthread_create(&trace->threads[i * trace->num_cols + j], \
				NULL, ray_trace_centers, (void *)&piece[i][j]) != 0)
			{
				free_closests(trace, piece, i, j);
				free(trace->centers);
				thread_error(trace, i * trace->num_cols + j);
				return ;
			}
		}
	}
	join_threads(trace);

	//use trace->centers for a second pass, peices do not need reset.
	i = -1;
	while (++i < trace->num_rows)
	{
		j = -1;
		while (++j < trace->num_cols)
		{
			if (pthread_create(&trace->threads[i * trace->num_cols + j], \
				NULL, ray_trace_s, (void *)&piece[i][j]) != 0)
			{
				free_closests(trace, piece, i, j);
				free(trace->centers);
				thread_error(trace, i * trace->num_cols + j);
				return ;
			}
		}
	}
	join_threads(trace);
	free_closests(trace, piece, trace->num_rows - 1, trace->num_cols);
	free(trace->centers);
}


//combine this options pass selection parameter or pass function pointer

/* void	render_scene_s(t_trace *trace)
{
	t_piece	piece[trace->num_rows][trace->num_cols];
	int		i;
	int		j;


	if (trace->menu_open)
		set_pieces_menu(trace, piece);

	i = -1;
	while (++i < trace->num_rows)
	{
		j = -1;
		while (++j < trace->num_cols)
		{
			if (!trace->menu_open)
				if (set_pieces(trace, piece, i, j))
					free_all(trace, piece, i, j);
			if (pthread_create(&trace->threads[i * trace->num_cols + j], \
				NULL, ray_trace_s, (void *)&piece[i][j]) != 0)
			{
				free_closests(trace, piece, i, j);
				thread_error(trace, i * trace->num_cols + j);
				return ;
			}
		}
	}
	join_threads(trace);

	free_closests(trace, piece, trace->num_rows - 1, trace->num_cols);
	free(trace->centers);
} */

//bvh testing stuff
void	sum_bvh_stats(t_bvh_stats *dst, t_bvh_stats *src)
{
	dst->box_tests += src->box_tests;
	dst->box_hits += src->box_hits;
	dst->group_visits += src->group_visits;
	dst->prim_tests += src->prim_tests;
	dst->shadow_box_tests += src->shadow_box_tests;
	dst->shadow_box_hits += src->shadow_box_hits;
	dst->shadow_group_visits += src->shadow_group_visits;
	dst->shadow_prim_tests += src->shadow_prim_tests;
	dst->camera_rays += src->camera_rays;
	dst->shadow_rays += src->shadow_rays;
	if (src->max_group_depth > dst->max_group_depth)
		dst->max_group_depth = src->max_group_depth;
	if (src->shadow_max_group_depth > dst->shadow_max_group_depth)
		dst->shadow_max_group_depth = src->shadow_max_group_depth;
}

void	sum_piece_stats(t_trace *trace, t_piece piece[trace->num_rows][trace->num_cols])
{
	int	i;
	int	j;

	i = -1;
	while (++i < trace->num_rows)
	{
		j = -1;
		while (++j < trace->num_cols)
			sum_bvh_stats(&trace->stats, &piece[i][j].intersects->stats);
	}
}

void	print_bvh_stats(t_bvh_stats *s)
{
	double avg_prim_cam = 0.0;
	double avg_group_cam = 0.0;
	double avg_box_cam = 0.0;

	double avg_prim_sh = 0.0;
	double avg_group_sh = 0.0;
	double avg_box_sh = 0.0;

	double box_hit_ratio = 0.0;
	double shadow_box_hit_ratio = 0.0;

	if (s->camera_rays > 0)
	{
		avg_prim_cam = (double)s->prim_tests / s->camera_rays;
		avg_group_cam = (double)s->group_visits / s->camera_rays;
		avg_box_cam = (double)s->box_tests / s->camera_rays;
	}

	if (s->shadow_rays > 0)
	{
		avg_prim_sh = (double)s->shadow_prim_tests / s->shadow_rays;
		avg_group_sh = (double)s->shadow_group_visits / s->shadow_rays;
		avg_box_sh = (double)s->shadow_box_tests / s->shadow_rays;
	}

	if (s->box_tests > 0)
		box_hit_ratio = (double)s->box_hits / s->box_tests;

	if (s->shadow_box_tests > 0)
		shadow_box_hit_ratio = (double)s->shadow_box_hits / s->shadow_box_tests;

	printf("\n=== BVH STATS ===\n\n");

	printf("Camera rays: %lld\n", s->camera_rays);
	printf("Shadow rays: %lld\n\n", s->shadow_rays);

	printf("---- Camera Traversal ----\n");
	printf("Avg prim tests / ray:    %.3f\n", avg_prim_cam);
	printf("Avg group visits / ray:  %.3f\n", avg_group_cam);
	printf("Avg box tests / ray:     %.3f\n", avg_box_cam);

	printf("\n---- Shadow Traversal ----\n");
	printf("Avg prim tests / ray:    %.3f\n", avg_prim_sh);
	printf("Avg group visits / ray:  %.3f\n", avg_group_sh);
	printf("Avg box tests / ray:     %.3f\n", avg_box_sh);

	printf("\n---- BVH Quality ----\n");
	printf("Box hit ratio:           %.3f\n", box_hit_ratio);
	printf("Shadow box hit ratio:    %.3f\n", shadow_box_hit_ratio);

	printf("\n---- Depth ----\n");
	printf("Max group depth:         %lld\n", s->max_group_depth);
	printf("Max shadow depth:        %lld\n", s->shadow_max_group_depth);

	printf("\n=====================\n");
}

/* render_scene starts by breaking up the screen into groups of pixels and 
assigns the limits of each part to a t_peice, each to be worked by a thread */
//t_piece	piece[trace->num_rows][trace->num_cols];


void	render_scene(t_trace *trace)
{
	t_piece	piece[trace->num_rows][trace->num_cols];
	int		i;
	int		j;

	if (trace->bvh_testing)
		init_bvh_stats(&trace->stats);//bvh tracking

	if (trace->menu_open)
		set_pieces_menu(trace, piece);
		//	print_threads(piece[i][j]);
	i = -1;
	while (++i < trace->num_rows)
	{
		j = -1;
		while (++j < trace->num_cols)
		{
			if (!trace->menu_open)
				if (set_pieces(trace, piece, i, j))
					free_all(trace, piece, i, j);
			if (pthread_create(&trace->threads[i * trace->num_cols + j], \
				NULL, ray_trace, (void *)&piece[i][j]) != 0)
			{
				free_closests(trace, piece, i, j);
				thread_error(trace, i * trace->num_cols + j);
				return ;
			}
		}
	}
	join_threads(trace);
	if (trace->bvh_testing)
	{
		sum_piece_stats(trace, piece);//bvh tracking
		print_bvh_stats(&trace->stats);
	}
	free_closests(trace, piece, trace->num_rows - 1, trace->num_cols);
}

void	render_low_res(t_trace *trace)
{
	t_piece	piece[trace->num_rows][trace->num_cols];
	int		i;
	int		j;

	//set the threads around the menu
	if (trace->menu_open)
		set_pieces_menu(trace, piece);
	//set_thread_colors(trace, piece);//testing
	i = -1;
	while (++i < trace->num_rows)
	{
		j = -1;
		while (++j < trace->num_cols)
		{
			if (!trace->menu_open)
				if (set_pieces(trace, piece, i, j))
					free_all(trace, piece, i, j);
			if (pthread_create(&trace->threads[i * trace->num_cols + j], \
				NULL, ray_trace_l, (void *)&piece[i][j]) != 0)
			{
				free_closests(trace, piece, i, j);
				thread_error(trace, i * trace->num_cols + j);
				return ;
			}
		}
	}
	join_threads(trace);
	free_closests(trace, piece, trace->num_rows - 1, trace->num_cols);
}

void	render(t_trace *trace)
{
	long	start;
	long	end;

	//cast_single_ray(trace, vec(0, 2, 0, 1), vec(0, 0, 1, 0));//for testing
	
	start = get_time();
	if (start == -1)
		clear_all(trace);
	if (trace->low_res)
		render_low_res(trace);
	else if (trace->supersample)
	{
		ft_putstr_color_fd(1, "SUPERSAMPLE IN PROGRESS...\n", BOLD_GREEN);
		render_scene_s(trace);
		ft_putstr_color_fd(1, "SUPERSAMPLE COMPLETE\n", BOLD_BRIGHT_GREEN);
	}
	else
		render_scene(trace); 
	mlx_put_image_to_window(trace->mlx_connect,
		trace->mlx_win, trace->img.img_ptr, 0, 0);
	
	end = get_time();
	if (end == -1)
		clear_all(trace);
	if (!trace->low_res)
		print_times(start, end, "RENDER TIME: %f seconds\n");
}
