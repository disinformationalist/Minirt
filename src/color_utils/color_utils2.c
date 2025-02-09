#include "minirt.h"

t_norm_color	*set_metal_colors(void)
{
	t_norm_color	*m_colors;

	m_colors = (t_norm_color *)malloc(sizeof(t_norm_color) * 19);
	if (!m_colors)
		return (NULL);
	m_colors[0] = (t_norm_color){1.0, 1.0, 1.0};
	m_colors[1] = (t_norm_color){0.75, 0.75, 0.75};
	m_colors[2] = (t_norm_color){1.0, 0.843, 0.0};
	m_colors[3] = (t_norm_color){0.72, 0.45, 0.2};
	m_colors[4] = (t_norm_color){0.854, 0.647, 0.125};
	m_colors[5] = (t_norm_color){0.65, 0.16, 0.16};
	m_colors[6] = (t_norm_color){0.83, 0.686, 0.22};
	m_colors[7] = (t_norm_color){0.5, 0.5, 0.5};
	m_colors[8] = (t_norm_color){0.69, 0.77, 0.87};
	m_colors[9] = (t_norm_color){0.55, 0.27, 0.07};
	m_colors[10] = (t_norm_color){0.66, 0.66, 0.66};
	m_colors[11] = (t_norm_color){0.41, 0.41, 0.41};
	m_colors[12] = (t_norm_color){0.275, 0.51, 0.71};
	m_colors[13] = (t_norm_color){1.0, 0.855, 0.725};
	m_colors[14] = (t_norm_color){0.44, 0.5, 0.56};
	m_colors[15] = (t_norm_color){0.898, 0.894, 0.886};
	m_colors[16] = (t_norm_color){0.737, 0.56, 0.56};
	m_colors[17] = (t_norm_color){1.0, 0.98, 0.94};
	m_colors[18] = (t_norm_color){0.1, 0.1, 0.1};
	return (m_colors);
}

double	clamp(double val, double bot, double top)
{
	if (val < bot)
		val = bot;
	if (val > top)
		val = top;
	return (val);
}

/* combines color components, balances reflect and refract
current in use, 0 - 255 object color, 0 - 1 light colors */

t_norm_color	get_final_color4(t_trace *trace, t_comps comps, \
	t_norm_color lt_color)
{
	t_norm_color	color_out;
	t_mat			m;
	double			r;
	double			r2;

	m = comps.mat;
	color_out.r = comps.color.r * (lt_color.r + m.amb * trace->amb->color.r);
	color_out.g = comps.color.g * (lt_color.g + m.amb * trace->amb->color.g);
	color_out.b = comps.color.b * (lt_color.b + m.amb * trace->amb->color.b);
	if (m.ref > 0 && m.transp > 0)
	{
		r = schlick(comps) * m.ref;
		r2 = (1.0 - r) * m.transp;
		color_out.r += r * comps.refl_col.r + r2 * comps.refr_col.r;
		color_out.g += r * comps.refl_col.g + r2 * comps.refr_col.g;
		color_out.b += r * comps.refl_col.b + r2 * comps.refr_col.b;
	}
	else
	{
		color_out.r += m.ref * comps.refl_col.r + m.transp * comps.refr_col.r;
		color_out.g += m.ref * comps.refl_col.g + m.transp * comps.refr_col.g;
		color_out.b += m.ref * comps.refl_col.b + m.transp * comps.refr_col.b;
	}
	return (color_out);
}
