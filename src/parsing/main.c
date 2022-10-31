/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:21 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 18:01:28 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void check_non_generic(t_info *info)
{
	int i;

	i = 0;
	if (ft_strlen(info->readline) > 0)
	{
		while (info->readline[i] == ' ' && info->readline[i] != '\0')
			i++;
		if (info->readline[i] == '&' && info->readline[i + 1] == '&')
		{
			printf("bash: syntax error near unexpected token `&&'\n");
			info->error = true;
			info->exit_status = 2;
			return;
		}
		if (info->readline[i] == ';' && info->readline[i + 1] == ';')
		{
			printf("bash: syntax error near unexpected token `;;'\n");
			info->error = true;
			info->exit_status = 2;
			return;
		}
		if (info->readline[i] == ';' && info->readline[i + 1] != ';')
		{
			printf("bash: syntax error near unexpected token `;'\n");
			info->error = true;
			info->exit_status = 2;
			return;
		}
		if (info->readline[i] == '&' && info->readline[i + 1] != '&')
		{
			printf("bash: syntax error near unexpected token `&'\n");
			info->error = true;
			info->exit_status = 2;
			return;
		}
		if (info->readline[i] == '(' || info->readline[i] == ')')
		{
			printf("in\n");
			i++;
			while (info->readline[i] == ' ' && info->readline[i] != '\0')
				i++;
			if (i > 0)
				i--;
			if (info->readline[i + 1] == '(' || info->readline[i + 1] == ')')
			{
				printf("bash: syntax error near unexpected token `)'\n");
				info->error = true;
				info->exit_status = 2;
				return;
			}
		}
	}
}

static t_token *initialize_token(t_token *token, t_info *info)
{
	token = ft_calloc(1, sizeof(t_token)); // This could be a function initialize first token
	if (!token)
	{
		printf("ERROR(initialize_token): malloc has failed!\n");
		info->error = true;
		return (NULL);
	}
	token->token = NULL;
	token->next = NULL;			  // This could be a function initialize first token
	token->single_quotes = false; // This could be a function initialize first token
	token->double_quotes = false; // This could be a function initialize first token
	token->ignore = false;		  // This could be a function initialize first token
	token->indentifier = 0;		  // This could be a function initialize first token
	return (token);
}

void errors_before(t_info *info)
{
	count_quotes(info);								// COUNTS THE QUOTES
	if (info->d_quotes != 0 || info->s_quotes != 0) // quotes not closed stops right here
	{
		if (check_quotes(info) == 1)
		{
			info->error = true;
			printf("Quotes are not closed!\n");
		}
	}
	if (pipe_excistence(info) == 0) // Checks if there are pipes and there are no problems with them.
	{
		if (pipe_cases(info) == 1)
		{
			info->error = true;
			printf("Syntax error | \n");
		}
	}
	check_non_generic(info);
	check_dollar_signs(info);
}

void print_the_list(char *message, t_token *token)
{
	printf("________%s_________________\n\n", message);
	while (token != NULL) // Printing out the  tokens for checking the correct info
	{
		printf("full list1[%d]: indentifier: %d %s ", token->index, token->indentifier, token->token);
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

int main(int argc, char **argv, char **envp)
{
	t_info info;
	t_token *token;

	printf("envp %s\n", envp[1]);
	token = NULL;
	if (argc != 1)
	{
		printf("%s doesn't need more arguments.\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		info.readline = readline("Mini_hell\U0001F34C\U0001F412 ");
		initialize_info(&info);
		errors_before(&info);
		if (info.error == false)
		{
			token = initialize_token(token, &info);
			lexer(&info, &token);
			print_the_list("after lexing", token);
			register_tokens(&info, &token, envp);
			print_the_list("register tokens check", token);
			
			freeing_tokens(token);
		}
		printf("after the !register_the_information!\n");
		if (ft_strlen(info.readline) != 0)
		{
			add_history(info.readline);
			free(info.readline);
		}
		else
			free(info.readline);
	}
	return (0);
}
