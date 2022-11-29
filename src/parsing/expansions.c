/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 15:32:15 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/17 03:50:12 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static	t_token	*get_rid_of_garbage(t_token **token, t_token *temp)
{
	t_token	*temp_token;

	temp_token = temp;
	if ((*token)->double_quotes == true)
	{
		ft_cut_exp(token);
		temp_token = (*token);
	}
	else
	{
		while (temp_token->next->index != (*token)->index)
			temp_token = temp_token->next;
		free((*token)->token);
		temp_token->next = (*token)->next;
		free(*token);
	}
	return (temp_token);
}

static void	combine_everything(char *tail, char *middle, t_token **token)
{
	char	*front;

	front = save_the_front((*token)->token);
	free((*token)->token);
	(*token)->token = ft_strjoin(front, middle);
	(*token)->token = ft_strjoin((*token)->token, tail);
}

static void	expand_env(t_token **token, char **envp)
{
	char	*env_var;
	char	*end_var;
	char	*tail;
	int		i;

	i = 0;
	env_var = save_var((*token)->token);
	tail = save_the_tail((*token)->token, env_var);
	while (envp[i] != NULL \
			&& envp[i] != ft_strnstr(envp[i], env_var, ft_strlen(env_var)))
		i++;
	if (envp[i] != NULL)
	{
		env_var = ft_strjoin(env_var, "=");
		end_var = ft_strtrim_beginning(envp[i], env_var);
		combine_everything(tail, end_var, token);
	}
	free(tail);
	free(env_var);
	free(end_var);
}

static void	taking_care_of_block(t_token **token, t_token *temp, char **envp)
{
	int	env_count;	
	int	env_excistence;

	env_excistence = 0;
	env_count = exp_count((*token)->token);
	while (env_count != 0)
	{
		env_excistence = find_expansion((*token)->token);
		if (env_excistence == 0)
		{
			if (env_var_excists((*token)->token, envp) == 0)
				expand_env(token, envp);
			else
				(*token) = get_rid_of_garbage(token, temp);
		}
		if (env_excistence == 2)
			(*token) = get_rid_of_garbage(token, temp);
		env_count--;
	}
}

//Legal rules of naming Variables in bash:
//	x	The variable name must be in the upper case as it is considered
//			good practise in bash scripting.
//	x	Insert the dolar sign "$" before the var name.
//	x	Do not use spaces after the initialiazation of the variable name
//			and its value.
//	x	A variable name can have letter/s.
//	X	A variable name can have numbers, underscores and digits.
void	expand_expansions(t_token **token, char **envp)
{
	t_token	*temp;

	temp = (*token);
	while ((*token) != NULL)
	{
		if ((*token)->single_quotes == false)
			taking_care_of_block(token, temp, envp);
		(*token) = (*token)->next;
	}
	(*token) = temp;
}
