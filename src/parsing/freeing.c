/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:13 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/17 23:42:15 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	freeing_tokens(t_token *token)
{
	t_token	*temp;

	temp = token;
	while (token != NULL)
	{
		temp = token;
		free(token->token);
		token = token->next;
		free(temp);
	}
	free(token);
}

void	freeing_chunks(t_chunk **chunk)
{
	t_chunk	*free_token;

	free_token = (*chunk);
	while ((*chunk) != NULL)
	{
		free_token = (*chunk);
		free((*chunk)->command_path);
		free((*chunk)->arguments);
		if ((*chunk)->in_f != NULL)
			free((*chunk)->in_f);
		if ((*chunk)->out_f != NULL)
			free((*chunk)->out_f);
		(*chunk) = (*chunk)->next;
		free(free_token);
	}
	free(*chunk);
}

void	freeing_e_list(t_env **e_list)
{
	t_env	*free_token;

	free_token = (*e_list);
	while ((*e_list) != NULL)
	{
		free((*e_list)->var);
		free((*e_list)->var_name);
		free((*e_list)->trim_var);
		free_token = (*e_list);
		(*e_list) = (*e_list)->next;
		free(free_token);
	}
	free(*e_list);
}

void	free_splitted_path(char **splitted_path, int i)
{
	i++;
	while (splitted_path[i] != NULL)
	{
		free(splitted_path[i]);
		i++;
	}
	free(splitted_path);
}
