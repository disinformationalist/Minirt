#include "minirt.h"
//---------------doubly circular linked list----------------


//----------------------------------------
//currently from push_swap
/* 

int	ft_size(t_stack *lst)
{
	int	count;

	count = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		count++;
	}
	return (count);
}

t_stack	*ft_last(t_stack *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	add_node(t_stack **stack, char **av, int *tab, long n)
{
	t_stack	*new;
	t_stack	*last;

	if (stack == NULL)
		return ;
	new = malloc(sizeof(t_stack));
	if (new == NULL)
		free_all(stack, av, tab);
	new->next = NULL;
	new->content = n;
	if (*stack == NULL)
	{
		*stack = new;
		new->prev = NULL;
	}
	else
	{
		last = ft_last(*stack);
		last->next = new;
		new->prev = last;
	}
}

void	free_stack(t_stack **stack)
{
	t_stack	*curr;
	t_stack	*temp;

	if (stack == NULL)
		return ;
	curr = *stack;
	while (curr)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
	*stack = NULL;
} */

