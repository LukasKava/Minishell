/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:13 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/17 04:55:45 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	freeing_tokens(t_token *token)
{
	t_token	*temp;

	temp = token;
	printf("\n");
	while (token != NULL)
	{
		temp = token;
		printf("[%s] ---> ", temp->token);
		free(token->token);
		token = token->next;
		free(temp);
		printf("token freed succesfully!\n");
	}
	printf("HEY!\n");
	free(token);
	printf("HEY1!\n");
}

void	freeing_chunks(t_chunk **chunk, t_info *info)
{
	t_chunk	*free_token;

	free_token = (*chunk);
	while ((*chunk) != NULL)
	{
		free_token = (*chunk);
		printf("command path: %s\n", (*chunk)->command_path);
		free((*chunk)->command_path);
		free((*chunk)->arguments);
		(*chunk) = (*chunk)->next;
		free(free_token);
		printf("chunk freed succesfully!\n");
	}
	free(*chunk);
	printf("dksj: %d\n", info->d_quotes);
}
