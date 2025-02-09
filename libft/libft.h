/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 09:18:40 by jhotchki          #+#    #+#             */
/*   Updated: 2023/09/18 21:56:34 by jhotchki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stddef.h>
# include <ctype.h>
# include <fcntl.h>

/***********LIBC FUNCTIONS************/
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
size_t	ft_strlen(const char *s);
void	*ft_memset(void *s, int c, size_t n);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
int		ft_atoi(const char *nptr);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);

/*********ADDITIONAL FUNCTIONS*********/

char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(const char *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
int		ft_putchar_fd(char c, int fd);
int		ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

/********MORE FUNCTIONS********/

void	ft_quicksort(int *tab, int start, int end);
void	ft_quicksort_params(char **tab, int start, int end);
long	ft_atol(const char *nptr);
double	ft_atof(char *s);
int		ft_putstr_color_fd(int fd, char *s, char *color);
char	*ft_strcpy(char *dest, const char *src);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strcat(char *dest, char *src);
size_t	ft_matrix_len(char **matrix);
void	ft_free_matrix_i(char **matrix, int i);

/********STRUCTURES**********/

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_stack
{
	void			*content;
	struct s_stack	*prev;
	struct s_stack	*next;
}					t_stack;

/**********BONUS FUNCTIONS***********/

t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
void	ft_rotate(t_stack **stack);
void	ft_reverse_rotate(t_stack **stack);
void	ft_push(t_stack **to, t_stack **from);
void	ft_swap_top2(t_stack **head);
t_stack	*ft_last(t_stack *lst);

/******GNL******/

size_t	ft_strlen_g(char *s);
char	*ft_strjoin_g(char *s1, char *s2);
char	*ft_strchr_g(char *s, int c);
char	*get_next_line(int fd);

#endif 
