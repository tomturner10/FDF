
NAME = fdf

SRCS = main.c \
		utils/ft_split.c \
		utils/get_next_line_utils.c \
		utils/get_next_line.c

FLAGS = -Werror -Wextra -Wall

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -Lmlx -lmlx -Imlx -Ilst -Iutils -framework OpenGL -framework AppKit $(SRCS) -o $(NAME)
	
clean:
	rm -f $(NAME)

fclean:
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re