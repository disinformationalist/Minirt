#include "minirt.h"

// pixel_color_get() retreives color of pixel(x, y) of image
// useful for backround setting or textures

t_norm_color pixel_color_get(int x, int y, t_img *img)
{
	unsigned int	pixcolor;
	
	t_norm_color	pixel_color;
	int 			offset;

	offset = y * img->line_len + (x * (img->bpp / 8));

	/* pixel_color.b = *(img->pixels_ptr + offset);
	pixel_color.g = *(img->pixels_ptr + offset + 1);
	pixel_color.r = *(img->pixels_ptr + offset + 2); */
	pixcolor = *(unsigned int *)(img->pixels_ptr + offset);
	pixel_color.r = (pixcolor >> 16) & 0xFF;
	pixel_color.g = (pixcolor >> 8) & 0xFF;
	pixel_color.b = pixcolor & 0xFF;


	return (pixel_color);
}

/* unsigned int pixel_color_get(int x, int y, t_img *img)
{
	unsigned int	pixel_color;
	int 			offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	pixel_color = *(unsigned int *)(img->pixels_ptr + offset);
	return (pixel_color);
} */


t_norm_color texture_plane_at(t_trace *trace, t_point point, t_matrix_4x4 transform)//place height / width with the image
{
	t_norm_color	col;
	t_point			obj_pnt;
	double				u;
	double				v;
	int i,j;
	double xmin =  -8, xmax =  8, zmin = -4.5 , zmax = 4.5 ;//set these base upon aspect ratio of in image and image size, window size

	
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

	/*u = u % trace->image1_w;
	v = v % trace->image1_h; */
	col = pixel_color_get(i, j, trace->image1);
	return (col);
}

t_norm_color texture_sp_at(t_trace *trace, t_point point, t_matrix_4x4 transform)//place height / width with the image
{
	t_norm_color	col;
	t_point			obj_pnt;
	int				u;
	int				v;
	double			theta;
	double			phi;



	obj_pnt = mat_vec_mult(transform, point);
	theta = atan2(obj_pnt.z, obj_pnt.x);
	phi = acos(obj_pnt.y);

	u = (int)(((theta + M_PI) / (2 * M_PI)) * (trace->image1_w - 1));
	v = (int)((phi / M_PI) * (trace->image1_h - 1));

	if (u < 0)
		u = -u;
	if (v < 0)
		v = -v;
	u = u % trace->image1_w;
	v = v % trace->image1_h;
	col = pixel_color_get(u, v, trace->image1);
	return (col);
}