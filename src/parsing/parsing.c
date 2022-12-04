/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:05:50 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/03 15:12:21 by lkavalia         ###   ########.fr       */
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
static char	*find_command_path(char *s, t_env *env)
{
	int		i;
	char	*path;
	char	**splitted_path;

	i = 0;
	path = NULL;
	while (env != NULL && env->var != ft_strnstr(env->var, "PATH", 5))
		env = env->next;
	if (env ==  NULL)
	{
		printf("ERRROR  ind parsing.c PATH is not excistent!\n");
		g_exit_status = 127;
		return (NULL);
	}
	else
		path = env->var;
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
			path = splitted_path[i];
			break ;
		}
		free(splitted_path[i]);
		i++;
	}
	if (splitted_path[i] == NULL)
	{
		printf("ERROR (find_command_path): could not find the command!\n");
		g_exit_status = 127;
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
	while (token != NULL && token->name != PIPE)
	{
		if (token->name >= COMMAND && token->name <= FLAG)
			length++;
		token = token->next;
	}
	(*chunk)->arguments = ft_calloc(length + 1, sizeof(char *));
	(*chunk)->arguments[length] = NULL;
	token = temp;
	while (token != NULL && token->name != PIPE)
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


static	void save_the_files(int amount, int id, t_token *token, t_redir **red)
{
	int	i;

	i = 0;
	(*red) = ft_calloc(amount + 1, sizeof(t_redir));
	(*red)[amount].name = NULL;
	while (token != NULL && token->name != PIPE && amount >= 0)
	{
		if ((token->name == id || token->name == id + 1)  && (token->next != NULL && (token->next->name == id + 9 || token->next->name == id + 10)))
		{
			token = token->next;
			(*red)[i].name = token->token;
			(*red)[i].type = token->name;
			i++;
			amount--; 
		}
		token = token->next;
	}
	i = 0;
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
}

static void	register_the_redirections(t_token *token, t_chunk **chunk)
{
	int	registered_input;
	int	registered_output;

	registered_input = 0;
	registered_output = 0;
	while (token != NULL && token->name != PIPE)
	{
		printf("checking: [%s]\n", token->token);
		if (registered_input != 1 && (token->name == R_INPUT || token->name == R_AP_INPUT) && (token))
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
}

void get_the_commands(t_token *token, t_env *env, t_chunk **chunk)
{
	t_chunk *temp;

	temp = (*chunk);
	while (token != NULL)
	{
		register_the_redirections(token, chunk);
		while (token != NULL && token->name != PIPE)
		{
			if (token->name == COMMAND)
			{
				(*chunk)->indentifier = CMD_BLOCK;
				(*chunk)->command_path = find_command_path(token->token, env);
				token = find_arguments(token, chunk);
			}
			else if (token->name == BUILT_IN)
			{
				(*chunk)->indentifier = BUILT_IN_BLOCK;
				token = find_arguments(token, chunk);
			}
			if (token != NULL && token->name != PIPE)
				token = token->next;
		}
		if (token != NULL && token->index != 0 && token->name == PIPE)
			(*chunk) = attach_chunk_end(*chunk);
		if (token != NULL)
			token = token->next;
	}
	(*chunk)->next = NULL;
	(*chunk) = temp;
}
