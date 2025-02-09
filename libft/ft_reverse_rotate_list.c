#include "libft.h"

void	ft_reverse_rotate(t_stack **stack)
{
	t_stack	*last;

	if (!(*stack) || !((*stack)->next))
		return ;
	last = ft_last(*stack);
	last->prev->next = NULL;
	last->next = *stack;
	last->prev = NULL;
	*stack = last;
	last->next->prev = last;
}
