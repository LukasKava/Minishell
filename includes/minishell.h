/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:22:19 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/12 15:46:58 by lkavalia         ###   ########.fr       */
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
	int		fd[2];
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

typedef struct s_vars
{
	int		number_of_infiles;
	int		number_of_outfiles;
	int		num_cmd;
	int		input_fd;
	int		output_fd;
	int		pid;
	int		pipe_group;
	int		save_stdout;
	int		save_stdin;
}t_vars;

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
void	freeing_chunks(t_chunk **chunk);

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
t_token *attach_token_end(t_token *token, t_info *info);
t_chunk *attach_chunk_end(t_chunk *chunk, t_info *info);

/*----	register_tokens.c	-------------*/
void	register_tokens(t_info *info, t_token **token, t_env *env);
char	*ft_delete(char *str, char *part);
void	recognise_builtins(t_token **token);

/*----	parsing.c	-------------*/
void	get_the_commands(t_token *token, t_env *env, t_chunk **chunk, t_info *info);

/*----	debugging.c	-------------*/
void	print_the_list(char *message, t_token *token);
void	print_the_chunk_list(char *message, t_chunk *chunk);
void	print_the_chunk_list_backwards(char *message, t_chunk *chunk);

/*----	input_output.c	-------------*/
void	input_first(int **fd, t_chunk	*salt, t_info *info, char	**envp);
void	output_first(int **fd, t_chunk	*salt, t_info *info, char	**envp);
void	input_output(int **fd, t_chunk	*salt, t_info *info, char	**envp);

/*----	expansions_utils.c	------------------*/
char	*ft_strtrim_beginning(char *s1, char *s2);
char	*save_the_front(char *token);
char	*save_tail(char *str);
int		confirm_expansion(t_token *token);

/*----	expansions_two.c	------------------*/
size_t	exp_count(char *str);
size_t	en_excists(char *str, t_env *env);
char	*save_ex_var(char *token);
char	*return_ex_value(char *var, t_env *env);

/*----	expansions.c	------------------*/
void	expand_expansions(t_token **token, t_env *env);

/*----	final_check.c	------------------*/
void	check_for_executables(t_chunk **chunk);

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
int		builtins_cd(char **line, t_env **e_list);

/*----	../builtins/env.c	------------------*/
int		builtins_env(char **arguments, t_env *e_list);

/*----	../builtins/pwd.c	------------------*/
int		ft_pwd(int fd);

/*----	../builtins/echo.c	------------------*/
int		builtins_echo(int fd, char **line);

/*----	../builtins/exit.c	------------------*/
//int		builtins_exit(t_env **exp_l, t_env **env_l, char **line);
int		builtins_exit(t_data *hive, char **line);

/*----	../builtins/export.c	------------------*/
int		builtins_export(t_env **exp_list, t_env **e_l, char **line, int fd);
void	print_export_l(t_env *ex_l, int fd);

/*----	../builtins/pwd.c	------------------*/
int 	ft_pwd(int fd);
int		builtins_pwd(int fd);

/*----	../builtins/unset.c	------------------*/
int		builtins_unset(t_env **exp_l, t_env **env_l, char **line);

/*----	../src/new_fork.c	------------------*/
void	execute(t_chunk **salt, t_data *data, char	**envp);

/*----	../parsing/errors.c	------------------*/
void	simple_err_message(t_info *info, char *message, int exit_status);

/*----	../src/here_doc.c	-------------*/
int		here_doc(char	*delimit);

/*----	../src/run.c	-------------*/
void	run(t_chunk *salt,char **envp);

/*----	../src/initalise_variables.c	-------------*/
t_vars	*initialize_vars(t_chunk **salt);
int		count_command_number(t_chunk **salt);

/*----	../src/pipes.c	-------------*/
int		pipe_this_node(t_chunk **salt);
int		pipe_last_node(t_chunk **salt);
void	set_pipe_io(t_chunk **salt, t_vars *vars);

/*----	../src/redirections.c	-------------*/
void	redirect_io(t_chunk **salt, t_vars *vars);
int		in_redirection_this_node(t_chunk **salt);
int		out_redirection_this_node(t_chunk **salt);
void	redirect_out(t_chunk **salt, t_vars *vars);
int		in_redirection_next_node(t_chunk **salt);
void	redirect_in(t_chunk **salt, t_vars *vars);

/*----	../src/builtin_handles.c	-------------*/
void	echo_handle(t_chunk	**salt);
void	cd_handle(t_chunk	**salt, t_env	*env);
void	pwd_handle(t_chunk	**salt);
void	env_handle(t_chunk **salt, t_env *e_list);
void	export_handle(t_env **exp_list, t_env **e_l, t_chunk **salt, int fd);
void	unset_handle(t_env **exp_l, t_env **env_l, t_chunk	**salt);
void	exit_handle(t_data *hive, t_chunk **salt);

/*----	../src/empty_data.c	-------------*/
void	empty_data_input(t_chunk	**salt, t_vars *vars);
void	empty_data_output(t_chunk **salt, t_vars *vars);

/*----	../src/pipeline_io.c	-------------*/
void	last_cmd_output(t_chunk	**salt, t_vars *vars, int i);
void	first_cmd_input(t_chunk **salt, int i);

void	built_in_handler(t_chunk **salt, t_data *data);
void	redirect_in_conditions(t_chunk **salt, t_vars *vars);

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
#define ELSE_BLOCK 22

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