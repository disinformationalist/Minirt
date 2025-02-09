#include "minirt.h"

/* typedef struct s_track_hits
{
	double	t;
	void	*object;
	t_type	object_type;
}	t_track_hits;

typedef struct s_intersects
{
	t_track_hits	*hits;
	int				count;
	int				size;
	t_track_hits	*closest;
}	t_intersects; */

void	print_intersects(t_intersects *ints)
{
	int i;
	t_track_hits curr;

	i = -1;
	while (++i < ints->count)
	{
		curr = ints->hits[i];
		printf("--------------Intersect--%d---------------\n", i); 
		printf("The object type: %d\n", curr.object_type);
		printf("t_value: %f\n", curr.t);
		printf("\n");
		printf("--------------------------------------------\n\n"); 
	}
}

//cast a single ray for testing purposed

void	cast_single_ray(t_trace *trace, t_point origin, t_vec3 dir)
{
	t_ray	r;
	t_intersects *intersects;
	dir = norm_vec(dir);
	r = ray(dir, origin);
	intersects = create_ints(trace->total_ints);
	if (!intersects)
		clear_all(trace);
	find_closest(trace, r, intersects);
	print_intersects(intersects);
	free(intersects->hits);
	free(intersects->closest);
	free(intersects);
	clear_all(trace);
}