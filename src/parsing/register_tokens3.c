/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   register_tokens3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 03:02:57 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/21 03:09:42 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	lenght_picker(t_token *t)
{
	if (t->name >= COMMAND && t->name <= FLAG)
		return (0);
	if (t->name == SPC && (t->s_quotes == true || t->d_quotes == true))
		return (0);
	if (t->name == EMPTY && t->s_quotes == true && t->d_quotes == true)
	{
		if (t->next != NULL)
		{
			if (t->next->name != PIPE)
				return (0);
		}
	}
	if (t->name == EMPTY && (t->s_quotes == true || t->d_quotes == true))
	{
		if (t->next != NULL)
		{
			if (t->next->name == SPC && \
			(t->next->s_quotes == false && t->next->d_quotes == false))
				return (0);
		}
	}
	if (t->ignore == true && t->next->ignore == true)
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
		if ((*token)->ignore == true && \
			((*token)->name != SPC && (*token)->name != EMPTY))
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

int	change_the_node(t_token **token, t_token *temp, t_token *del, int ign)
{
	free((*token)->t);
	(*token)->t = ft_strdup((*token)->next->t);
	(*token)->ignore = (*token)->next->ignore;
	(*token)->d_quotes = (*token)->next->d_quotes;
	(*token)->s_quotes = (*token)->next->s_quotes;
	(*token)->name = (*token)->next->name;
	del = (*token)->next;
	(*token)->next = (*token)->next->next;
	free(del->t);
	free(del);
	(*token) = temp;
	ign = 1;
	return (ign);
}
