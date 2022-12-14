# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rkaufman <rkaufman@student.42wolfsburg.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/01 21:47:32 by rkaufman          #+#    #+#              #
#    Updated: 2022/04/06 07:20:11 by rkaufman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	minishell_tester

CC			:=	gcc
HEADERFILE	:=	minishell_tester.h

SRC			:=	ft_tester.c \
				ft_get_next_line.c \
				ft_utils.c \
				ft_split.c \
				ft_threads.c \
				ft_list.c \
				ft_communication.c \
				ft_pipes.c \
				ft_children.c


OBJ			:=	$(SRC:%.c=%.o)

CFLAGS		:=	-Wall -Wextra -Werror -pthread -g #-fsanitize=thread

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

norm:
	norminette -R CheckForbiddenSourceHeader $(SRC) $(HEADERFILE)

.PHONY: clean fclean re