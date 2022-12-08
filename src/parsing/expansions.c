/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 15:32:15 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/06 14:45:55 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void combine_everything(t_token **token, char *var, t_env *env)
{
	char	*beginning;
	char	*tail;
	
	beginning = save_the_front((*token)->token);
	tail = save_tail((*token)->token);
	var = return_ex_value(var, env);
	free((*token)->token);
	(*token)->token = ft_strjoin(beginning, var);
	(*token)->token = ft_strjoin((*token)->token, tail);
	free(var);
	free(tail);
}

static void	cut_bad_fruit(t_token **token)
{
	char *beginning;
	char *tail;
	
	beginning = save_the_front((*token)->token);
	tail = save_tail((*token)->token);
	free((*token)->token);
	(*token)->token = ft_strjoin(beginning, tail);
	free(tail);
}

static void	take_care_of_block(t_token **token, t_env *env)
{
	int	exp_c;
	char *var;

	exp_c = exp_count((*token)->token);
	var = NULL;
	while (exp_c != 0)
	{
		if (confirm_expansion((*token)) == 0)
		{
			var = save_ex_var((*token)->token);
			if (en_excists(var, env) == 0)
				combine_everything(token, var, env);
			else
			{
				cut_bad_fruit(token);
				free(var);
			}
			exp_c--;
		}
	}
}

static void	exit_status(t_token **token)
{
	if ((*token)->single_quotes == true)
		return ;
	if ((*token)->token[0] != '\0' && (*token)->token[0] == '$')
	{
		if ((*token)->token[1] != '\0' && (*token)->token[1] == '?')
		{
			free((*token)->token);
			(*token)->token = ft_itoa(g_exit_status);
		}
	}
}

void	expand_expansions(t_token **token, t_env *env)
{
	t_token *temp;

	temp = (*token);
	while ((*token) != NULL)
	{
		exit_status(token);
		if ((*token)->single_quotes == false && confirm_expansion(*token) == 0)
		{
			take_care_of_block(token, env);
			(*token) = temp;
		}
		(*token) = (*token)->next;
	}
	(*token) = temp;
}
