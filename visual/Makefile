NAME = vis

SRCS = main.c	   

FLAGS = -Wall -Werror -Wextra -I includes

OBJ = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	gcc -o $(NAME) $(FLAGS) $(OBJ) -L../elcapitan/ -lmlx -framework OpenGL -framework AppKit ../libft/libft.a
	echo "visual done"

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
