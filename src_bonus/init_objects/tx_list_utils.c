#include "minirt.h"

void	null_init(t_tx *new)
{
	new->i_name = NULL;
	new->m_name = NULL;
	new->image = NULL;
	new->bump_map = NULL;
}

t_tx	*create_tx(char **line)
{
	t_tx	*new;

	new = (t_tx *)malloc(sizeof(t_tx));
	if (!new)
		return (NULL);
	null_init(new);
	new->i_name = ft_strdup(line[1]);
	if (!new->i_name)
		return (free(new), NULL);
	if (line[2])
	{
		new->m_name = strdup(line[2]);
		if (!new->m_name)
		{
			free(new->i_name);
			free(new);
			return (NULL);
		}
	}
	new->next = new;
	new->prev = new;
	return (new);
}

bool	append_tx(t_tx **start, char **line)
{
	t_tx	*new;
	t_tx	*last;

	new = create_tx(line);
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
