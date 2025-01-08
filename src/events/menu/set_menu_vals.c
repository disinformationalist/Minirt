#include "minirt.h"

t_norm_color	get_obj_color2(t_on *on)
{
	if (on->type == LIGHT)
		return (mult_color(255, (((t_light *)on->object)->color)));
	else if (on->type == SPHERE)
		return (((t_sphere *)on->object)->color);
	else if (on->type == PLANE)
		return (((t_plane *)on->object)->color);
	else if (on->type == CYLINDER)
		return (((t_cylinder *)on->object)->color);
	else if (on->type == CUBE)
		return (((t_cube *)on->object)->color);
	else if (on->type == HYPERBOLOID)
		return (((t_hyperboloid *)on->object)->color);
	else
		return (color(0, 0, 0));
}

char	*get_tx(t_on *on)
{
	if (on->type == SPHERE)
		return (((t_sphere *)on->object)->texture->i_name);
	else if (on->type == PLANE)
		return (((t_plane *)on->object)->texture->i_name);
	else if (on->type == CYLINDER)
		return (((t_cylinder *)on->object)->texture->i_name);
	else if (on->type == CUBE)
		return (((t_cube *)on->object)->texture->i_name);
	else if (on->type == HYPERBOLOID)
		return (((t_hyperboloid *)on->object)->texture->i_name);
	/* else if (on->type == LIGHT)
		return ((t_mat){0.8, 0.9, 0.9, 200.0, 0.0, 0.0, 1.0}); */
	else
		return ("");
}

char	*copy_till(char *s1, char *s2, char c)
{
	int i = -1;

	while (s1[++i] != c && i < 9)
			s2[i] = s1[i];
	return (s2);
}

void	set_color(void *mlx_con, void *mlx_win, t_on *on)
{
	t_norm_color	color;
	unsigned int	hex_col;
	char 			r[10];
	char 			g[10];
	char 			b[10];
	char			hex[20];
	int				y_s;
	int				opt;
	unsigned int	color1;

	color1 = 0x90C4FF;	
	opt = get_option(on);
	color = get_obj_color2(on);
	if (opt == 1)
		copy_till(ft_strchr(get_tx(on), '/') + 1, hex, '.');
	else if (opt == 2)
		ft_strcpy(hex, "Checker");
	else
	{
		hex_col = (unsigned int)(((uint8_t)color.r << 16) | ((uint8_t)color.g << 8) | ((uint8_t)color.b));
		sprintf(hex, "0x%x", hex_col);
	}

	sprintf(r, "%d", (uint8_t)color.r);
	sprintf(g, "%d", (uint8_t)color.g);
	sprintf(b, "%d", (uint8_t)color.b);
	mlx_string_put(mlx_con, mlx_win, 315, 87, color1, hex);
	y_s = 117;
	mlx_string_put(mlx_con, mlx_win, 327, y_s, color1, r);
	mlx_string_put(mlx_con, mlx_win, 327, y_s += 25, color1, g);
	mlx_string_put(mlx_con, mlx_win, 327, y_s += 25, color1, b);
	return ;
}

void	set_type(void *mlx_con, void *mlx_win, t_type type)
{
	unsigned int	color;

	color = 0x90C4FF;	
	if (type == SPHERE)
		mlx_string_put(mlx_con, mlx_win, 182, 58, color, "SPHERE");
	else if (type == PLANE)
		mlx_string_put(mlx_con, mlx_win, 185, 58, color, "PLANE");
	else if (type == CYLINDER)
		mlx_string_put(mlx_con, mlx_win, 176, 58, color, "CYLINDER");
	else if (type == HYPERBOLOID)
		mlx_string_put(mlx_con, mlx_win, 168, 58, color, "HYPERBOLOID");
	else if (type == CUBE)
		mlx_string_put(mlx_con, mlx_win, 187, 58, color, "CUBE");
	else if (type == LIGHT)
		mlx_string_put(mlx_con, mlx_win, 184, 58, color, "LIGHT");
	else if (type == CAM)
		mlx_string_put(mlx_con, mlx_win, 182, 58, color, "CAMERA");
	else if (type == VOID)
		mlx_string_put(mlx_con, mlx_win, 187, 58, color, "VOID");
}

t_mat	get_obj_mat(t_on *on)
{
	if (on->type == SPHERE)
		return (((t_sphere *)on->object)->mat);
	else if (on->type == PLANE)
		return (((t_plane *)on->object)->mat);
	else if (on->type == CYLINDER)
		return (((t_cylinder *)on->object)->mat);
	else if (on->type == CUBE)
		return (((t_cube *)on->object)->mat);
	else if (on->type == HYPERBOLOID)
		return (((t_hyperboloid *)on->object)->mat);
	/* else if (on->type == LIGHT)
		return ((t_mat){0.8, 0.9, 0.9, 200.0, 0.0, 0.0, 1.0}); */
	else
		return ((t_mat){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 9});
}

//properties: {amb, diff, spec, shine, reflect, transp, refract}

void	set_props(void *mlx_con, void *mlx_win, t_on *on)
{
	t_mat			mat;
	char			prop[10];
	int				y_s;
	int				x_s;
	unsigned int	color;


	color = 0x90C4FF;
	
	x_s = 324;
	mat = get_obj_mat(on);
	y_s = 196;
	
	if (mat.preset == 0)
		ft_strcpy(prop, "Default");	
	else if (mat.preset == 1)
		ft_strcpy(prop, "Shimmer");
	else if (mat.preset == 2)
		ft_strcpy(prop, "Matte");
	else if (mat.preset == 3)
		ft_strcpy(prop, "Glass");
	else if (mat.preset == 4)
		ft_strcpy(prop, "Mirror");
	else if (mat.preset == 5)
		ft_strcpy(prop, "Diamond");
	else if (mat.preset == 6)
		ft_strcpy(prop, "Water");
	else if (mat.preset == 7)
		ft_strcpy(prop, "Chome");
	else if (mat.preset == 8)
		ft_strcpy(prop, "Air");
	else
		ft_strcpy(prop, "Custom");

	mlx_string_put(mlx_con, mlx_win, x_s - 8, y_s, color, prop);

	sprintf(prop, "%.2f", mat.amb);
	y_s = 227;
	mlx_string_put(mlx_con, mlx_win, x_s, y_s, color, prop);
	sprintf(prop, "%.2f", mat.diff);
	mlx_string_put(mlx_con, mlx_win, x_s, y_s += 25, color, prop);
	sprintf(prop, "%.2f", mat.spec);
	mlx_string_put(mlx_con, mlx_win, x_s, y_s += 25, color, prop);
	sprintf(prop, "%.2f", mat.shine);
	mlx_string_put(mlx_con, mlx_win, x_s, y_s += 25, color, prop);
	sprintf(prop, "%.2f", mat.ref);
	mlx_string_put(mlx_con, mlx_win, x_s, y_s += 25, color, prop);
	sprintf(prop, "%.2f", mat.transp);
	mlx_string_put(mlx_con, mlx_win, x_s, y_s += 25, color, prop);
	sprintf(prop, "%.2f", mat.refract);
	mlx_string_put(mlx_con, mlx_win, x_s, y_s += 25, color, prop);
}

void	set_shadow_bump(void *con, void *win, t_type type, t_trace *trace)
{
	bool shadow;
	bool bump;

	if (type == SPHERE)
	{
		shadow = trace->curr_sp->shadow;
		bump = trace->curr_sp->bump;
	}
	else if (type == PLANE)
	{
		shadow = trace->curr_pl->shadow;
		bump = trace->curr_pl->bump;
	}
	else if (type == CYLINDER)
	{
		shadow = trace->curr_cy->shadow;
		bump = trace->curr_cy->bump;
	}
	else if (type == CUBE)
	{
		shadow = trace->curr_cu->shadow;
		bump = trace->curr_cu->bump;
	}
	else if (type == HYPERBOLOID)
	{
		shadow = trace->curr_hy->shadow;
		bump = trace->curr_hy->bump;
	}
	else
		return ;
	if (shadow)
		mlx_string_put(con, win, 331, 407, 0x00FF00, "ON");
	else
		mlx_string_put(con, win, 329, 407, 0xFF0000, "OFF");
	if (bump)
		mlx_string_put(con, win, 331, 437, 0x00FF00, "ON");
	else
		mlx_string_put(con, win, 329, 437, 0xFF0000, "OFF");	
}

//sets the current vals on the control board

void	set_menu_vals(t_trace *trace, t_on *on)
{
	void	*con;
	void	*win;
	
	con = trace->mlx_connect;
	win = trace->mlx_win;
	set_type(con, win, on->type);
	set_color(con, win, on);
	set_props(con, win, on);
	set_shadow_bump(con, win, on->type, trace);
}