/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:05:50 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/03 21:09:59 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//	x everything else = -9
//	x forced redirection ('>|') = -7
//	x ampersand ('&') = -6
//	x double_ampersand ('&&') = -5
//	x semicolon	(';') = -4
//	x double_semicolon (';;') = -3
//	x left_bracket ('(') = -2
//	x right_bracket (')') = -1
//	|space = 0
//	|pipe = 1
//	|redirection_input = 2
//	|redirection_output = 3
//	|redirect_append_input = 4
//	|redirect_append_output = 5
//	|escape_char '\' == 6
//	command = 7
//	built-in = 8
//	arguments = 9
//	flags = 10
//	input_file = 11
//	output_file = 12
//	delimitor = 13

static	char *find_command_path(char *s, char **envp)
{
	int		i;
	char	*path;
	char	**splitted_path;

	i = 0;
	while (envp[i] != ft_strnstr(envp[i], "PATH", 5))
		i++;
	path = envp[i];
	splitted_path = ft_split(path, ':');
	splitted_path[0] = ft_strtrim(splitted_path[0], "PATH=");
	i = 0;
	while (splitted_path[i] != NULL)
	{
		splitted_path[i] = ft_strjoin(splitted_path[i], "/");
		splitted_path[i] = ft_strjoin(splitted_path[i], s);
		if (access(splitted_path[i], F_OK) == 0)
		{
			if (access(splitted_path[i], X_OK) == 0)
			{
				printf("command_path: %s\n", splitted_path[i]);
				return (splitted_path[i]);
			}
		}
		i++;
	}
	printf("ERROR (find_command_path): could not find the command!\n");
	return (NULL);
}

static void	find_arguments(t_token *token, t_chunk **chunk)
{
	int	i;
	int	length;
	t_token *temp;

	i = 0;
	length = 0;
	temp = token;
	while (token != NULL && (token->indentifier < 1 || token->indentifier > 5))
	{
		if (token->indentifier >= 7 && token->indentifier <= 10)
			length++;
		token = token->next;
	}
	(*chunk)->arguments = ft_calloc(length + 1, sizeof(char *));
	(*chunk)->arguments[length] = NULL;
	token = temp;
	while (token != NULL && (token->indentifier < 1 || token->indentifier > 5))
	{
		if (token->indentifier >= 7 && token->indentifier <= 10)
		{
			(*chunk)->arguments[i] = token->token;
			i++;
		}
		token = token->next;
	}
	token = temp;
}

/**
 * FUNCTION: (register_chunk) 
 * 
 * @param token 
 * @param chunk 
 * @param indentifier 
 */
static void	register_chunk(t_token *token, t_chunk **chunk, int indentifier)
{
	(*chunk)->arguments = ft_calloc(2, sizeof(char *));
	if ((*chunk)->arguments == NULL)
		printf("ERROR (get_the_commands): calloc has failed!\n");
	(*chunk)->arguments[0] = token->token;
	(*chunk)->arguments[1] = NULL;
	(*chunk)->indentifier = indentifier;
	if (token->next != NULL && (token->next->indentifier < PIPE || token->next->indentifier > R_AP_OUTPUT))
	{
		(*chunk) = attach_chunk_end(*chunk);
		(*chunk) = (*chunk)->next;
	}
}


void	get_the_commands(t_info *info, t_token *token, char **envp, t_chunk **chunk)
{
//	int	lenght;
	t_chunk *temp;

//	lenght = 0;
	temp = (*chunk);
	while (token != NULL)
	{
		if (token->indentifier == 11)
			register_chunk(token, chunk, 3);
		else if (token->indentifier == 12)
			register_chunk(token, chunk, 4);
		else if (token->indentifier == 13)
			register_chunk(token, chunk, 5);
		else if (token->indentifier == 7)
		{
			printf("command: %s\n", token->token);
			(*chunk)->command_path = find_command_path(token->token, envp);
			find_arguments(token, chunk);
			(*chunk)->indentifier = 1;
		}
		if (token->indentifier >= 1 && token->indentifier <= 5)
		{
			(*chunk) = attach_chunk_end(*chunk);
			(*chunk) = (*chunk)->next;
		}
		token = token->next;
	}
	(*chunk) = attach_chunk_end(*chunk);
	(*chunk)->next = NULL;
	(*chunk) = temp;
//	printf("command-path: %s\n", (*chunk)->command_path);
	printf("info->info: %d\n", info->d_quotes);
}