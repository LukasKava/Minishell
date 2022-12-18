/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 23:45:04 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 19:55:58 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*path_checker(t_env *env)
{
	char	*path;

	path = NULL;
	if (env == NULL)
	{
		printf("\033[0;31mERROR in parsing.c PATH is not excistent!\033[0m\n");
		return (NULL);
	}
	else
		path = env->var;
	return (path);
}

int	find_correct_path(char **splitted_path, char *s)
{
	int	i;

	i = 0;
	while (splitted_path[i] != NULL)
	{
		splitted_path[i] = ft_strjoin(splitted_path[i], "/");
		splitted_path[i] = ft_strjoin(splitted_path[i], s);
		if (access(splitted_path[i], F_OK) == 0 && \
			access(splitted_path[i], X_OK) == 0)
			break ;
		free(splitted_path[i]);
		i++;
	}
	return (i);
}

int	filling_arguments(t_token *token, t_chunk **chunk, int i)
{
	if ((token->name >= COMMAND && token->name <= FLAG) || \
		(token->name == EMPTY && (token->single_quotes == 1 || \
		token->double_quotes == 1)) || (token->name == SPC && \
		(token->single_quotes == 1 || token->double_quotes == 1)))
	{
		(*chunk)->arguments[i] = token->token;
		i++;
	}
	return (i);
}

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
char	*find_command_path(char *s, t_env *env)
{
	int		i;
	char	*path;
	char	**splitted_path;

	i = 0;
	while (env != NULL && env->var != ft_strnstr(env->var, "PATH", 5))
		env = env->next;
	path = path_checker(env);
	if (path == NULL)
		return (NULL);
	splitted_path = ft_split(path, ':');
	splitted_path[0] = ft_delete(splitted_path[0], "PATH=");
	i = find_correct_path(splitted_path, s);
	path = splitted_path[i];
	if (splitted_path[i] == NULL)
	{
		free(splitted_path);
		return (NULL);
	}
	free_splitted_path(splitted_path, i + 1);
	return (path);
}

t_token	*choosing_block(t_token *token, t_chunk **chunk, t_env *env)
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
	return (token);
}
