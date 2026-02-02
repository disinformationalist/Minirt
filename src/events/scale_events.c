#include "minirt.h"
#include "scale.h"

void 	set_scaknobs(t_trace *trace, t_control control)
{
	int				i, j;
	int				putxx, putxy;
	int				putyy, putyx;
	int				putzx, putzy;

	int				putxx2, putxy2;
	int				putyx2, putyy2;
	int				putzx2, putzy2;

	int				putxyzx, putxyzy;
	unsigned int	color;

	//c1
	putxx = control.knobs.scalex.posx;
	putxy = control.knobs.scalex.posy;

	putyx = control.knobs.scaley.posx;
	putyy = control.knobs.scaley.posy;
	
	putzx = control.knobs.scalez.posx;
	putzy = control.knobs.scalez.posy;

	//c2 xyz xz, yz, xy
	putxx2 = control.knobs.scalexz.posx;
	putxy2 = control.knobs.scalexz.posy;
	
	putyx2 = control.knobs.scaleyz.posx;
	putyy2 = control.knobs.scaleyz.posy;
	
	putzx2 = control.knobs.scalexy.posx;
	putzy2 = control.knobs.scalexy.posy;

	//c3
	putxyzx = control.knobs.scalexyz.posx;
	putxyzy = control.knobs.scalexyz.posy;


	j = -1;
	while(++j < control.k_height)
	{
		i = -1;
		while (++i < control.k_width)
		{
			color = pixel_color_get3(i, j, control.sca);
			if (color != 0xFF202020)
			{
				my_pixel_put(putxx + i, putxy + j, &trace->img, color);
				my_pixel_put(putyx + i, putyy + j, &trace->img, color);
				my_pixel_put(putzx + i, putzy + j, &trace->img, color);

				my_pixel_put(putxyzx + i, putxyzy + j, &trace->img, color);
			}
			color = pixel_color_get3(i, j, control.sil);
			if (color != 0xFF202020)
			{
				my_pixel_put(putxx2 + i, putxy2 + j, &trace->img, color);
				my_pixel_put(putyx2 + i, putyy2 + j, &trace->img, color);
				my_pixel_put(putzx2 + i, putzy2 + j, &trace->img, color);
			}
		}
	}
}

void	set_scapos(t_vec3 scale, t_vec3 scale_2, double scale_3, t_control *con)
{
	t_knobs knobs = con->knobs;

	//1st
	con->knobs.scalex.posx = ft_round(60 * cos(scale.x)) + knobs.scalex.cx;
	con->knobs.scalex.posy = knobs.scalex.cy - ft_round(60 * sin(scale.x));
	con->knobs.scaley.posx = ft_round(60 * cos(scale.y)) + knobs.scaley.cx;
	con->knobs.scaley.posy = knobs.scaley.cy - ft_round(60 * sin(scale.y));
	con->knobs.scalez.posx = ft_round(60 * cos(scale.z)) + knobs.scalez.cx;
	con->knobs.scalez.posy = knobs.scalez.cy - ft_round(60 * sin(scale.z));
	//2nd
	con->knobs.scalexz.posx = ft_round(90 * cos(scale_2.x)) + knobs.scalexz.cx;
	con->knobs.scalexz.posy = knobs.scalexz.cy - ft_round(90 * sin(scale_2.x));
	con->knobs.scaleyz.posx = ft_round(90 * cos(scale_2.y)) + knobs.scaleyz.cx;
	con->knobs.scaleyz.posy = knobs.scaleyz.cy - ft_round(90 * sin(scale_2.y));
	con->knobs.scalexy.posx = ft_round(90 * cos(scale_2.z)) + knobs.scalexy.cx;
	con->knobs.scalexy.posy = knobs.scalexy.cy - ft_round(90 * sin(scale_2.z));
	//3rd
	con->knobs.scalexyz.posx = ft_round(120 * cos(scale_3)) + knobs.scalexyz.cx;
	con->knobs.scalexyz.posy = knobs.scalexyz.cy - ft_round(120 * sin(scale_3));
}

void	set_sca(t_trace *trace)
{
	t_vec3		scale;
	t_vec3		init_scale;
	t_vec3		scale_2;
	double		scale_3;
	t_on		*on = trace->on;
	t_control	*con = trace->obj_control;

	scale = get_scale(on, trace);
	init_scale = get_init_scale(on, trace);
	scale = vec(1 / scale.x, 1 / scale.y, 1 / scale.z, 0);
	
	con->sca1 = scale;
	con->sca2 = init_scale;
	//divide out init
	scale = vec(scale.x / init_scale.x, scale.y / init_scale.y, scale.z / init_scale.z, 0);
	//xz, yz, xy, leave amount common to pair
	scale_2 = vec(fmin(scale.x, scale.z), fmin(scale.y, scale.z), fmin(scale.y, scale.x), 0);
	scale_3 = fmin(scale_2.x, scale.y);

	//scale = individual
	scale.x = convert_to_angle(scale.x, PI_SIXTHS);
	scale.y = convert_to_angle(scale.y, FIVE_PI_SIXTHS);
	scale.z = convert_to_angle(scale.z, -PI_HALVES);
	//scale_2 = pairs
	scale_2.x = convert_to_angle(scale_2.x, -PI_SIXTHS);
	scale_2.y = convert_to_angle(scale_2.y, -FIVE_PI_SIXTHS);
	scale_2.z = convert_to_angle(scale_2.z, PI_HALVES);
	//scale_3 = all
	if (scale_3 >= 1)
		scale_3 = (scale_3 - 1) / 3.006 - PI_HALVES;
	else
		scale_3 = -PI_HALVES - (1 - scale_3) * 3;

	con->knobs.scalex.angle = scale.x;
	con->knobs.scaley.angle = scale.y;
	con->knobs.scalez.angle = scale.z;

	con->knobs.scalexz.angle = scale_2.x;
	con->knobs.scaleyz.angle = scale_2.y;
	con->knobs.scalexy.angle = scale_2.z;

	con->knobs.scalexyz.angle = scale_3;

	set_scapos(scale, scale_2, scale_3, con);	
}

void	set_sca_dials(t_trace *trace)
{
	t_control		control;
	int				i;
	int				j;
	int				l;
	int				k;
	int				lstart;

	control = *trace->obj_control;
	j = 139;
	k = 0;
	lstart = -1;
	while (++j < control.m_height)
	{
		i = -1;
		l = lstart;
		while (++i < control.sd_width)
			my_pixel_put(++l, j, &trace->img, pixel_color_get3(i, k, control.sca_dials));
		k++;
	}
	set_sca(trace);
	set_scaknobs(trace, *trace->obj_control);
	if (trace->on && trace->on->type == HYPERBOLOID)
	{
		img_copyto(&trace->img, control.waist, 54, 522, control.w_width, control.w_height);
		set_waistknob(&trace->img, control, trace->on);
	}
}

	//1st circle
	/* con->sca1x.i = ft_round(60 * cos(scale.x));//rad*cos(angle)
	con->sca1x.j = ft_round(60 * sin(scale.x));
	con->sca1y.i = ft_round(60 * cos(scale.y));
	con->sca1y.j = ft_round(60 * sin(scale.y));
	con->sca1z.i = ft_round(60 * cos(scale.z));
	con->sca1z.j = ft_round(60 * sin(scale.z));
	//2nd
	con->sca2x.i = ft_round(90 * cos(scale_2.x));
	con->sca2x.j = ft_round(90 * sin(scale_2.x));
	con->sca2y.i = ft_round(90 * cos(scale_2.y));
	con->sca2y.j = ft_round(90 * sin(scale_2.y));
	con->sca2z.i = ft_round(90 * cos(scale_2.z));
	con->sca2z.j = ft_round(90 * sin(scale_2.z));
	//3rd
	con->scale_xyz.i = ft_round(120 * cos(scale_3));
	con->scale_xyz.j = ft_round(120 * sin(scale_3));
 */