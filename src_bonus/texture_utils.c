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

/* typedef struct s_tx_vals
{
	int		img_h;
	int		img_w;
	int		i;
	int		j;
	double	xmin;
	double	xmax;
	double	zmin;
	double	zmax;
	double	u;
	double 	v;
	double img_iasp;

}	t_tx_vals; */

t_norm_color texture_plane_at(t_trace *trace, t_point obj_pnt, t_plane *plane)
{
	t_norm_color	col;

	(void)trace;
	//t_img *tx = trace->textures->image;
	int	img_h = plane->texture->i_height;
	int	img_w = plane->texture->i_width;
	double img_iasp = (double)img_h / (double)img_w;

	double				u;
	double				v;
	int					i;
	int					j;

	double xmin =  -5, xmax =  5, zmin = xmin * img_iasp , zmax = xmax * img_iasp;//working. struct these vals...?
	u =  (obj_pnt.x - xmin) / (xmax - xmin);
	v = (obj_pnt.z - zmin) / (zmax - zmin);
	if (u < 0)
		u = -u;
	if (v < 0)
		v = -v;
	u = 1 - fabs(fmod(u, 2.0) - 1.0);
	v = 1 - fabs(fmod(v, 2.0) - 1.0);
	i = ft_round((img_w - 1) * u);
	j = ft_round((img_h - 1) * v);
	col = pixel_color_get(i, j, plane->texture->image);

	//bump map part//using color of text
	//plane->norm = norm_vec(add_vec(plane->norm, set_pl_norm_pert(i, j, plane->texture->image, col)));
	return (col);
}

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
	phi = acos(obj_pnt.y / sphere->radius);
	

	pos.i = ft_round(((theta + M_PI) / (2 * M_PI)) * (double)(dims.i - 1));
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
		comps->bump = sp_bump(pos, dims, sphere->texture->bump_map);
		comps->normal = norm_vec(add_vec(comps->normal, comps->bump));//make a sepbump normal that sets to normal if bump is off or not present
	}
	return (col);
}



//	trace->perturb = set_norm_pert(pos.i, pos.j, trace->image1, col);//set a double in trace struct then apply,
//make sphere->perturb instead?//lookout threads race