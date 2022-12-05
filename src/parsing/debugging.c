/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:13:42 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/04 21:58:47 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Debugging command. it prints out the token list.
void print_the_list(char *message, t_token *token)
{
	printf("________%s_________________\n\n", message);
	while (token != NULL) // Printing out the  tokens for checking the correct info
	{
		printf("full list1[%d]: indentifier: %d %s ", token->index, token->name, token->token);
		if (token->ignore == true)
			printf("%d", token->ignore);
		if (token->double_quotes == true)
			printf("%d\n", token->double_quotes);
		else if (token->single_quotes == true)
			printf("%d\n", token->single_quotes);
		else
			printf("\n");
		token = token->next;
	}
}

// Debugging command. it prints out the token list.
void print_the_chunk_list(char *message, t_chunk *chunk)
{
	int	i;

	i = 0;
	printf("________%s_________________\n", message);
	while (chunk != NULL) // Printing out the  tokens for checking the correct info
	{
		printf("full listC: indentifier: %d: %s\n", chunk->indentifier, chunk->command_path);
		printf("ARGUMENTS:\n");
		while (chunk->arguments != NULL && chunk->arguments[i] != NULL)
		{
			printf("%s ", chunk->arguments[i]);
			i++;
		}
		printf("\n");
		i = 0;
		if (chunk->in_f != NULL)
		{
			printf("INFILES:\n");
			while (chunk->in_f[i].name != NULL)
			{
				printf("[%s]: %d ", chunk->in_f[i].name, chunk->in_f[i].type);
				i++;
			}
			printf("\n");
		}
		i = 0;
		if (chunk->out_f != NULL)
		{
			printf("OUTFILES:\n");
			while (chunk->out_f[i].name != NULL)
			{
				printf("[%s]: %d ", chunk->out_f[i].name, chunk->out_f[i].type);
				i++;
			}
		}
		i = 0;
		printf("\n\n");
		chunk = chunk->next;
	}
}

void print_the_chunk_list_backwards(char *message, t_chunk *chunk)
{
	int i;

	i = 0;
	while (chunk->next != NULL)
		chunk = chunk->next;
	printf("________%s_________________\n", message);
	while (chunk != NULL) // Printing out the  tokens for checking the correct info
	{
		printf("full listC: indentifier: %d: %s\n", chunk->indentifier, chunk->command_path);
		printf("ARGUMENTS:\n");
		while (chunk->arguments[i] != NULL)
		{
			printf("%s ", chunk->arguments[i]);
			i++;
		}
		i = 0;
		printf("\n\n");
		chunk = chunk->prev;
	}
}