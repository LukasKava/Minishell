CC = gcc
CFLAGS = -Wall -Werror -Wextra

NAME = minishell

MAIN =	./src/main.c \
		./src/utils.c \
		./src/freeing.c \
		./src/initialize.c \
		./src/register_tokens.c\
		./src/check_tokens.c\
		./src/check_pipes.c \
		./src/check_quotes.c \
		./src/lexer.c \


OBJS = ${SOURCES:.c=.o}

LIBFT = libft
LIBFT_NAME = libft.a

$(NAME)	:	$(SOURCES)
	@make  -sC ./libft
	@echo "Compiling minishell..."
	$(CC) $(CFLAGS) $(MAIN) $(SOURCES) -L$(LIBFT) -g -lft -lreadline -o $(NAME)

all: $(NAME)

clean:
	@make clean -sC ./libft
	@echo "Removing object files..."
	@rm -f ${OBJS}

fclean:	clean
	@make fclean -sC ./libft
	@echo "Removing minishell..."
	@rm -f ${NAME}

re:	fclean all

re_bonus:	fclean bonus

.PHONY:	clean fclean re re_bonus