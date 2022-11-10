/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:05:50 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/10 13:05:55 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * FUNCTION: (find_command_path) finds and validates the needed
 * 				command path it achieves it by going into environment
 * 				variables finding the one that starts with "PATH=". 
 * 				Processes it using  ft_split, ft_delete, ft_strjoin
 * 				and then checks if command excists and if it can
 * 				be executed using access.
 *	RETURN_VALUE:
 *	x 	correct path
 *	x 	NULL if the command was not found.
 */
static char	*find_command_path(char *s, char **envp)
{
	int		i;
	char	*path;
	char	**splitted_path;

	i = 0;
	while (envp[i] != ft_strnstr(envp[i], "PATH", 5))
		i++;
	path = envp[i];
	splitted_path = ft_split(path, ':');
	splitted_path[0] = ft_delete(splitted_path[0], "PATH=");
	i = 0;
	while (splitted_path[i] != NULL)
	{
		splitted_path[i] = ft_strjoin(splitted_path[i], "/");
		splitted_path[i] = ft_strjoin(splitted_path[i], s);
		if (access(splitted_path[i], F_OK) == 0 && \
							access(splitted_path[i], X_OK) == 0)
		{
			printf("command_path: %s\n", splitted_path[i]);
			path = splitted_path[i];
			break ;
		}
		free(splitted_path[i]);
		i++;
	}
	if (splitted_path[i] == NULL)
	{
		printf("ERROR (find_command_path): could not find the command!\n");
		return (NULL);
	}
	i++;
	while (splitted_path[i] != NULL)
	{
		free(splitted_path[i]);
		i++;
	}
	free(splitted_path);
	return (path);
}

/**
 * FUNCTION: (find_arguments) takes care of finding and registering command and
 * 				its arguments. The function can be devided into 3 parts:
 * 	1. Checking how many spots we will need for our 2d array.
 * 	2. Giving a chunck->arguments that space.
 * 	3. filling up the available spots with command and arguments.
 */
static void	find_arguments(t_token *token, t_chunk **chunk)
{
	int		i;
	int		length;
	t_token	*temp;

	i = 0;
	length = 0;
	temp = token;
	while (token != NULL && (token->name < PIPE || token->name > R_AP_OUTPUT))
	{
		if (token->name >= COMMAND && token->name <= FLAG)
			length++;
		token = token->next;
	}
	(*chunk)->arguments = ft_calloc(length + 1, sizeof(char *));
	(*chunk)->arguments[length] = NULL;
	token = temp;
	while (token != NULL && (token->name < PIPE || token->name > R_AP_OUTPUT))
	{
		if (token->name >= COMMAND && token->name <= FLAG)
		{
			(*chunk)->arguments[i] = token->token;
			i++;
		}
		token = token->next;
	}
}

/**
 * FUNCTION: (register_chunk) alocates the space for the argument and updates
 * 				the chunk name accordingly.
 */
static void	register_chunk(t_token *token, t_chunk **chunk, int indentifier, t_info *info)
{
	(*chunk)->arguments = ft_calloc(2, sizeof(char *));
	if ((*chunk)->arguments == NULL)
	{
		printf("ERROR (get_the_commands): calloc has failed!\n");
		info->error = true;
	}
	(*chunk)->arguments[0] = token->token;
	(*chunk)->arguments[1] = NULL;
	(*chunk)->indentifier = indentifier;
	if (token->next != NULL && (token->next->name < PIPE || token->next->name > R_AP_OUTPUT))
		(*chunk) = attach_chunk_end(*chunk);
}

/**
 * FUNCTION: (get_the_commands) It processes the before excisting information of
 * 				lexing and registering tokens parts and makes it useable for the 
 * 				execution.
 * 	MAIN_PARTS:
 * 	x	if conditions to recognise the parts.
 * 	x	function register_chunk which is needed to indentify the chunk itself.
 * 	x	if condidtion for checking metacharacters and creating tokens accordingly.
 */
void	get_the_commands(t_info *info, t_token *token, char **envp, t_chunk **chunk)
{
	t_chunk *temp;

	temp = (*chunk);
	while (token != NULL)
	{
		if (token->name == INPUT_F)
			register_chunk(token, chunk, INPUT_F, info);
		else if (token->name == OUTPUT_F)
			register_chunk(token, chunk, OUTPUT_F, info);
		else if (token->name == R_AP_OUTPUT_F)
			register_chunk(token, chunk, R_AP_OUTPUT_F, info);
		else if (token->name == DELIMITOR)
			register_chunk(token, chunk, DELIMITOR, info);
		else if (token->name == COMMAND || token->name == BUILT_IN)
		{
			printf("command: %s\n", token->token);
			if (token->name == COMMAND)
				(*chunk)->command_path = find_command_path(token->token, envp);
			find_arguments(token, chunk);
			if (token->name == COMMAND)
				(*chunk)->indentifier = CMD_BLOCK;
			else
				(*chunk)->indentifier = BUILT_IN_BLOCK;
		}
		if (token->index != 0 && (token->name >= PIPE && token->name <= R_AP_OUTPUT))
			(*chunk) = attach_chunk_end(*chunk);
		token = token->next;
	}
	(*chunk)->next = NULL;
	(*chunk) = temp;
}
