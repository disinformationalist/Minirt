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

unsigned char pixel_gray_get(int x, int y, t_img *img)
{
	unsigned char	gray;
	int 			offset;

	offset = y * img->line_len + (x * (img->bpp / 8));
	gray = *(unsigned char *)(img->pixels_ptr + offset);
	return (gray);
}

typedef struct s_tx_vals
{
	/* int		img_h;
	int		img_w;
	int		i;
	int		j; */
	double	xmin;
	double	xmax;
	double	zmin;
	double	zmax;
	double	u;
	double 	v;
	double img_iasp;

}	t_tx_vals;

/* comp_uv(t_point obj_pnt, double xmin, double xmax, double iasp)
{
	
} */


t_norm_color texture_plane_at(t_trace *trace, t_point obj_pnt, t_plane *plane, t_comps *comps)
{
	t_norm_color	col;
	t_tx_vals		vals;
	t_position		dims;
	t_position		pos;

	(void)trace;
	//t_img *tx = trace->textures->image;
	dims.j = plane->texture->i_height;
	dims.i = plane->texture->i_width;
	vals.img_iasp = (double)dims.j / (double)dims.i;

	vals.xmin =  -5;
	vals.xmax =  5;
	vals.zmin = vals.xmin * vals.img_iasp;
	vals.zmax = vals.xmax * vals.img_iasp;//working. struct these vals...?
	vals.u =  (obj_pnt.x - vals.xmin) / (vals.xmax - vals.xmin);
	vals.v = (obj_pnt.z - vals.zmin) / (vals.zmax - vals.zmin);
	if (vals.u < 0)
		vals.u = -vals.u;
	if (vals.v < 0)
		vals.v = -vals.v;
	vals.u = 1 - fabs(fmod(vals.u, 2.0) - 1.0);
	vals.v = 1 - fabs(fmod(vals.v, 2.0) - 1.0);
	pos.i = ft_round((dims.i - 1) * vals.u);
	pos.j = ft_round((dims.j - 1) * vals.v);
	col = pixel_color_get(pos.i, pos.j, plane->texture->image);

	if (plane->bump)
	{
		comps->bump = sp_bump(pos, dims, plane->texture->bump_map);
		comps->normal = norm_vec(add_vec(comps->normal, comps->bump));//make a sepbump normal that sets to normal if bump is off or not present
	
	}
	//bump map part//using color of text
	//plane->norm = norm_vec(add_vec(plane->norm, set_pl_norm_pert(i, j, plane->texture->image, col)));
	return (col);
}

//try other way used in the checker here...

t_norm_color texture_sp_at(t_trace *trace, t_point obj_pnt, t_sphere *sphere, t_comps *comps)
{
	t_norm_color	col;
	t_position		pos;
	double			theta;
	double			phi;

	t_position	dims;
	
	dims.i = sphere->texture->i_width;
	dims.j = sphere->texture->i_height;

	(void)trace;
	theta = atan2(obj_pnt.z, obj_pnt.x);
	phi = acos(obj_pnt.y);// / sphere->radius);
	

	pos.i = ft_round(((theta + M_PI) / (M_PI)) * (double)(dims.i - 1));// could go out of bound on dims *2 on div mpi
	pos.j = ft_round((phi / M_PI) * (double)(dims.j - 1));

	if (pos.i < 0)
		pos.i = -pos.i;
	if (pos.j < 0)
		pos.j = -pos.j;
	pos.i = pos.i % (dims.i - 1);
	pos.j = pos.j % (dims.j - 1);
	col = pixel_color_get(pos.i, pos.j, sphere->texture->image);
	if (sphere->bump)
	{
		t_vec3 w_bump;
		comps->bump = sp_bump(pos, dims, sphere->texture->bump_map);
		t_vec3 tangent = vec(-sin(theta), 0, cos(theta), 0);
		t_vec3 bitangent = vec(cos(theta) * cos(phi), -sin(phi), sin(theta) *  cos(phi), 0);
		
		w_bump = add_vec(scale_vec(comps->bump.x, tangent), scale_vec(comps->bump.y, bitangent));
		w_bump = add_vec(scale_vec(comps->bump.z, comps->normal), w_bump);
		

		comps->normal = norm_vec(w_bump);
		//comps->normal = norm_vec(add_vec(comps->normal, comps->bump));//make a sepbump normal that sets to normal if bump is off or not present
	
	}
	return (col);
}



//	trace->perturb = set_norm_pert(pos.i, pos.j, trace->image1, col);//set a double in trace struct then apply,
//make sphere->perturb instead?//lookout threads race