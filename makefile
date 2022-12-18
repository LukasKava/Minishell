# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/10 16:25:07 by lkavalia          #+#    #+#              #
#    Updated: 2022/12/18 03:33:58 by lkavalia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

define IMG
            ,
         ,-`;
  ,      ; ;      ,
 ,  `'"""   """'`  ,
, ,-'````, ,````'-, ,
``    ,'`` ``',    ``
     ' ,`; ;`, '
     `,`,; ;,',`
     ,'` ,  ,`
-, '`,-'`; ;',`, ,
`,,'`    ; ;  `,`,`
         ; ;    `
         ;  '-
         ; '`
         ` `
        , '
   ,'`:, '
    , - `      KTJ
endef
export IMG

CC = gcc

#CFLAGS = -Wall -Werror -Wextra -fsanitize=address
CFLAGS = -Wall -Werror -Wextra

NAME = minishell

MAIN =	./src/parsing/main.c \
		./src/parsing/utils.c \
		./src/parsing/freeing.c \
		./src/parsing/initialize.c \
		./src/parsing/register_tokens.c\
		./src/parsing/register_tokens2.c\
		./src/parsing/register_tokens3.c\
		./src/parsing/lexer_cases.c\
		./src/parsing/check_tokens.c\
		./src/parsing/check_pipes.c \
		./src/parsing/quotes.c \
		./src/parsing/lexer.c \
		./src/parsing/parsing.c\
		./src/parsing/parsing_utils.c\
		./src/parsing/debugging.c\
		./src/parsing/expansions.c\
		./src/parsing/expansions_utils.c\
		./src/parsing/expansions_two.c\
		./src/parsing/final_check.c\
		./src/parsing/errors.c\
		./src/signals.c\
		./src/builtins/builtins_utils.c\
		./src/builtins/cd.c\
		./src/builtins/cd2.c\
		./src/builtins/echo.c\
		./src/builtins/env.c\
		./src/builtins/exit.c\
		./src/builtins/export.c\
		./src/builtins/export2.c\
		./src/builtins/export3.c\
		./src/builtins/pwd.c\
		./src/builtins/unset.c\
		./src/execute/here_doc.c\
		./src/execute/new_fork.c\
		./src/execute/run.c\
		./src/execute/initialize_variables.c\
		./src/execute/pipes.c\
		./src/execute/redirections.c\
		./src/execute/builtin_handles.c\
		./src/execute/empty_data.c

OBJS = ${SOURCES:.c=.o}

LIBFT = libft
LIBFT_NAME = libft.a

$(NAME)	:	$(SOURCES)
	@echo "$$IMG" 
	@make  -sC ./libft
	@echo "Compiling minishell..."
	@$(CC) $(CFLAGS) $(MAIN) $(SOURCES) -L$(LIBFT) -g -lft -lreadline -o $(NAME)

all: $(NAME)

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell

clean:
	@make clean -sC ./libft
	@echo "Removing object files..."
	@rm -f ${OBJS}

fclean:	clean
	@make fclean -sC ./libft
	@echo "Removing minishell..."
	@rm -f ${NAME}

re:	fclean all

memory:
	valgrind --leak-check=full --show-leak-kinds=all ./minishell

re_bonus:	fclean bonus

.PHONY:	clean fclean re re_bonus