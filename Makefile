# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/01/27 16:51:29 by amakhiny          #+#    #+#              #
#    Updated: 2018/01/27 16:51:36 by amakhiny         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME  = amakhiny.filler
INC	  = -I ./libft -I ./includes
FLAGS = -Wall -Wextra -Werror
LIBFT = libft/libft.a

MAIN_FILE  = main.c
#FILL_FILES = ft_filler.c	

RED		= \033[31m
GREEN	= \033[32m
CYAN	= \033[36m
GREY	= \033[37m

MAIN_O = $(MAIN_FILE:.c=.o)
#FILL_O = $(FILL_FILEs:.c=.o)

LIBFT_A = -L libft -lft

all: $(NAME)

$(NAME): $(MAIN_O)
	@echo " $(GREEN)Compiling....."
	@make -o1 -C libft
	@gcc -o $(NAME) $(FLAGS) $(INC) $(MAIN_O) $(LIBFT_A)
	#@echo " $(CYAN)$(NAME) $(GREY)generated \xF0\x9F\x98\xBA"

%.o: %.c
	@gcc $(FLAGS) $(INC) -c -o $@ $<

clean:
	@/bin/rm -f $(OBJ)
	@make -C libft clean
	@echo " $(RED)All objects has been deleteted!"

fclean: clean
	@/bin/rm -f $(NAME)
	@make -C libft fclean
	@echo " $(RED)$(NAME)$(GREY) has been deleteted!"

re: fclean all

.PHONY: all clean fclean re
