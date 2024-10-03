#include "libft.h"

void	ft_push(t_stack **to, t_stack **from)
{
	t_stack	*node;

	if (*from == NULL)
		return ;
	node = *from;
	*from = node->next;
	if (*from)
		(*from)->prev = NULL;
	if (*to == NULL)
	{
		*to = node;
		(*to)->next = NULL;
	}
	else
	{
		(*to)->prev = node;
		node->next = *to;
		*to = node;
	}
}
