#include "minirt.h"

static inline void	set_pixel00(t_trace *trace, t_point view_topleft, \
	t_vec3 right, t_vec3 true_up)
{
	t_vec3	move;
	t_vec3	scale;

	trace->pix_delta_rht = scale_vec(trace->pixel_width, right);
	trace->pix_delta_down = scale_vec(trace->pixel_height, neg(true_up));
	trace->n2 = trace->n * trace->n;
	move = add_vec(trace->pix_delta_down, trace->pix_delta_rht);
	if (trace->supersample)
	{
		scale = scale_vec(1.0 / (2.0 * trace->n), move);
		trace->pixel00 = add_vec(view_topleft, scale);
	}
	else
		trace->pixel00 = add_vec(view_topleft, scale_vec(0.5, move));
	trace->move_x = scale_vec(1.0 / trace->n, trace->pix_delta_rht);
	trace->move_y = scale_vec(1.0 / trace->n, trace->pix_delta_down);
}

static inline void	set_view_topleft(t_trace *trace, t_vec3 view_center, \
	double view_width, double view_height)
{
	t_point	view_topleft;
	t_vec3	horizontal_move;
	t_vec3	vertical_move;
	t_vec3	right;

	if (!veccmp(trace->cam->orient, vec(0, 1, 0, 0)))
	{
		right = vec(1, 0, 0, 0);
		trace->cam->true_up = vec(0, 0, -1, 0);
	}
	else if (!veccmp(trace->cam->orient, vec(0, -1, 0, 0)))
	{
		right = vec(1, 0, 0, 0);
		trace->cam->true_up = vec(0, 0, 1, 0);
	}
	else
	{
		right = norm_vec(cross_prod(vec(0, 1, 0, 0), trace->cam->orient));
		trace->cam->true_up = norm_vec(cross_prod(trace->cam->orient, right));
	}
	horizontal_move = scale_vec(view_width / 2.0, right);
	vertical_move = scale_vec(view_height / 2.0, trace->cam->true_up);
	view_topleft = add_vec(view_center, vertical_move);
	view_topleft = subtract_vec(view_topleft, horizontal_move);
	set_pixel00(trace, view_topleft, right, trace->cam->true_up);
}

void	init_viewing(t_trace *trace)
{
	t_point	view_center;
	double	focal_len;
	double	view_width;
	double	view_height;

	trace->cam->orient = norm_vec(trace->cam->orient);
	focal_len = 1.0;
	view_center = add_vec(trace->cam->center, \
		scale_vec(1.0 / focal_len, trace->cam->orient));
	view_width = 2.0 * tan((double)(trace->cam->fov / 2.0) * DEG_TO_RAD);
	view_height = view_width / ASPECT;
	trace->pixel_width = view_width / (double)trace->width;
	trace->pixel_height = view_height / (double)trace->height;
	set_view_topleft(trace, view_center, view_width, view_height);
	trace->cam->transform = rot_to(vec(0, 0, 1, 0), trace->cam->orient);
	trace->cam->transform_up = rot_to(vec(0, 1, 0, 0), trace->cam->true_up);
	trace->cam->rottran = trace->cam->transform_up;
	trace->cam->rots = vec(0, 0, 0, 0);
}

static inline void	reset_topleft(t_trace *trace, t_vec3 view_center, \
	double view_width, double view_height)
{
	t_point	view_topleft;
	t_vec3	horizontal_move;
	t_vec3	vertical_move;
	t_vec3	right;
	t_vec3	true_up;

	true_up = trace->cam->true_up;
	right = norm_vec(cross_prod(true_up, trace->cam->orient));
	horizontal_move = scale_vec(view_width / 2.0, right);
	vertical_move = scale_vec(view_height / 2.0, true_up);
	view_topleft = add_vec(view_center, vertical_move);
	view_topleft = subtract_vec(view_topleft, horizontal_move);
	set_pixel00(trace, view_topleft, right, true_up);
}

//resets the camera after rotating, moving

void	reinit_viewing(t_trace *trace)
{
	t_point	view_center;
	double	focal_len;
	double	view_width;
	double	view_height;

	trace->cam->orient = norm_vec(trace->cam->orient);
	focal_len = 1.0;
	view_center = add_vec(trace->cam->center, \
		scale_vec(1.0 / focal_len, trace->cam->orient));
	view_width = 2.0 * tan((double)(trace->cam->fov / 2.0) * DEG_TO_RAD);
	view_height = view_width / ASPECT;
	trace->pixel_width = view_width / (double)trace->width;
	trace->pixel_height = view_height / (double)trace->height;
	reset_topleft(trace, view_center, view_width, view_height);
}
