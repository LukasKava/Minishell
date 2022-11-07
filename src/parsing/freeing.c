/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:13 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/07 10:54:36 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void freeing_tokens(t_token *token)
{
	printf("\n");
	t_token *temp;

	temp = token;
	while (token != NULL)
	{

		temp = token;
		printf("[%s] ---> ", temp->token);
		free(token->token);
		token = token->next;
		//	free(free_token->token);
		free(temp);
		printf("token freed succesfully!\n");
	}
	free(token);
}

void freeing_chunks(t_chunk **chunk, t_info *info)
{
	t_chunk *free_token;

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