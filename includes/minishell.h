/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:22:19 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/01 16:25:40 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <string.h>
# include "../libft/libft.h"
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <sys/errno.h>
# include <signal.h>
# include "./get_next_line.h"
# include <linux/limits.h>
# include <errno.h>
# include <stdint.h>

extern int	g_exit_status;

typedef struct s_env
{
	char	*var;
	char	*var_name;
	struct	s_env *next;
}	t_env;

typedef struct s_redirection
{
	char	*name;
	int		type;
} t_redir;

typedef struct s_chunk
{
	char	*command_path;
	char	**arguments;
	t_redir	*in_f;
	t_redir	*out_f;
	int		fd_in;
	int		fd_out;
	int		indentifier;
	struct	s_chunk *prev;
	struct	s_chunk	*next;
}	t_chunk;

typedef struct s_token
{
	char		*token;
	bool		single_quotes;
	bool		double_quotes;
	bool		ignore;
	int			index;
	int 		name;
	struct 		s_token *next;
}	t_token;

typedef struct s_info
{
	char	*readline;
	int		s_quotes;
	int		d_quotes;
	int		redirect_input;
	int		redirect_output;
	int		redirect_input_append;
	int		redirect_output_append;
	int		dollar_sign;
	int		dollar_question_sign;
	int		pipes;
	bool	error;
	int		trigger;
	int		start;
}	t_info;

typedef struct	s_struct_holder
{
	t_env	*env;
	t_env	*exp_l;
	t_token *token;
	t_chunk *c_arr;
	t_info	info;
}	t_data;

/*----	lexer_cases.c	-----------------*/
int		rest_of_the_cases(t_info *info, int i, t_token **token);
int		quotes(t_info *info, int i, t_token **token);
int 	doubles(t_info *info, int i, t_token **token);

/*----	lexer.c	-----------------*/
int		ft_check_speacials(char *str, int i);
void 	lexer(t_info *info, t_token **token);

/*----	check_pipes.c	-----------------*/
int		pipe_cases(t_info *info);
int		pipe_excistence(t_info *info);

/*----	quotes.c	-----------------*/
void	check_quote_type(t_token **token, char c);
int		check_quotes(t_info *info);
void	count_quotes(t_info *info);
void	connecting_quotes(t_token **token);

void	check_tokens(t_info *info, t_token **token);
int 	space(t_info *info, int i, t_token **token);

/*----	freeing.c	----------------------*/
void	freeing_tokens(t_token *token);
void	freeing_chunks(t_chunk **chunk, t_info *info);

int		parsing(t_info *info);
int 	quotes_in_pipe(t_info *info, char quote, int position); 
char 	**bananasplit(t_info *info);
int 	which_quote(const char *s1, int index);
int 	if_quotes_closed(t_info *info, char quote);

/*----	utils.c	------------------------*/
int		ft_strcmp(const char *s1, const char *s2);
int 	set_error_str(t_info *info, char *message, int error);
void	count_redirections(t_info *info);
void	check_dollar_signs(t_info *info);
int		skip_white_sp(char *s, int i);
int 	skip_quotes(char *str, char quote, int i);

/*----	initialize.c	-------------------*/
void	initialize_info(t_info *info);
t_token *initialize_token(t_token *token, t_info *info);
t_chunk *initialize_chunk(t_chunk *chunk, t_info *info);
t_token *attach_token_end(t_token *token);
t_chunk *attach_chunk_end(t_chunk *chunk);

/*----	register_tokens.c	-------------*/
void	register_tokens(t_info *info, t_token **token, t_env *env);
char	*ft_delete(char *str, char *part);
void recognise_builtins(t_token **token);

/*----	parsing.c	-------------*/
void	get_the_commands(t_info *info, t_token *token, t_env *env, t_chunk **chunk);

/*----	debugging.c	-------------*/
void	print_the_list(char *message, t_token *token);
void	print_the_chunk_list(char *message, t_chunk *chunk);
void	print_the_chunk_list_backwards(char *message, t_chunk *chunk);

/*----	expansions.c	------------------*/
void	expand_expansions(t_token **token, t_env *env);

/*----	fork.c	-------------*/
void	second_child(t_chunk **salt, t_info *info, char **envp);
void	free_fd(int **fd);

/*----	here_doc.c	-------------*/
void	here_doc(t_chunk	**salt, t_info *info, char	**envp);
void	here_doc_run(t_chunk	*salt, t_info *info, char	**envp);
void	here_doc_multi(t_chunk	**salt);
int		find_delim(t_chunk ** salt);

/*----	run.c	-------------*/
void	run(t_chunk *salt, t_info *info, char **envp);

/*----	input_output.c	-------------*/
void	input_first(int **fd, t_chunk	*salt, t_info *info, char	**envp);
void	output_first(int **fd, t_chunk	*salt, t_info *info, char	**envp);
void	input_output(int **fd, t_chunk	*salt, t_info *info, char	**envp);

/*----	cleaner.c	-------------*/
void	free_fd(int **fd);

/*----	roles.c	-------------*/
void	roles_expanded(int **fd, t_chunk	*salt, t_info *info, char	**envp);

/*----	expansions_utils.c	------------------*/
size_t	ft_case(char c);
char	*ft_strtrim_beginning(char *s1, char *s2);
void	ft_cut_exp(t_token **token);
char	*save_the_front(char *token);
char	*save_the_tail(char *token, char *current);

/*----	expansions_utils.c	------------------*/
size_t	exp_count(char *str);
int		find_expansion(char *str);
char	*save_var(char *token);
size_t	env_var_excists(char *str, t_env *env);

/*----	../signals.c	------------------*/
void	handle_sigint(int sig);
void	handle_sigint_child(int num);

/*----	../builtins/builtins_utils.c	------------------*/
void	create_e_list(t_env **e_list, char **env);
void	freeing_e_list(t_env **e_list);
int		valid_name(char *name);
t_env	*attach_end(t_env *token);
char	*save_name(char *str);

/*----	../builtins/cd.c	------------------*/
int builtins_cd(char **line, t_env **e_list);

/*----	../builtins/env.c	------------------*/
int		builtins_env(char **arguments, t_env *e_list);

/*----	../builtins/pwd.c	------------------*/
int		ft_pwd(int fd);

/*----	../builtins/echo.c	------------------*/
int		builtins_echo(int fd, char **line);

/*----	../builtins/exit.c	------------------*/
int		builtins_exit(t_env **exp_l, t_env **env_l, char **line);

/*----	../builtins/export.c	------------------*/
int 	builtins_export(t_env **exp_list, t_env **e_l, char **line, int fd);
void	print_export_l(t_env *ex_l, int fd);

/*----	../builtins/pwd.c	------------------*/
int ft_pwd(int fd);

/*----	../builtins/unset.c	------------------*/
int		builtins_unset(t_env **exp_l, t_env **env_l, char **line);

//INDENTIFIER EXPLANATION:
/**
 *	x everything else = -9
 *	x forced redirection ('>|') = -7	//Cast an error because its not manditory.
 *	x ampersand ('&') = -6				//Cast an error because its not manditory
 *	x double_ampersand ('&&') = -5		//Cast an error because its not manditory
 *	x semicolon	(';') = -4				//Cast an error because its not manditory
 *	x double_semicolon (';;') = -3		//Cast an error because its not manditory
 *	x left_bracket ('(') = -2			//Cast an error because its not manditory
 *	x right_bracket (')') = -1			//Cast an error because its not manditory
 *	|space = 0
 *	|pipe = 1
 *	|redirection_input = 2
 *	|redirection_output = 3
 *	|redirect_append_input = 4
 *	|redirect_append_output = 5
 *	|escape_char '\' == 6
 *	command = 7
 *	built-in = 8
 *	arguments = 9
 *	flags = 10
 *	input_file = 11
 *	output_file = 12
 *	delimitor = 13
 */

// indentifiers:
//  command block == 20
//  Built-in block == 21
//  input file == 11
//	output file == 12
//  delimitor == 13

// NOT USED
#define FORCED_R -7
#define AMPERSAND -6
#define DOUBLE_AMPERSAND -5
#define SEMICOLON -4
#define DOUBLE_SEMICOLON -3
#define L_BRACKET -2
#define R_BRACKET -1

// USED
#define ELSE -9
#define SPace 0
#define PIPE 1
#define R_INPUT 2
#define R_AP_INPUT 3
#define R_OUTPUT 4
#define R_AP_OUTPUT 5
#define ESCAPE 6
#define COMMAND 7
#define BUILT_IN 8
#define ARGUMENT 9
#define FLAG 10
#define INPUT_F 11
#define DELIMITOR 12
#define OUTPUT_F 13
#define	R_AP_OUTPUT_F 14

// USEFULL FOR THE EXECUTION

#define CMD_BLOCK 20
#define BUILT_IN_BLOCK 21

#define	OUTPUT 1
#define INPUT 0
#define	WRITE 1
#define READ 0
#define TRUE 1
#define FALSE 0
// INPUT_F the same
// OUTPUT_F the same
// DELIMITOR the same

#endif