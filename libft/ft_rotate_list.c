#include "libft.h"

void	ft_rotate(t_stack **stack)
{
	t_stack	*last;

	if (*stack == NULL || (*stack)->next == NULL)
		return ;
	last = ft_last(*stack);
	last->next = *stack;
	*stack = (*stack)->next;
	(*stack)->prev = NULL;
	last->next->prev = last;
	last->next->next = NULL;
}
