/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:22:19 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/22 14:53:40 by pbiederm         ###   ########.fr       */
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

//extern int	g_exit_status;
//extern bool bip;

typedef struct g_collection
{
	int		g_exit_status;
	bool	bip;
}	t_collect;

extern t_collect	g_errors;

typedef struct s_env
{
	char			*var;
	char			*var_name;
	char			*trim_var;
	struct s_env	*next;
}	t_env;

typedef struct s_redirection
{
	char	*name;
	int		type;
}	t_redir;

typedef struct s_chunk
{
	char			*command_path;
	char			**arguments;
	t_redir			*in_f;
	t_redir			*out_f;
	int				fd[2];
	int				indentifier;
	struct s_chunk	*prev;
	struct s_chunk	*next;
}	t_chunk;

typedef struct s_token
{
	char			*t;
	bool			s_quotes;
	bool			d_quotes;
	bool			ig;
	int				index;
	int				name;
	struct s_token	*next;
}	t_token;

typedef struct s_info
{
	char	*r;
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
	int		f;
}	t_info;

typedef struct s_struct_holder
{
	t_env	*env;
	t_env	*exp_l;
	t_token	*token;
	t_chunk	*c_arr;
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
	int		capture_exit_flag;
	int		capture_redirection_error;
	int		builtin_exit_fail;
}t_vars;

/*----	lexer_cases.c	-----------------*/
int		rest_of_the_cases(t_info *info, int i, t_token **token);
int		quotes(t_info *info, int i, t_token **token);
int		doubles(t_info *info, int i, t_token **token);

/*----	lexer.c	-----------------*/
int		ft_check_speacials(char *str, int i);
void	lexer(t_info *info, t_token **token);

/*----	check_pipes.c	-----------------*/
int		pipe_cases(t_info *info);
int		pipe_excistence(t_info *info);
int		check_for_spaces(char *str);

/*----	quotes.c	-----------------*/
int		check_quotes(t_info *info);
void	count_quotes(t_info *info);
void	connecting_quotes(t_token **token);
void	check_for_deleting_spaces(t_token **t);

/*----	quotes2.c	-----------------*/
int		remaking_node(t_token **t, t_token *delete, int ignore);
int		first_group(t_token **t, t_token *delete, int ignore);
int		full_qoute_empty_qoute(t_token **t, t_token *delete, int ignore);
int		full_quote_space_quote(t_token **t, t_token *delete, int ignore);
void	final_decision(t_token **t);

void	check_tokens(t_info *info, t_token **token);
int		space(t_info *info, int i, t_token **token);
int		space_norm(t_token **token, t_info *info, int i);
void	first_token(t_token **t);

/*----	freeing.c	----------------------*/
void	freeing_tokens(t_token *token);
void	freeing_chunks(t_chunk **chunk);
void	freeing_e_list(t_env **e_list);
void	free_splitted_path(char **splitted_path, int i);

int		parsing(t_info *info);
int		quotes_in_pipe(t_info *info, char quote, int position);
char	**bananasplit(t_info *info);
int		which_quote(const char *s1, int index);
int		if_quotes_closed(t_info *info, char quote);

/*----	utils.c	------------------------*/
int		ft_strcmp(const char *s1, const char *s2);
int		set_error_str(t_info *info, char *message, int error);
void	count_redirections(t_info *info);
void	check_dollar_signs(t_info *info);
int		skip_white_sp(char *s, int i);
int		skip_quotes(char *str, char quote, int i);
char	*ft_delete(char *str, char *part);

/*----	initialize.c	-------------------*/
void	initialize_info(t_info *info);
t_token	*initialize_token(t_token *token, t_info *info);
t_chunk	*initialize_chunk(t_chunk *chunk, t_info *info);
t_token	*attach_token_end(t_token *token, t_info *info);
t_chunk	*attach_chunk_end(t_chunk *chunk, t_info *info);

/*----	register_tokens.c	-------------*/
void	register_tokens(t_info *info, t_token **token, t_env *env);
int		space_check(t_token *token);
void	check_specials(t_token **token);

/*----	register_tokens2.c	-------------*/
void	register_redirection_name(t_token **token, int redirection);
void	register_next_token(t_token **token);
void	recognise_builtins(t_token **token);
void	recognise_commands(t_token **token);

/*----	register_tokens3.c	-------------*/
void	check_command_excists(t_token **token, t_env *env);
void	ignore(t_token **token);
int		lenght_picker(t_token *token);
int		change_the_node(t_token **token, t_token *temp, t_token *del, int ign);

/*----	parsing.c	-------------*/
void	get_the_commands(t_token *t, t_env *env, t_chunk **c, t_info *i);
t_token	*find_arguments(t_token *token, t_chunk **chunk);

/*----	parsing_utils.c	-------------*/
int		find_correct_path(char **splitted_path, char *s);
char	*path_checker(t_env *env);
int		filling_arguments(t_token *token, t_chunk **chunk, int i);
char	*find_command_path(char *s, t_env *env);
t_token	*choosing_block(t_token *token, t_chunk **chunk, t_env *env);

/*----	debugging.c	-------------*/
void	print_the_list(char *message, t_token *token);
void	print_the_chunk_list(char *message, t_chunk *chunk);
void	print_the_chunk_list_backwards(char *message, t_chunk *chunk);

/*----	input_output.c	-------------*/
void	input_first(int **fd, t_chunk	*salt, t_info *info, char	**envp);
void	output_first(int **fd, t_chunk	*salt, t_info *info, char	**envp);
void	input_output(int **fd, t_chunk	*salt, t_info *info, char	**envp);

/*----	expansions_utils.c	------------------*/
char	*ft_strtrim_f(char *s1, char *s2);
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
void	handle_child(int sig);

/*----	../builtins/builtins_utils.c	------------------*/
void	create_e_list(t_env **e_list, char **env);
int		valid_name(char *name);
t_env	*attach_end(t_env *token);
char	*save_name(char *str);

/*----	../builtins/cd.c & cd2.c	------------------*/
int		builtins_cd(char **line, t_env **e_list, t_env **exp_l);
char	*all_cases(t_env **e, char **line, int spec);
char	*get_home(t_env **e);
int		check_s_c(char **line);
char	*get_env_prev(t_env **e);

/*----	../builtins/env.c	------------------*/
int		builtins_env(char **arguments, t_env *e_list, t_vars *vars);

/*----	../builtins/pwd.c	------------------*/
int		ft_pwd(int fd);

/*----	../builtins/echo.c	------------------*/
int		builtins_echo(int fd, char **line, t_vars *vars);

/*----	../builtins/exit.c	------------------*/
//int		builtins_exit(t_env **exp_l, t_env **env_l, char **line);
int		builtins_exit(t_data *hive, char **line);

/*----	../builtins/export.c	------------------*/
int		builtins_export(t_env **exp_list, t_env **e_l, char **line, int fd);
void	print_export_l(t_env *ex_l, int fd);

/*----	../builtins/export2.c	------------------*/
int		possible_cases(char *arg);
int		name_exists(t_env *exp_list, char *str);
void	inject_to_exp_l(t_env **exp_l, char *str);
void	inject_to_e_l(t_env **env_l, char *str);
int		check_len(t_env *exl);

/*----	../builtins/export3.c	------------------*/
void	sort_indexes(t_env **exl);

/*----	../builtins/pwd.c	------------------*/
int		ft_pwd(int fd);
int		builtins_pwd(int fd, t_vars *vars);

/*----	../builtins/unset.c	------------------*/
int		builtins_unset(t_env **exp_l, t_env **env_l, char **line);
void	delete_env_node(t_env **e_l, char *str);

/*----	../src/new_fork.c	------------------*/
void	execute(t_chunk **salt, t_data *data, char **envp);

/*----	../parsing/errors.c	------------------*/
void	simple_err_message(t_info *info, char *message, int exit_status);
t_chunk	*error_initialise(t_info *info);
int		readline_err(t_data hive);
void	errors_before(t_info *info);

/*----	../src/here_doc.c	-------------*/
void	here_doc_handle(t_chunk **salt, t_vars *vars);
void	here_doc(char *delimit, t_vars *vars);

/*----	../src/run.c	-------------*/
void	run(t_chunk	*salt, char	**envp);

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
void	echo_handle(t_chunk	**salt, t_vars *vars);
void	cd_handle(t_chunk	**salt, t_env	*env, t_env	*exp_l);
void	pwd_handle(t_chunk	**salt, t_vars *vars);
void	env_handle(t_chunk **salt, t_env *e_list, t_vars *vars);
void	export_handle(t_env **exp_list, t_env **e_l, t_chunk **salt, int fd);
void	unset_handle(t_env **exp_l, t_env **env_l, t_chunk **salt, char **envp);
void	exit_handle(t_data *hive, t_chunk **salt);

/*----	../src/empty_data.c	-------------*/
void	empty_data_input(t_chunk	**salt, t_vars *vars);
void	empty_data_output(t_chunk **salt, t_vars *vars);

/*----	../src/pipeline_io.c	-------------*/
void	last_cmd_output(t_chunk	**salt, t_vars *vars, int i);
void	first_cmd_input(t_chunk **salt, int i);

/*----	../src/new_fork.c	-------------*/
void	built_in_handler(t_chunk **salt, t_data \
*data, t_vars *vars, char **envp);

void	redirect_in_conditions(t_chunk **salt, t_vars *vars);
void	pipe_error_execute(void);
void	get_exit_status(t_vars *vars, int status);
void	manage_fd(t_chunk **salt, t_vars *vars);
void	no_fork_handle(t_chunk **salt, t_data *data, t_vars *vars, char **envp);

/*----	../src/execution.c	-------------*/
void	pipe_fork(t_chunk **salt, t_data *data, char **envp, t_vars *vars);
void	fork_error(void);
void	command_error(t_chunk **salt, t_vars *vars);
void	restore_standard_io(t_vars *vars);
void	child_process(t_chunk **salt, t_vars *vars, char **envp);

/*----	../src/errors.c	-------------*/
void	pipe_error_execute(void);
void	redirection_out_error(t_vars *vars);
void	checker_before(t_data *hive);

/*----	../src/child_process_do.c	-------------*/
void	child_process_do(t_chunk **salt,
			t_data	*data, t_vars *vars, char **envp);

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
# define FORCED_R -7
# define AMPERSAND -6
# define DOUBLE_AMPERSAND -5
# define SEMICOLON -4
# define DOUBLE_SEMICOLON -3
# define L_BRACKET -2
//# define R_BRACKET -1

// USED
# define ELSE -9
# define EMPTY -1
# define SPC 0
# define PIPE 1
# define R_INPUT 2
# define R_AP_INPUT 3
# define R_OUTPUT 4
# define R_AP_OUTPUT 5
# define ESCAPE 6
# define COMMAND 7
# define BUILT_IN 8
# define ARGUMENT 9
# define FLAG 10
# define INPUT_F 11
# define DELIMITOR 12
# define OUTPUT_F 13
# define R_AP_OUTPUT_F 14

// USEFULL FOR THE EXECUTION

# define CMD_BLOCK 20
# define BUILT_IN_BLOCK 21
# define ELSE_BLOCK 22

# define OUTPUT 1
# define INPUT 0
# define WRITE 1
# define READ 0
# define TRUE 1
# define FALSE 0

#endif