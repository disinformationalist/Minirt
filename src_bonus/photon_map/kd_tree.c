#include "minirt.h"

void	swap_pho(t_photon *a, t_photon *b)
{
	t_photon tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int partition(t_photon *photons, t_position se, int axis)
{
	float	pivot;
	int 	i;
	int		j;

	if (axis == 0)
		pivot = photons[se.j].pos.x;
	else if (axis == 1)
		pivot = photons[se.j].pos.y;
	else
		pivot = photons[se.j].pos.z;
	i = se.i;
	j = i - 1;
	while (++j < se.j)
	{
		if ((axis == 0 && photons[j].pos.x <= pivot) ||
			(axis == 1 && photons[j].pos.y <= pivot) ||
			(axis == 2 && photons[j].pos.z <= pivot))
		{
		swap_pho(&photons[i], &photons[j]);
		i++;
		}
	}
	swap_pho(&photons[i], &photons[se.j]);
	return (i);
}

void	quicksearch(t_photon *photons, t_position se, int k, int axis)
{
	int pivot;

	if (se.i < se.j)
	{
		pivot = partition(photons, se, axis);
		if (k < pivot)
		{
			se.j = pivot - 1;
			quicksearch(photons, se, k, axis);
		}
		else if(k > pivot)
		{
			se.i = pivot + 1;
			quicksearch(photons, se, k, axis);
		}
	}
}

//axes: 0 = x, 1 = y, 2 = z;

void	set_median(t_photon *photons, t_position se, int med, int axis)
{
	quicksearch(photons, se, med, axis);
}

t_kdnode *build_tree(t_photon *photons, int start, int end, int depth)
{
	int axis;
	int median;
	t_kdnode *node;
	t_position se;

	if (start >= end)
		return (NULL);
	axis = depth % 3;
	median = (start + end) / 2;
	node = (t_kdnode *)malloc(sizeof(t_kdnode));
	if (!node)
		return (NULL);
	se.i = start;
	se.j = end;
	set_median(photons, se, median, axis);
	node->photon = photons[median];
	node->split_dim = axis;
	node->left = build_tree(photons, start, median, depth + 1);
	node->right = build_tree(photons, median + 1, end, depth + 1);
	return node;
}

void	free_tree(t_kdnode *tree)
{
	if (!tree)
		return ;
	free_tree(tree->left);
	free_tree(tree->right);
	free(tree);
}

/* void	free_mapping(t_trace *trace)
{
	if (trace->gl_tree)
		free_tree(trace->gl_tree);
	if (trace->global_map)
	{
		free(trace->global_map->photons);
		free(trace->global_map);
	}
	if (trace->c_tree)
		free_tree(trace->c_tree);
	if (trace->caustic_map)
	{
		free(trace->caustic_map->photons);
		free(trace->caustic_map);
	}
} */