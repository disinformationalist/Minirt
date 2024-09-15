#include "minirt.h"

bool	veccmp(t_vec3 v1, t_vec3 v2)
{
	if (v1.x != v2.x)
		return (true);
	if (v1.y != v2.y)
		return (true);
	if (v1.z != v2.z)
		return (true);
	return (false);
}

//rodriguez formula to get rotation of any dir vec to up vec(inverse transform rotation)
//takes normed vecs...
//not working, weird scaling.
t_matrix_4x4 inv_rot(t_vec3 start, t_vec3 target)
{
	t_vec3 			axis;
	double 			angle;
	t_matrix_4x4	res;
start = norm_vec(start);
target = norm_vec(target);
/* 	if (fabs(angle) < 1e-6)// already aligned
		return (identity(&res), res); */
	//if ortho handle todo
	if (!veccmp(start, vec(0, 1, 0, 0)))
		return (identity(&res), res);
	if (!veccmp(start, vec(0, -1, 0, 0)))
		return (rot_z(M_PI));
	if (!veccmp(start, vec(1, 0, 0, 0)))
		return (rot_z(-M_PI / 2));
	if (!veccmp(start, vec(-1, 0, 0, 0)))
		return (rot_z(M_PI / 2));
	axis = cross_prod(start, target);
	norm_vec(axis);
	angle  = acos(dot(start, target));
/* 		 if (fabs(fabs(angle) - M_PI) < 1e-6) {
        axis = cross_prod(start, vec(1, 0, 0, 0));
        if (sqrt(dot(axis, axis)) < 1e-6) { // Default axis is collinear with the rotation axis
            axis = cross_prod(start, vec(0, 1, 0, 0));
        }
        axis = norm_vec(axis);
        angle = M_PI; }*/
    
	
	//use rod to get
	double cos_theta;
	double sin_theta;

	double x;
	double y;
	double z;

	x = axis.x;
	y = axis.y;
	z = axis.z;

	cos_theta = cos(angle);
	sin_theta = sin(angle);

	res.m[0][0] = cos_theta + x * x * (1 - cos_theta);
	res.m[0][1] = x * y * (1 - cos_theta) - z * sin_theta;
	res.m[0][2] = x * z * (1 - cos_theta) + y * sin_theta;
	res.m[0][3] = 0;

	res.m[1][0] = y * x * (1 - cos_theta) + z * sin_theta;
	res.m[1][1] = cos_theta + y * y * (1 - cos_theta);
	res.m[1][2] = y * z * (1 - cos_theta) - x * sin_theta;
	res.m[1][3] = 0;

	res.m[2][0] = z * x * (1 - cos_theta) - y * sin_theta;
	res.m[2][1] = z * y * (1 - cos_theta) + x * sin_theta;
	res.m[2][2] = cos_theta + z * z * (1 - cos_theta);
	res.m[2][3] = 0;

	res.m[3][0] = 0;
	res.m[3][1] = 0;
	res.m[3][2] = 0;
	res.m[3][3] = 1;

	return (res);
}


//set all transforms to account for position and orientation.

void	set_sp_transforms(t_trace *trace)
{
	t_sphere	*curr_sp;

	if (trace->spheres)
	{
		curr_sp = trace->spheres;
		while (true)
		{
			t_matrix_4x4 inv_scale = inv_scaling(curr_sp->radius, curr_sp->radius, curr_sp->radius);
			t_matrix_4x4 inv_tran = translation(-curr_sp->center.x, -curr_sp->center.y, -curr_sp->center.z);// building the transform working
			curr_sp->transform = (mat_mult(inv_scale, inv_tran));
 
			/* t_matrix_4x4 tran = translation(curr_sp->center.x, curr_sp->center.y, curr_sp->center.z);// building the forward transform and inverse, also works
			t_matrix_4x4 scale = scaling(curr_sp->radius, curr_sp->radius, curr_sp->radius);
			curr_sp->transform = inverse(mat_mult(tran, scale)); */
			curr_sp = curr_sp->next;
			if (curr_sp == trace->spheres)
				break;
		}
	}
}
// use orientation vec to get the rotations from assumed normal pointing upward.
//building method for getting the right rotation here


//will need inverse(transform), transpose(inverse(transform)) just assign inverse, maybe both.


t_matrix_4x4 get_rot(t_vec3 ori)// must use this + normal, then invert... use rodrigues see chat
{
	t_matrix_4x4	res;
	double			yaw;
	double			pitch;
	t_matrix_4x4	y_rot;
	t_matrix_4x4	x_rot;

	if (!veccmp(ori, vec(0, 1, 0, 0)))
		return (identity(&res), res);
	if (!veccmp(ori, vec(0, -1, 0, 0)))
		return (rot_z(M_PI));
	if (!veccmp(ori, vec(1, 0, 0, 0)))
		return (rot_z(M_PI / 2));
	if (!veccmp(ori, vec(-1, 0, 0, 0)))
		return (rot_z(-M_PI / 2));
	if (!veccmp(ori, vec(0, 0, 1, 0)))
		return (rot_x(M_PI / 2));
	if (!veccmp(ori, vec(0, 0, -1, 0)))
		return (rot_x(-M_PI / 2));
	yaw = atan2(ori.x, ori.z);
	pitch = asin(ori.y);
	y_rot = rot_y(yaw);
	x_rot = rot_x(pitch);
	return (mat_mult(x_rot, y_rot));
}

void	set_pl_transforms(t_trace *trace)
{
	t_plane	*curr_pl;

	if (trace->planes)
	{
		curr_pl = trace->planes;
		while (true)
		{
			
			//combine inv_tran and inv_rot inverse for the transform.
			//this isnt working right...
			
			t_matrix_4x4 inv_trans = translation(-curr_pl->point.x, -curr_pl->point.y, -curr_pl->point.z);
			//curr_pl->transform = inv_trans;

			//curr_pl->transform = mat_mult(inv_trans, inv_rot(curr_pl->norm, vec(0, 1, 0, 0)));
			
			
			curr_pl->transform = mat_mult(get_rot(curr_pl->norm), inv_trans);
			//curr_pl->transform = mat_mult(get_rot(curr_pl->norm), inv_trans);
	
			curr_pl = curr_pl->next;
			if (curr_pl == trace->planes)
				break;
		}
	}
}

void	set_cy_transforms(t_trace *trace)
{
	t_cylinder	*curr_cy;

	if (trace->cylinders)
	{
		curr_cy = trace->cylinders;
		while (true)
		{
			norm_vec(curr_cy->norm);
			t_matrix_4x4 inv_trans = translation(-curr_cy->center.x, -curr_cy->center.y, -curr_cy->center.z);// building the transform working
		t_matrix_4x4 scale = inv_scaling(curr_cy->radius, curr_cy->radius, curr_cy->radius);
			//curr_cy->transform = mat_mult(get_rot(curr_cy->norm), mat_mult(scale, inv_trans));
	curr_cy->transform = mat_mult(get_rot(curr_cy->norm), mat_mult(scale, inv_trans));
			//curr_cy->transform = mat_mult(scale, mat_mult(get_rot(curr_cy->norm), inv_trans));
			/* t_matrix_4x4 id;
			identity(&id);
			curr_cy->transform = inv_trans; */
			//print_matrix_4(&curr_cy->transform);
			curr_cy = curr_cy->next;
			if (curr_cy == trace->cylinders)
				break;
	}
		}
}

void	render(t_trace *trace)
{
	//set all transforms here maybe move to outside render later... must adjust hooks
	set_sp_transforms(trace);
	set_pl_transforms(trace);
	set_cy_transforms(trace);

	if (trace->supersample)
	{
		ft_putstr_color_fd(1, "SUPERSAMPLE IN PROGRESS...\n", BOLD_GREEN);
		trace->pixels_xl = malloc_ui_matrix(trace->width_orig * trace->s_kernel, trace->height_orig * trace->s_kernel);
		if (!trace->pixels_xl)
		{
			printf(RED"The super malloc has failed\n"RESET);
			close_win(trace);
		}
		zero_ui_matrix(trace->pixels_xl, trace->width, trace->height);
	}
	
	long start = get_time();//test
	render_scene(trace);
	
	if (trace->supersample)
	{
		downsample_xl(trace->width, trace->height, &trace->img, trace->pixels_xl, trace->s_kernel);
		zero_ui_matrix(trace->pixels_xl, trace->width, trace->height);
		free_ui_matrix(trace->pixels_xl, trace->height);
		ft_putstr_color_fd(1, "SUPERSAMPLE COMPLETE!\n", BOLD_BRIGHT_GREEN);
	}
	mlx_put_image_to_window(trace->mlx_connect,
		trace->mlx_win, trace->img.img_ptr, 0, 0);
	print_times(start, get_time(), "RENDER TIME: %f seconds\n");//test
}
