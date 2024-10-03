#include "libft.h"

size_t	ft_matrix_len(char **matrix)
{
	size_t	i;

	i = 0;
	while (matrix[i])
		i++;
	return (i);
}
