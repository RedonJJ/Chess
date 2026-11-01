NAME    = chess

SRC     = src/main.c \
               src/plateau.c \
		       src/saisie.c \
			   src/pieces.c

OBJ     = $(SRC:.c=.o)

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -I include

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re