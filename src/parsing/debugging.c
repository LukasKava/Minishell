/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:13:42 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/19 13:56:15 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Debugging command. it prints out the token list.
void	print_the_list(char *message, t_token *token)
{
	printf("________%s_________________\n\n", message);
	while (token != NULL)
	{
		printf("full list1[%d]: indentifier: %d [%s] ", token->index, token->name, token->t);
		if (token->ignore == true)
			printf("%d", token->ignore);
		if (token->d_quotes == true)
			printf("%d\n", token->d_quotes);
		else if (token->s_quotes == true)
			printf("%d\n", token->s_quotes);
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
	printf("\033[30;1m________%s_________________\033[0m\n", message);
	while (chunk != NULL) // Printing out the  tokens for checking the correct info
	{
		printf("\033[30;1mfull listC: indentifier: %d: %s\033[0m\n", chunk->indentifier, chunk->command_path);
		printf("\033[30;1mARGUMENTS:\033[0m\n");
		while (chunk->arguments != NULL && chunk->arguments[i] != NULL)
		{
			printf("\033[30;1m[%s] \033[0m", chunk->arguments[i]);
			i++;
		}
		printf("\n");
		i = 0;
		if (chunk->in_f != NULL)
		{
			printf("\033[30;1mINFILES:\033[0m\n");
			while (chunk->in_f[i].name != NULL)
			{
				printf("\033[30;1m[%s]: %d \033[0m", chunk->in_f[i].name, chunk->in_f[i].type);
				i++;
			}
			printf("\n");
		}
		i = 0;
		if (chunk->out_f != NULL)
		{
			printf("\033[30;1mOUTFILES:\033[0m\n");
			while (chunk->out_f[i].name != NULL)
			{
				printf("\033[30;1m[%s]: %d \033[0m", chunk->out_f[i].name, chunk->out_f[i].type);
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