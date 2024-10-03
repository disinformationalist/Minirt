# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/06 18:12:19 by jhotchki          #+#    #+#              #
#    Updated: 2023/10/21 11:43:33 by jhotchki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := libftprintf.a

SRCS := ft_printf.c ft_print_chars.c ft_print_ptr.c ft_print_nbrs.c ft_printf_utils.c \
ft_get_width.c ft_print_hex.c ft_error.c

CFLAGS := -Wall -Wextra -Werror

CC := cc

OBS := $(SRCS:.c=.o)

.SILENT:

all: $(NAME)

bonus: all

$(NAME): $(OBS)
		ar rcs $(NAME) $(OBS)

clean:
		rm -rf $(OBS) 

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
