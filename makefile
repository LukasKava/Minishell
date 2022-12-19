# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/10 16:25:07 by lkavalia          #+#    #+#              #
#    Updated: 2022/12/19 17:13:35 by pbiederm         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

define IMG
 .              +   .                .   . .     .  .
                   .                    .       .     *
  .       *                        . . . .  .   .  + .
            "You Are Here"            .   .  +  . . .
.                 |             .  .   .    .    . .
                  |           .     .     . +.    +  .
                 \|/            .       .   . .
        . .       V          .    * . . .  .  +   .
           +      .           .   .      +
                            .       . +  .+. .
  .                      .     . + .  . .     .      .
           .      .    .     . .   . . .        ! /
      *             .    . .  +    .  .       - O -
          .     .    .  +   . .  *  .       . / |
               . + .  .  .  .. +  .
.      .  .  .  *   .  *  . +..  .            *
 .      .   . .   .   .   . .  +   .    .            +
endef
export IMG

CC = cc

CFLAGS = -Wall -Werror -Wextra -fsanitize=address
#CFLAGS = -Wall -Werror -Wextra

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
		./src/parsing/expansions.c\
		./src/parsing/expansions_utils.c\
		./src/parsing/expansions_two.c\
		./src/parsing/final_check.c\
		./src/parsing/errors.c\
		./src/parsing/debugging.c\
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
		./src/execute/empty_data.c\
		./src/execute/builtin_handles_II.c\
		./src/execute/execution.c\
		./src/execute/errors.c\
		./src/execute/redirections_II.c\
		./src/execute/child_process_do.c

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

unsetall:
	unset SHELL SESSION_MANAGER QT_ACCESSIBILITY COLORTERM XDG_CONFIG_DIRS SSH_AGENT_LAUNCHER XDG_MENU_PREFIX TERM_PROGRAM_VERSION GNOME_DESKTOP_SESSION_ID LC_ADDRESS GNOME_SHELL_SESSION_MODE DOTNET_ROOT LC_NAME SSH_AUTH_SOCK XMODIFIERS DESKTOP_SESSION LC_MONETARY GTK_MODULES PWD LOGNAME XDG_SESSION_DESKTOP XDG_SESSION_TYPE SYSTEMD_EXEC_PID XAUTHORITY VSCODE_GIT_ASKPASS_NODE IM_CONFIG_CHECK_ENV GJS_DEBUG_TOPICS HOME USERNAME IM_CONFIG_PHASE LANG LC_PAPER LS_COLORS XDG_CURRENT_DESKTOP WAYLAND_DISPLAY GIT_ASKPASS INVOCATION_ID MANAGERPID DOTNET_BUNDLE_EXTRACT_BASE_DIR CHROME_DESKTOP GJS_DEBUG_OUTPUT VSCODE_GIT_ASKPASS_EXTRA_ARGS GNOME_SETUP_DISPLAY LESSCLOSE XDG_SESSION_CLASS LC_IDENTIFICATION TERM LESSOPEN USER VSCODE_GIT_IPC_HANDLE DISPLAY SHLVL LC_TELEPHONE QT_IM_MODULE LC_MEASUREMENT XDG_RUNTIME_DIR LC_TIME VSCODE_GIT_ASKPASS_MAIN JOURNAL_STREAM XDG_DATA_DIRS GDK_BACKEND PATH GDMSESSION ORIGINAL_XDG_CURRENT_DESKTOP DBUS_SESSION_BUS_ADDRESS GIO_LAUNCHED_DESKTOP_FILE_PID GIO_LAUNCHED_DESKTOP_FILE LC_NUMERIC TERM_PROGRAM _

.PHONY:	clean fclean re re_bonus