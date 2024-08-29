#include "libft.h"

void	ft_swap_top2(t_stack **head)
{
	t_stack	*n_1;
	t_stack	*n_2;

	if (!(*head) || !((*head)->next))
		return ;
	n_1 = *head;
	n_2 = (*head)->next;
	n_1->next = n_2->next;
	if (n_2->next != NULL)
		n_2->next->prev = n_1;
	n_2->prev = NULL;
	n_2->next = n_1;
	n_1->prev = n_2;
	*head = n_2;
}
