#include "image_processing.h"

//output of this is jagged, do before downsample

unsigned int **pm_circle_matrix(unsigned int **in, int width, int height, int layers, int axis_radius)
{
	unsigned int		**out;
	int					padded_width;
	int					padded_height;
	int					dist;
	int 				x;
	int 				y;
	int 				cx;
	int 				cy;

	cx = width / 2;
	cy = height / 2;
	padded_width = 2 * axis_radius + 2 * layers;
	padded_height = 2 * axis_radius + 2 * layers;
	out = malloc_ui_matrix(padded_width, padded_height);
	if (!out)
		return (NULL);
	y = -1;
	while (++y < padded_height)
	{
		x = -1;
		while (++x < padded_width)
		{
			int src_x = x - layers + (cx - axis_radius);
			int src_y = y - layers + (cy - axis_radius);
			int dx = src_x - cx;
			int dy = src_y - cy;
			dist = sqrt(dx * dx + dy * dy);
			if (dist <= axis_radius)
					out[y][x] = in[src_y][src_x];
			else
			{
				 double angle = atan2(dy, dx);
                    int reflected_x = cx + (int)((axis_radius - (dist - axis_radius)) * cos(angle));
                    int reflected_y = cy + (int)((axis_radius - (dist - axis_radius)) * sin(angle));

				
				 /*  double ratio = (double)axis_radius / dist;
                int reflected_x = cx + (int)(dx * ratio);
                int reflected_y = cy + (int)(dy * ratio); */
				
				/* double angle = atan2(dy, dx);
				int reflected_x = cx + (int)(axis_radius * cos(angle));
				int reflected_y = cy + (int)(axis_radius * sin(angle)); */
				if ((x >= 0 && x < padded_width) && ( y >= 0 && y < padded_height))
					out[y][x] = in[reflected_y][reflected_x];
					//my_pixel_put(x, y, img_pad, pixel_color_get(reflected_x, reflected_y, img));
			}
		}	
	}	
	return (out);
}

t_img	*pad_mirror_circle(void *mlx_ptr, t_img *img, int width, int height, int layers, int axis_radius)
{
	t_img	*img_pad;
	int		padded_width;
	int		padded_height;
	int		dist;
	int x;
	int y;
	int cx;
	int cy;

	
	cx = width / 2;
	cy = height / 2;
	padded_width = 2 * axis_radius + 2 * layers;
	padded_height = 2 * axis_radius + 2 * layers;
	img_pad = create_img(mlx_ptr, padded_width, padded_height);
	if (!img_pad)
		return (NULL);
	y = -1;
	while (++y < padded_height)
	{
		x = -1;
		while (++x < padded_width)
		{
			int src_x = x - layers + (cx - axis_radius);
			int src_y = y - layers + (cy - axis_radius);
			int dx = src_x - cx;
			int dy = src_y - cy;
			dist = sqrt(dx * dx + dy * dy);
			if (dist <= axis_radius)
				my_pixel_put(x, y, img_pad, pixel_color_get(src_x, src_y, img));
			else
			{
				 double angle = atan2(dy, dx);
                    int reflected_x = cx + (int)((axis_radius - (dist - axis_radius)) * cos(angle));
                    int reflected_y = cy + (int)((axis_radius - (dist - axis_radius)) * sin(angle));

				
				 /*  double ratio = (double)axis_radius / dist;
                int reflected_x = cx + (int)(dx * ratio);
                int reflected_y = cy + (int)(dy * ratio); */
				
				/* double angle = atan2(dy, dx);
				int reflected_x = cx + (int)(axis_radius * cos(angle));
				int reflected_y = cy + (int)(axis_radius * sin(angle)); */
				if ((x >= 0 && x < padded_width) && ( y >= 0 && y < padded_height))
					my_pixel_put(x, y, img_pad, pixel_color_get(reflected_x, reflected_y, img));
			}
		}	
	}	
	return (img_pad);
}

t_img	*loop_reflector_circle(void *mlx_ptr, t_img *img, int *width, int *height, int layers, int axis_radius, int iters)//seems ok..
{
	t_img	*img_pad;
	t_img	*img_nxt;
	int 	i;
	
	img_pad = pad_mirror_circle(mlx_ptr, img, *width, *height, layers, axis_radius);
	i = 0;
	*height = 2 * axis_radius + 2 * layers;
	*width = 2 * axis_radius + 2 * layers;
	while (++i < iters)
	{
		axis_radius += layers;
		img_nxt = pad_mirror_circle(mlx_ptr, img_pad, *width, *height, layers, axis_radius);
		mlx_destroy_image(mlx_ptr, img_pad->img_ptr);
		free(img_pad);
		*height += 2 * layers;
		*width += 2 * layers;
		img_pad = img_dup(mlx_ptr, img_nxt, *width, *height);
		mlx_destroy_image(mlx_ptr, img_nxt->img_ptr);
		free(img_nxt);
	} 
	
	return (img_pad);
}