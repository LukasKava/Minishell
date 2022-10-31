/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:22:19 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 12:22:38 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include "../libft/libft.h"
# include <stdbool.h>

//	pipe = 1
//	file_name = 2
//	command = 3
//	redirection_input = 4
//	redirection_output = 5
//	redirect_append_input = 6
//	redirect_append_output = 7
//	text = 8
//	space = 0

// typedef struct s_chunk
// {
// 	char	*command_path;
// 	char	**arguments;
// 	int		indentifier;
// }	t_token;

typedef struct s_token
{
	char		*token;
//	int			pipe;
//	int			single_quotes;
//	int			double_quotes;
	bool		single_quotes;
	bool		double_quotes;
	bool		ignore;
	int			index;
	int 		indentifier;
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
	int		exit_status;
}	t_info;


void	lexer(t_info *info, t_token **token);
int pipe_excistence(t_info *info);
void check_tokens(t_info *info, t_token **token);
int	check_quotes(t_info *info);

/*----freeing.c----------------------*/
void freeing_tokens(t_token *token);

int		parsing(t_info *info);
int		check_pipes(t_info *info);
int 	pipe_cases(t_info *info);
int 	quotes_in_pipe(t_info *info, char quote, int position); 
char 	**bananasplit(t_info *info);
int 	which_quote(const char *s1, int index);
int 	if_quotes_closed(t_info *info, char quote);

/*----utils.c------------------------*/
void	count_quotes(t_info *info);
int		ft_strcmp(const char *s1, const char *s2);
int 	set_error_str(t_info *info, char *message, int error);
void	count_redirections(t_info *info);
void	check_dollar_signs(t_info *info);
int		skip_white_sp(char *s, int i);

/*----initialize.c-------------------*/
void	initialize_info(t_info *info);

/*----register_tokens.c-------------*/
void	register_tokens(t_info *info, t_token **token, char **envp);

#endif