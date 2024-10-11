#include "minirt.h"

// pixel_color_get() retreives color of pixel(x, y) of image
// useful for backround setting or textures

t_norm_color pixel_color_get(int x, int y, t_img *img)
{
	unsigned int	pixcolor;
	t_norm_color	pixel_color;
	int 			offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	pixcolor = *(unsigned int *)(img->pixels_ptr + offset);
	pixel_color.r = (pixcolor >> 16) & 0xFF;
	pixel_color.g = (pixcolor >> 8) & 0xFF;
	pixel_color.b = pixcolor & 0xFF;


	return (pixel_color);
}



//---------luminosity based bump mapping

double get_lumin(t_norm_color color)
{
	return ((0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b) / 255.0);
}

t_vec3 set_sp_norm_pert(int i, int j, t_img *img, t_norm_color col)
{
	t_vec3 perturb;
	double lumin_col;
	double dfdx;
	double dfdy;
	double norm_factor;
	double bump = .25;
	

	lumin_col = get_lumin(col);

	dfdx = get_lumin(pixel_color_get(i + 1, j, img)) - lumin_col;	
	dfdy = get_lumin(pixel_color_get(i, j + 1, img)) - lumin_col;

	norm_factor = 1.0 / (sqrt(dfdx * dfdx + dfdy * dfdy + 1));
	perturb.x = -dfdx * norm_factor * bump;
	perturb.y = -dfdy * norm_factor * bump;
	perturb.z = norm_factor * bump;

	return (perturb);
}


t_vec3 set_pl_norm_pert(int i, int j, t_img *img, t_norm_color col)
{
	t_vec3 perturb;
	double lumin_col;
	double dfdx;
	double dfdy;
	double norm_factor;
	double bump = .25;
	

	lumin_col = get_lumin(col);

	dfdx = get_lumin(pixel_color_get(i + 1, j, img)) - lumin_col;	
	dfdy = get_lumin(pixel_color_get(i, j + 1, img)) - lumin_col;

	norm_factor = 1.0 / (sqrt(dfdx * dfdx + dfdy * dfdy + 1));
	perturb.x = -dfdx * norm_factor * bump;
	perturb.z = -dfdy * norm_factor * bump;
	perturb.y = norm_factor * bump;


	return (perturb);
}
//------------

t_norm_color texture_plane_at(t_trace *trace, t_point point, t_matrix_4x4 transform, t_vec3 *norm)//place height / width with the image
{
	t_norm_color	col;
	t_point			obj_pnt;
	double				u;
	double				v;
	int i, j;
	double xmin =  -4.5, xmax =  4.5, zmin = -4.5 , zmax = 4.5; //8,4.5//set these base upon aspect ratio of in image and image size, window size

(void)norm;
	
	//double aspect = trace->image1_w / trace->image1_h;

	obj_pnt = mat_vec_mult(transform, point);
	/* u = ft_round((trace->image1_w - 1) * (obj_pnt.x - xmin) / (xmax - xmin));
	v = ft_round((trace->image1_h - 1) * (obj_pnt.z - zmin) / (zmax - zmin));
 */
	u =  (obj_pnt.x - xmin) / (xmax - xmin);
	v = (obj_pnt.z - zmin) / (zmax - zmin);
	if (u < 0)
		u = -u;
	if (v < 0)
		v = -v;
	u = 1 - fabs(fmod(u, 2.0) - 1.0);
	v = 1 - fabs(fmod(v, 2.0) - 1.0);
	i = ft_round((trace->image1_w - 1) * u);
	j = ft_round((trace->image1_h - 1) * v);

	col = pixel_color_get(i, j, trace->image1);
	//bump map part
	//*norm = norm_vec(add_vec(*norm, set_pl_norm_pert(i, j, trace->image1, col)));
	return (col);
}

t_norm_color texture_sp_at(t_trace *trace, t_point point, t_matrix_4x4 transform, t_vec3 *norm)//place height / width with the image
{
	t_norm_color	col;
	t_point			obj_pnt;
	t_position		pos;
	double			theta;
	double			phi;

(void)norm;
	obj_pnt = mat_vec_mult(transform, point);
	theta = atan2(obj_pnt.z, obj_pnt.x);
	phi = acos(obj_pnt.y);

	pos.i = ft_round(((theta + M_PI) / (2 * M_PI)) * (trace->image1_w - 1));
	pos.j = ft_round((phi / M_PI) * (trace->image1_h - 1));//* Aspect?

	if (pos.i < 0)
		pos.i = -pos.i;
	if (pos.j < 0)
		pos.j = -pos.j;
	pos.i = pos.i % (trace->image1_w - 1);
	pos.j = pos.j % (trace->image1_h - 1);
	col = pixel_color_get(pos.i, pos.j, trace->image1);
	//maybe place if() here
	//*norm = norm_vec(add_vec(*norm, set_sp_norm_pert(pos.i, pos.j, trace->image1, col)));

//	trace->perturb = set_norm_pert(pos.i, pos.j, trace->image1, col);//set a double in trace struct then apply,
	return (col);
}

