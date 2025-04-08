#include "minirt.h"


static inline void	set_sp_box(t_sphere *new)
{
	t_mat4	transform;

	new->center = vec(0.0, 0.0, 0.0, 1.0);
	new->radius = 500;
	new->color = color(124, 162, 205);
	transform = inv_scaling(new->radius, \
			new->radius, new->radius);
	new->curr_scale = transform;
	new->t_transform = transpose(transform);
	new->i_transform = inverse(transform);
	new->curr_rottran = identity();
	new->transform = transform;
	new->mat = get_mat(DEFAULT);
	new->id = 1;
	new->shadow = false;
	new->bump = false;
	new->next = new;
	new->prev = new;
	new->option = 0;
	new->w_frost = false;
	new->is_box = true;
	new->pattern = uv_checker(20, 10, color(40, 40, 40), color(255, 255, 255));//adjust
	new->bump_level = DEFAULT_BUMP;
	new->fuzz_lev = new->bump_level / 1000;
	new->rots = vec(0, 0, 0, 0);
}

static inline t_sphere	*create_sp_box(void)
{
	t_sphere	*new;

	new = (t_sphere *)malloc(sizeof(t_sphere));
	if (!new)
		return (NULL);
	set_sp_box(new);
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_sp_box(t_trace *trace, t_sphere **start)
{
	t_sphere	*new;
	t_sphere	*last;
	bool		temp;

	if (!trace->spheres)
		temp = false;
	else
		temp = true;
	//printf("temp: %d\n", temp);
	
	//trace->on->object = trace->spheres;
	//trace->on->type = SPHERE;
	trace->total_ints += 2;
	new = create_sp_box();
	if (!new)
		return (true);
	if (*start == NULL)
	{
		*start = new;
		new->id = 1;
		if (!temp)
			trace->curr_sp = trace->spheres;
		new->texture = trace->textures;
		return (false);
	}
	last = (*start)->prev;
	new->id = last->id + 1;
	new->next = *start;
	(*start)->prev = new;
	new->prev = last;
	last->next = new;
	(*start) = new;
	if (!temp)
		trace->curr_sp = trace->spheres;
	new->texture = trace->textures;
	return (false);
}


void	set_sp_vals(t_sphere *new, char **line)
{
	char	*diam_str;

	diam_str = line[2];
	new->center = get_coordinates(line[1], 1.0);
	new->radius = get_double(&diam_str) / 2.0;
	 
	new->mat = get_mat(ENAMEL);
	//new->mat = get_mat(DEFAULT);
	//new->mat = get_mat(MIRROR);
	//new->mat = get_mat(GLASS);
	//new->mat = get_mat(METAL);
	new->color = color(220, 155, 43);//pure gold used in spflake
	//new->color = color(220, 130, 35); //gold brown used in spflake
	//new->color = color(255, 255, 255);
	//new->mat.spec = 0;
	//new->shadow = false;
	//new->color = get_color(line[3], 1.0);
	new->shadow = true;
	new->bump = false;
	new->option = 0;
	new->w_frost = false;
	new->texture = NULL;
	new->is_box = false;
	new->bump_level = DEFAULT_BUMP;
	new->fuzz_lev = new->bump_level / 1000;
}

t_sphere	*create_sphere(char **line)
{
	t_sphere	*new;

	new = (t_sphere *)malloc(sizeof(t_sphere));
	if (!new)
		return (NULL);
	set_sp_vals(new, line);
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_sp(t_sphere **start, char **line)
{
	t_sphere	*new;
	t_sphere	*last;

	new = create_sphere(line);
	if (!new)
		return (true);
	if (*start == NULL)
	{
		*start = new;
		new->id = 1;
		return (false);
	}
	last = (*start)->prev;
	new->id = last->id + 1;
	new->next = *start;
	(*start)->prev = new;
	new->prev = last;
	last->next = new;
	return (false);
}
