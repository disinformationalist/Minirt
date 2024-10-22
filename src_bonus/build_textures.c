#include "minirt.h"

double get_lumin(t_norm_color color)
{
	return ((0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b) / 255.0);
}


uint8_t luminosity(t_norm_color color)
{
	return ((uint8_t)(0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b));
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
// set up a luminosity based grayscale image, change this function to perturb based on the gray value from the image/bumpmap
//could i make the perturb vector, store it in 3darray then just call.

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

t_img 	*create_img(void *mlx_ptr, int width, int height)
{
	t_img *img;
	
	img = (t_img *)malloc(sizeof(t_img));
	if (!img)
		return (NULL);
	if (new_img_init(mlx_ptr, img, width, height) == -1)
	{
		free(img);
		return (NULL);
	}
	return (img);
}

t_img *build_lumin_map(void *mlx_con, t_img *img, int width, int height)
{
	t_img			*bump_map;
	uint8_t			gray_val;
	unsigned int	gray_col;
	int				i;
	int				j;

	bump_map = create_img(mlx_con, width, height);
	if (!bump_map)
		return (NULL);
	j = -1;
	while (++j < height)
	{
		i = -1;
		while (++i < width)
		{
			gray_val = luminosity(pixel_color_get(i, j, img));
			gray_col = (gray_val << 16 | gray_val << 8 | gray_val);
			my_pixel_put(i, j, bump_map, gray_col);
		}
	}
	return (bump_map);
}


//build texture/ bump map list here, if no bump map provided. create a luminosity based

int	import_textures(void *mlx_con, t_tx *textures)
{
	t_tx *curr;

	if (textures == NULL)
		return (0);
	curr = textures;
	while (true)
	{
		curr->image = import_png(mlx_con, curr->i_name, &curr->i_width, &curr->i_height);
		if (!curr->image)
			return (1);
		/* if (curr->m_name)
			curr->bump_map = import_png(mlx_con, curr->m_name, &curr->m_width, &curr->m_height);
		else
			curr->bump_map = build_lumin_map(mlx_con, curr->image, curr->i_width, curr->i_height);
		if (!curr->bump_map)
			return (1); */
		curr = curr->next;
		if (curr == textures)
			break;
	}
	return (0);
}