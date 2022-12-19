/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   register_tokens3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 03:02:57 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/19 14:06:56 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	lenght_picker(t_token *token)
{
	if (token->name >= COMMAND && token->name <= FLAG)
		return (0);
	if ((token->name == EMPTY || token->name == SPC) && \
		(token->s_quotes == false && token->d_quotes == false))
		return (0);
	return (1);
}

static	int	looping_through_tokens(t_token **token, char *path, t_token *temp)
{
	int		i;
	char	**split_path;

	i = 0;
	split_path = NULL;
	while ((*token) != NULL)
	{
		if ((*token)->name == COMMAND)
		{
			split_path = ft_split(path, ':');
			split_path[0] = ft_delete(split_path[0], "PATH=");
			i = find_correct_path(split_path, (*token)->t);
			if (split_path[i] == NULL)
			{
				free_splitted_path(split_path, i);
				(*token) = temp;
				return (1);
			}
			free_splitted_path(split_path, i);
		}
		(*token) = (*token)->next;
	}
	return (0);
}

void	check_command_excists(t_token **token, t_env *env)
{
	char	*path;
	t_token	*temp;

	path = NULL;
	while (env != NULL && env->var != NULL && \
			env->var != ft_strnstr(env->var, "PATH", 5))
		env = env->next;
	if (env == NULL || env->var == NULL)
		return ;
	else
		path = env->var;
	temp = (*token);
	if (looping_through_tokens(token, path, temp) == 1)
		return ;
	(*token) = temp;
}

/**
 * FUNCTION: (ignore)  finds already preset ignore trigger that is true
 * 				and sets the following token ignore triggers to true until
 * 				it finds one of the metacharacters. Ex. Pipe.
 */
void	ignore(t_token **token)
{
	t_token	*temp;

	temp = (*token);
	while ((*token) != NULL)
	{
		if ((*token)->ignore == true)
		{
			while ((*token) != NULL && ((*token)->name < PIPE || \
										(*token)->name > R_AP_OUTPUT))
			{
				(*token)->ignore = true;
				(*token) = (*token)->next;
			}
		}
		if ((*token) != NULL)
			(*token) = (*token)->next;
	}
	(*token) = temp;
}
