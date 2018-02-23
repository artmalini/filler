NAME = vis

SRCS = main.c	   

FLAGS = -Wall -Werror -Wextra -I includes

OBJ = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	gcc -o $(NAME) $(FLAGS) $(OBJ) -L../elcapitan/ -lmlx -framework OpenGL -framework AppKit ../libft/libft.a

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
