/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 15:32:15 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/04 21:34:24 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *return_ex_value(char *var, t_env *env)
{
	char *full_var;

	while (env != NULL && env->var != ft_strnstr(env->var, var, ft_strlen(var)))
		env = env->next;
	if (env == NULL || (env->var[ft_strlen(var)] != '=' && env->var[ft_strlen(var)] != '\0'))
	{
		printf("VAR does not excist!\n");
		g_exit_status = 127;
		free(var);
		return (NULL);
	}
	if (env->var[ft_strlen(var)] == '=')
		var = ft_strjoin(var, "=");
	full_var = env->var;
	full_var = ft_strtrim_beginning(full_var, var);
	free(var);
	return (full_var);
}

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

static int	confirm_expansion(t_token *token)
{
	int	i;

	i = 0;
	while (token->token[i] != '\0')
	{
		if (token->token[i] == '$')
		{
			i++;
			if (token->token[i] == '\0')
				return (1);
			if ((token->token[i] >= 'a' && token->token[i] <= 'z') ||\
			 	(token->token[i] >= 'A' && token->token[i] <= 'Z') ||\
				(token->token[i] >= '0' && token->token[i] <= '9') ||\
				token->token[i] == '_')
				return (0);
		}
		i++;
	}
	return (1);
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


void	expand_expansions(t_token **token, t_env *env)
{
	t_token *temp;

	temp = (*token);
	while ((*token) != NULL)
	{
		if ((*token)->single_quotes == false && confirm_expansion(*token) == 0)
		{
			take_care_of_block(token, env);
			(*token) = temp;
		}
		(*token) = (*token)->next;
	}
	(*token) = temp;
}
