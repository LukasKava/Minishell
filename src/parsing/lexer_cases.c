/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cases.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:06:55 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/12 19:31:33 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 *	FUNCTION:(space) saves everything before space and saves
 * 				sets  a new start for the next time the lexer needs to
 * 				save a chunk of text. It also sets a trigger for lexer
 *				so that it would create a new node.
 *	RETURN:
 *	--------> (i) its a position to continue from.
 */
int space(t_info *info, int i, t_token **token)
{
	(*token)->token = ft_substr(info->readline, info->start, i - info->start);
	i = skip_white_sp(info->readline, i) - 1;
	(*token)->next = NULL;
	info->start = i + 1;
	info->trigger = 1;
	return (i);
}

int quotes(t_info *info, int i, t_token **token)
{
	char quote;
	int a;

	quote = info->readline[i];
	a = 0;
	if (info->readline[i - 1] == ' ')
	{
		printf("in one!\n");
		(*token) = attach_token_end(*token, info);
		(*token)->token = malloc(sizeof(char) + 2);
		(*token)->token[0] = ' ';
		(*token)->token[1] = '\0';
		(*token) = (*token)->next;
	}
	else if (i != 0 && (ft_isalpha(info->readline[i - 1]) == 1 || ft_isalnum(info->readline[i - 1]) == 0) && (info->readline[i - 1] != 34 && info->readline[i - 1] != 39))
	{
		printf("in 2\n");
		(*token) = attach_token_end(*token, info);
		(*token)->token = ft_substr(info->readline, info->start, i - info->start);
		(*token) = (*token)->next;
	}
	info->start = skip_white_sp(info->readline, i + 1);
	i = skip_quotes(info->readline, quote, i + 1);
	(*token)->token = ft_substr(info->readline, info->start, i - (info->start));
	(*token)->next = NULL;
	check_quote_type(token, quote);
	info->start = i + 1;
	info->trigger = 1;
	if (info->readline[i + 1] == ' ')
	{
		a = skip_white_sp(info->readline, i + 1);
		if ((info->readline[a] == '"' || info->readline[a] == 39) || info->readline[a] == '\0')
		{
			i = a - 1;
			info->start = a;
		}
	}
	return (i);
}

/**
 * FUNCTION: (rest_of_the_cases) takes care of the cases with metacharacter:
 * 	-------->	('|')	pipes.
 *	-------->	('<')	redirect_input.
 * 	-------->	('>')	redirect_output.
 * 	-------->	('&')	ampersand.
 * 	-------->	(';')	semicolon.
 *	-------->	('(')	left bracket.
 *	-------->	(')')	right bracket.
 */
int rest_of_the_cases(t_info *info, int i, t_token **token)
{
	char sign;

	sign = info->readline[i];
	if (i != 0 && ft_check_speacials(info->readline, i) == 0)
	{
		(*token)->token = ft_substr(info->readline, info->start, i - info->start);
		(*token) = attach_token_end(*token, info);
		(*token) = (*token)->next;
	}
	(*token)->token = malloc(sizeof(char) * 2);
	(*token)->token[0] = sign;
	(*token)->token[1] = '\0';
	if (info->readline[i + 1] == ' ')
		i = skip_white_sp(info->readline, i + 1) - 1;
	info->start = i + 1;
	info->trigger = 1;

	return (i);
}

int doubles(t_info *info, int i, t_token **token)
{
	char sign;

	sign = info->readline[i];
	if (i != 0 && ft_check_speacials(info->readline, i) == 0)
	{
		printf("inside the i = 0\n");
		(*token)->token = ft_substr(info->readline, info->start, i - info->start);
		(*token) = attach_token_end(*token, info);
		(*token) = (*token)->next;
	}
	(*token)->token = malloc(sizeof(char) * 3);
	(*token)->token[0] = sign;
	(*token)->token[1] = info->readline[i + 1];
	(*token)->token[2] = '\0';
	printf("[%s]\n", (*token)->token);
	if (info->readline[i + 2] == ' ')
		i = skip_white_sp(info->readline, i + 2) - 1;
	else
		i++;
	info->start = i + 1;
	info->trigger = 1;
	return (i);
}
