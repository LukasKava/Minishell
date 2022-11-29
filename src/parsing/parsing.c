/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:05:50 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/29 13:54:57 by lkavalia         ###   ########.fr       */
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
static t_token	*find_arguments(t_token *token, t_chunk **chunk)
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
	return (token);
}

/**
 * FUNCTION: (register_chunk) alocates the space for the argument and updates
 * 				the chunk name accordingly.
 */
// static void	register_chunk(t_token *token, t_chunk **chunk, int indentifier, t_info *info)
// {
// 	(*chunk)->arguments = ft_calloc(2, sizeof(char *));
// 	if ((*chunk)->arguments == NULL)
// 	{
// 		printf("ERROR (get_the_commands): calloc has failed!\n");
// 		info->error = true;
// 	}
// 	(*chunk)->arguments[0] = token->token;
// 	(*chunk)->arguments[1] = NULL;
// 	(*chunk)->indentifier = indentifier;
// 	if (token->next != NULL && (token->next->name < PIPE || token->next->name > R_AP_OUTPUT))
// 		(*chunk) = attach_chunk_end(*chunk);
// }


static	void save_the_files(int amount, int id, t_token *token, t_redir **red)
{
	int	i;

	i = 0;
	printf("amount: %d\n", amount);
	(*red) = ft_calloc(amount + 1, sizeof(t_redir));
	(*red)[amount].name = NULL;
	while (token != NULL && token->name != PIPE && amount >= 0)
	{
		if ((token->name == id || token->name == id + 1)  && (token->next != NULL && (token->next->name == id + 9 || token->next->name == id + 10)))
		{
			printf("indentifier: %d\n", token->name);
			token = token->next;
			(*red)[i].name = token->token;
			(*red)[i].type = token->name;
			i++;
			amount--; 
		}
		token = token->next;
	}
	i = 0;
	printf("INFILE NAMES: ");
	while ((*red)[i].name != NULL)
	{
		printf("[%s]: %d ", (*red)[i].name, (*red)[i].type);
		i++;
	}
	printf("\n\n");
}

static	void count_inputs(t_token *token, t_chunk **chunk, int id)
{
	t_token	*temp;
	int		count;
	int		ap_count;

	temp = token;
	count = 0;
	ap_count = 0;
	if (id == R_AP_INPUT || id == R_AP_OUTPUT)
		id--;
	while (token != NULL && token->name != PIPE)
	{
		if (token->name == id)
			count++;
		if (token->name == id + 1)
			ap_count++;
		token = token->next;
	}
	token = temp;
	if ((count + ap_count) != 0 && id == R_INPUT)
		save_the_files(count + ap_count, id, token, &(*chunk)->in_f);
	else if ((count + ap_count) != 0 && id == R_OUTPUT)
		save_the_files(count + ap_count, id, token, &(*chunk)->out_f);
	printf("input_count: %d\n", count);
	printf("input_ap_count: %d\n", ap_count);
}

static t_token	*register_the_redirections(t_token *token, t_chunk **chunk)
{
	int	registered_input;
	int	registered_output;

	registered_input = 0;
	registered_output = 0;
	while (token != NULL && token->name != PIPE)
	{
		if (registered_input != 1 && (token->name == R_INPUT || token->name == R_AP_INPUT))
		{
			registered_input = 1;
			printf("inside register redirections!\n");
			count_inputs(token, chunk, token->name);
		}
		else if (registered_output != 1 && (token->name == R_OUTPUT || token->name == R_AP_OUTPUT))
		{
			registered_output = 1;
			printf("inside register redirections 2!\n");
			count_inputs(token, chunk, token->name);
		}
		token = token->next;
	}
	return (token);
}

void get_the_commands(t_info *info, t_token *token, char **envp, t_chunk **chunk)
{
	t_chunk *temp;
	int		command;

	temp = (*chunk);
	command = 0;
	while (token != NULL)
	{
		if (token->name == COMMAND || token->name == BUILT_IN)
		{
			printf("command: %s\n", token->token);
			command = token->name;
			if (command == COMMAND)
				(*chunk)->command_path = find_command_path(token->token, envp);
			if (command == COMMAND)
				(*chunk)->indentifier = CMD_BLOCK;
			else
				(*chunk)->indentifier = BUILT_IN_BLOCK;
			token = find_arguments(token, chunk);
			if (token != NULL && (token->name > PIPE && token->name <= R_AP_OUTPUT))
			{
				printf("inside register the redirections!: %s\n", token->token);
				token = register_the_redirections(token, chunk);
			}
		}
		if (token != NULL && token->index != 0 && token->name == PIPE)
			(*chunk) = attach_chunk_end(*chunk);
		if (token != NULL)
			token = token->next;
	}
	(*chunk)->next = NULL;
	(*chunk) = temp;
	printf("info: %d\n", info->dollar_sign);
}