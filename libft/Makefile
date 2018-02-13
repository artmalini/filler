# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/10/24 13:52:27 by amakhiny          #+#    #+#              #
#    Updated: 2017/10/24 13:53:44 by amakhiny         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
GC = gcc
CFLAGS = -c -Wall -Wextra -Werror -I libft.h
SOURCE = *.c
OBJ = $(SOURCE:.c=.o)

all: $(NAME)

$(NAME):
	$(GC) $(CFLAGS) $(SOURCE)
	ar rc $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
