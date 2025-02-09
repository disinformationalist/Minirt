#include "minirt.h"

t_norm_color	get_obj_color2(t_on *on)
{
	if (on->object == NULL)
		return (color(0, 0, 0));
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
	else
		return ("");
}

char	*copy_till(char *s1, char *s2, char c)
{
	int i = -1;

	while (s1[++i] != c && i < 9)
			s2[i] = s1[i];
	s2[i] = '\0';
	return (s2);
}

void	set_color(void *mlx_con, void *mlx_win, t_on *on)
{
	t_norm_color	color;
	unsigned int	hex_col;
	char 			r[10];
	char 			g[10];
	char 			b[10];
	char			hex[10];
	int				y_s;
	int				opt;
	unsigned int	color1;

	if (on->object == NULL)
		return ;
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
	{
		mlx_string_put(mlx_con, mlx_win, 70, 58, color, "TO CAMERA");
		mlx_string_put(mlx_con, mlx_win, 182, 58, color, "SPHERE");
		mlx_string_put(mlx_con, mlx_win, 278, 58, color, "TO PLANE");
	}
	else if (type == PLANE)
	{
		mlx_string_put(mlx_con, mlx_win, 182 - 112, 58, color, "TO SPHERE");
		mlx_string_put(mlx_con, mlx_win, 185, 58, color, "PLANE");
		mlx_string_put(mlx_con, mlx_win, 269, 58, color, "TO CYLINDER");
	}
	else if (type == CYLINDER)
	{
		mlx_string_put(mlx_con, mlx_win, 72, 58, color, "TO PLANE");
		mlx_string_put(mlx_con, mlx_win, 176, 58, color, "CYLINDER");
		mlx_string_put(mlx_con, mlx_win, 276, 58, color, "TO HYPER");
	}
	else if (type == HYPERBOLOID)
	{
		mlx_string_put(mlx_con, mlx_win, 65, 58, color, "TO CYLINDER");
		mlx_string_put(mlx_con, mlx_win, 168, 58, color, "HYPERBOLOID");
		mlx_string_put(mlx_con, mlx_win, 280, 58, color, "TO CUBE");
	}
	else if (type == CUBE)
	{
		mlx_string_put(mlx_con, mlx_win, 72, 58, color, "TO HYPER");
		mlx_string_put(mlx_con, mlx_win, 187, 58, color, "CUBE");
		mlx_string_put(mlx_con, mlx_win, 278, 58, color, "TO LIGHT");
	}
	else if (type == LIGHT)
	{		
		mlx_string_put(mlx_con, mlx_win, 76, 58, color, "TO CUBE");
		mlx_string_put(mlx_con, mlx_win, 184, 58, color, "LIGHT");
		mlx_string_put(mlx_con, mlx_win, 274, 58, color, "TO CAMERA");
	}
	else if (type == CAM)
	{
		mlx_string_put(mlx_con, mlx_win, 74, 58, color, "TO LIGHT");
		mlx_string_put(mlx_con, mlx_win, 182, 58, color, "CAMERA");
		mlx_string_put(mlx_con, mlx_win, 274, 58, color, "TO SPHERE");
	}
	else if (type == VOID)
	{
		mlx_string_put(mlx_con, mlx_win, 70, 58, color, "TO CAMERA");
		mlx_string_put(mlx_con, mlx_win, 187, 58, color, "VOID");
		mlx_string_put(mlx_con, mlx_win, 274, 58, color, "TO SPHERE");
	}
}

t_mat	get_obj_mat(t_on *on)
{
	if (on->object == NULL)
		return ((t_mat){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 9});
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
		ft_strcpy(prop, "Enamel");
	else if (mat.preset == 7)
		ft_strcpy(prop, "Water");
	else if (mat.preset == 8)
		ft_strcpy(prop, "Chome");
	else if (mat.preset == 9)
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
	bool 	shadow;
	bool 	bump;

	if (trace->on->object == NULL)
		return ;
	if (type == SPHERE)
	{
		t_sphere sp = *trace->curr_sp;
		shadow = sp.shadow;
		bump = sp.bump;
	}
	else if (type == PLANE)
	{
		t_plane pl = *trace->curr_pl;
		shadow = pl.shadow;
		bump = pl.bump;
	}
	else if (type == CYLINDER)
	{
		t_cylinder cy = *trace->curr_cy;
		shadow = cy.shadow;
		bump = cy.bump;
	}
	else if (type == CUBE)
	{
		t_cube cu = *trace->curr_cu;
		shadow = trace->curr_cu->shadow;
		bump = cu.bump;
	}
	else if (type == HYPERBOLOID)
	{
		t_hyperboloid hy = *trace->curr_hy;
		shadow = hy.shadow;
		bump = hy.bump;
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

void	put_bump_level(void *con, void *win, t_trace *trace)
{
	double 			level;
	unsigned int	color;
	char			bump[10];

	color = 0x90C4FF;	
	level = get_bump_level(trace->on);
	sprintf(bump, "%.2f", level);
	mlx_string_put(con, win, 324, 467, color, bump);
}

//sets the current vals on the control board

void	set_menu_vals(t_trace *trace, t_on *on)
{
	void			*con;
	void			*win;
	char			str[10];

	if (on->type == LIGHT)
	{
		set_light_vals(trace, on);
		return ;
	}
	con = trace->mlx_connect;
	win = trace->mlx_win;
	set_type(con, win, on->type);
	if (!on->object)
		return ;
	if (on->type == CAM)
	{
		double fov = ((t_cam *)(on->object))->fov;
		sprintf(str, "%.2f", fov);
		mlx_string_put(con, win, 332, 87, 0x90C4FF, str);
		return ;
	}
	set_color(con, win, on);
	set_props(con, win, on);
	set_shadow_bump(con, win, on->type, trace);
	put_bump_level(con, win, trace);

}