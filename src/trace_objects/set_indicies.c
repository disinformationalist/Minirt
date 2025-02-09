#include "minirt.h"

static inline void	add_con(t_cons *cons, int *size, void *obj, t_type type)
{
	if (*size < 200)
	{
		cons[*size].obj = obj;
		cons[*size].type = type;
		(*size)++;
	}
	else
		return ;
}

static inline void	set_n(double *n, t_cons *con)
{
	if (con->type == SPHERE)
		*n = ((t_sphere *)con->obj)->mat.refract;
	else if (con->type == PLANE)
		*n = ((t_plane *)con->obj)->mat.refract;
	else if (con->type == CYLINDER)
		*n = ((t_cylinder *)con->obj)->mat.refract;
	else if (con->type == CUBE)
		*n = ((t_cube *)con->obj)->mat.refract;
	else if (con->type == HYPERBOLOID)
		*n = ((t_hyperboloid *)con->obj)->mat.refract;
	else
		return ;
}

static inline void	check_and_set(int size, double *n, t_cons *cons)
{
	if (size == 0)
		*n = 1.0;
	else
		set_n(n, &cons[size - 1]);
}

static inline int	search_cons(int *size, t_track_hits *current, t_cons *cons)
{
	int	found;
	int	k;
	int	j;

	found = 0;
	j = -1;
	while (++j < *size)
	{
		if (cons[j].obj == current->object)
		{
			found = 1;
			k = j;
			while (k < *size -1)
			{
				cons[k] = cons[k + 1];
				k++;
			}
			(*size)--;
			break ;
		}
	}
	return (found);
}

//set indicies of refraction at a point

void	set_indicies(t_intersects *intersects, double *n1, double *n2)
{
	t_track_hits	*current;
	t_cons			cons[200];
	int				size;
	int				i;
	int				found;

	size = 0;
	*n1 = 1.0;
	*n2 = 1.0;
	i = -1;
	while (++i < intersects->count)
	{
		current = &intersects->hits[i];
		if (current->t == intersects->closest->t)
			check_and_set(size, n1, cons);
		found = search_cons(&size, current, cons);
		if (!found)
			add_con(cons, &size, current->object, current->object_type);
		if (current->t == intersects->closest->t)
		{
			check_and_set(size, n2, cons);
			break ;
		}
	}
}
