#include "minirt.h"

// adds b2 to b1 (b1 expands to contain b2)

void	add_to(t_box *b1, t_box b2)
{
	add_pnt(b1, b2.min);
	add_pnt(b1, b2.max);
}

//true if point is within box

bool	contains_pnt(t_box box, t_point point)
{
	bool	in_x;
	bool	in_y;
	bool	in_z;

	in_x = point.x >= box.min.x && point.x <= box.max.x;
	in_y = point.y >= box.min.y && point.y <= box.max.y;
	in_z = point.z >= box.min.z && point.z <= box.max.z;
	return (in_x && in_y && in_z);
}

//true if b1 contains b2

bool	contains_box(t_box b1, t_box b2)
{
	return (contains_pnt(b1, b2.min) && contains_pnt(b1, b2.max));
}

//return a new box with a transform applied to an existing one

t_box	*transform_box(t_box *box, t_mat4 transf)
{
	t_point	point;
	t_box	*new;

	if (!box)
		return (NULL);
	new = empty_box();
	if (!new)
		return (NULL);
	point = box->min;
	add_pnt(new, mat_vec_mult(transf, point));
	point = vec(box->min.x, box->min.y, box->max.z, 1);
	add_pnt(new, mat_vec_mult(transf, point));
	point = vec(box->min.x, box->max.y, box->min.z, 1);
	add_pnt(new, mat_vec_mult(transf, point));
	point = vec(box->min.x, box->max.y, box->max.z, 1);
	add_pnt(new, mat_vec_mult(transf, point));
	point = vec(box->max.x, box->min.y, box->min.z, 1);
	add_pnt(new, mat_vec_mult(transf, point));
	point = vec(box->max.x, box->min.y, box->max.z, 1);
	add_pnt(new, mat_vec_mult(transf, point));
	point = vec(box->max.x, box->max.y, box->min.z, 1);
	add_pnt(new, mat_vec_mult(transf, point));
	point = box->max;
	add_pnt(new, mat_vec_mult(transf, point));
	free(box);
	return (new);
}
