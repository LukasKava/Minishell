/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:15 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/26 14:12:34 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void initialize_info(t_info *info)
{
	info->exit_status = 0;
	info->s_quotes = 0;
	info->d_quotes = 0;
	info->pipes = 0;
	info->redirect_input = 0;
	info->redirect_output = 0;
	info->redirect_input_append = 0;
	info->redirect_output_append = 0;
	info->dollar_question_sign = 0;
	info->dollar_sign = 0;
	info->start = 0;
	info->trigger = 0;
	info->error = false;
}

t_token *initialize_token(t_token *token, t_info *info)
{
	token = ft_calloc(1, sizeof(t_token)); // This could be a function initialize first token
	if (!token)
	{
		printf("ERROR(initialize_token): malloc has failed!\n");
		info->error = true;
		return (NULL);
	}
	token->token = NULL;
	token->next = NULL;
	token->single_quotes = false;
	token->double_quotes = false;
	token->ignore = false;
	token->name = 0;
	return (token);
}

t_chunk *initialize_chunk(t_chunk *chunk, t_info *info)
{
	chunk = ft_calloc(1, sizeof(t_chunk));
	if (!chunk)
	{
		printf("ERROR(initialize_chunk): malloc has failed!\n");
		info->error = true;
		return (NULL);
	}
	chunk->arguments = NULL;
	// chunk->exec_fd[0] = -1;
	// chunk->exec_fd[1] = -1;
	chunk->command_path = NULL;
	chunk->indentifier = -1;
	chunk->prev = NULL;
	chunk->next = NULL;
	return (chunk);
}

/**
 * FUNCTION: (attach_token_end) does attach the ending node to the
 * 				current excisting list.
 * RETURN:
 *	-- (token) is current node.
 */
t_token *attach_token_end(t_token *token)
{
	t_token *temp;

	temp = malloc(sizeof(t_token));
	if (!temp)
	{
		printf("allocation failed!\n");
		return (NULL);
	}
	temp->double_quotes = false;
	temp->single_quotes = false;
	temp->ignore = false;
	temp->index = 0;
	temp->name = 0;
	temp->next = NULL;
	token->next = temp;
	return (token);
}


t_chunk *attach_chunk_end(t_chunk *chunk)
{
	t_chunk *newNode;
	t_chunk	*temp;

	newNode = malloc(sizeof(t_chunk));
	temp = chunk;
	if (!newNode)
	{
		printf("allocation failed!\n");
		return (NULL);
	}
	while (temp->next != NULL)
		temp = temp->next;
	newNode->command_path = NULL;
	newNode->arguments = NULL;
	newNode->indentifier = 0;
	newNode->next = NULL;
	newNode->prev = temp;
	temp->next = newNode;
	chunk = chunk->next;
	return (chunk);
}
