/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:18 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 12:43:08 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	temp->indentifier = 0;
	temp->next = NULL;
	token->next = temp;
	return (token);
}

/**
 *	FUNCTION:(space) saves everything before space and saves
 * 				sets  a new start for the next time the lexer needs to
 * 				save a chunk of text. It also sets a trigger for lexer
 *				so that it would create a new node.
 *	RETURN:
 *	--------> (i) its a position to continue from.
 */
static int space(t_info *info, int i, t_token **token)
{
	(*token)->token = ft_substr(info->readline, info->start, i - info->start);
	while (info->readline[i] == ' ')
		i++;
	i--;
	(*token)->next = NULL;
	info->start = i + 1;
	info->trigger = 1;
	return (i);
}

static void check_quote_type(t_token **token, char c)
{
	if (c == '"')
	{
		(*token)->double_quotes = true;
		// Later include expansion variable check
	}
	else
		(*token)->single_quotes = true;
}

static int quotes(t_info *info, int i, t_token **token)
{
	char quote;
	int a;

	quote = info->readline[i];
	a = 0;
	if (info->readline[i - 1] == ' ')
	{
		printf("in\n");
		(*token) = attach_token_end(*token);
		(*token)->token = malloc(sizeof(char) + 2);
		(*token)->token[0] = ' ';
		(*token)->token[1] = '\0';
		(*token) = (*token)->next;
	}
	i++;
	info->start = skip_white_sp(info->readline, i);
	while (info->readline[i] != quote)
		i++;
	(*token)->token = ft_substr(info->readline, info->start, i - (info->start));
	(*token)->next = NULL;
	check_quote_type(token, quote);
	info->start = i + 1;
	info->trigger = 1;
	if (info->readline[i + 1] == ' ')
	{
		a = i + 1;
		a = skip_white_sp(info->readline, a);
		if (info->readline[a] == '"' || info->readline[a] == 39)
		{
			i = a - 1;
			info->start = a;
		}
	}
	return (i);
}

static int ft_check_speacials(char *str, int i)
{
	printf("str[i]: %c\n", str[i]);
	printf("str[i - 1]: %c\n", str[i - 1]);
	if (str[i - 1] == '<' || str[i - 1] == '>')
		return (1);
	if (str[i - 1] == '|' || str[i - 1] == ' ' || str[i - 1] == 34 || str[i - 1] == 39)
		return (1);
	if (str[i - 1] == '&' || str[i - 1] == ';' || str[i - 1] == '(')
		return (1);
	return (0);
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
static int rest_of_the_cases(t_info *info, int i, t_token **token)
{
	char sign;

	sign = info->readline[i];
	if (i != 0 && ft_check_speacials(info->readline, i) == 0)
	{
		(*token)->token = ft_substr(info->readline, info->start, i - info->start);
		(*token) = attach_token_end(*token);
		(*token) = (*token)->next;
	}
	(*token)->token = malloc(sizeof(char) * 2);
	(*token)->token[0] = sign;
	(*token)->token[1] = '\0';
	if (info->readline[i + 1] == ' ')
	{
		i++;
		while (info->readline[i] == ' ' && info->readline[i] != '\0')
			i++;
		i--;
	}
	info->start = i + 1;
	info->trigger = 1;

	return (i);
}

static t_token *triggered(t_info *info, t_token *token, int i)
{
	if (info->readline[i + 1] == '\0')
	{
		i++;
		if (ft_check_speacials(info->readline, i) == 1)
			return (token);
	}
	token = attach_token_end(token);
	token = token->next;
	info->trigger = 0;
	return (token);
}

static int doubles(t_info *info, int i, t_token **token)
{
	//	printf("case: redirect_append\n");
	char sign;

	sign = info->readline[i];
	if (i != 0 && ft_check_speacials(info->readline, i) == 0)
	{
		(*token)->token = ft_substr(info->readline, info->start, i - info->start);
		//	printf("checking redirecting token: %s\n", (*token)->token);
		(*token) = attach_token_end(*token);
		(*token) = (*token)->next;
	}
	(*token)->token = malloc(sizeof(char) * 3);
	(*token)->token[0] = sign;
	(*token)->token[1] = info->readline[i + 1];
	(*token)->token[2] = '\0';
	if (info->readline[i + 2] == ' ')
	{
		i += 2;
		while (info->readline[i] == ' ' && info->readline[i] != '\0')
			i++;
		i--;
	}
	else
		i++;
	info->start = i + 1;
	//	printf("start redirection: %d %c%c i: %d %c%c", info->start, info->readline[info->start], info->readline[info->start + 1], i, info->readline[i], info->readline[i + 1]);
	info->trigger = 1;

	return (i);
}

static bool possible_doubles(t_info *info, int i)
{
	if (info->readline[i] == '>' && info->readline[i + 1] == '|')
		return (true);
	if (info->readline[i] == '<' && info->readline[i + 1] == '<')
		return (true);
	if (info->readline[i] == '>' && info->readline[i + 1] == '>')
		return (true);
	if (info->readline[i] == '&' && info->readline[i + 1] == '&')
		return (true);
	if (info->readline[i] == ';' && info->readline[i + 1] == ';')
		return (true);
	return (false);
}

static bool possible_metacharacters(t_info *info, int i)
{
	if (info->readline[i] == '<' || info->readline[i] == '>')
		return (true);
	if (info->readline[i] == '(' || info->readline[i] == ')')
		return (true);
	if (info->readline[i] == '|' || info->readline[i] == '&' || info->readline[i] == ';')
		return (true);
	return (false);
}

void lexer(t_info *info, t_token **token)
{
	int i;
	t_token *temp_token;

	temp_token = (*token);
	i = skip_white_sp(info->readline, 0);
	info->start = i;
	info->trigger = 0;
	while (info->readline[i] != '\0')
	{
		if (info->readline[i] == 34 || info->readline[i] == 39)
			i = quotes(info, i, token);
		else if (info->readline[i] == ' ')
			i = space(info, i, token);
		else if (possible_doubles(info, i) == true)
			i = doubles(info, i, token);
		else if (possible_metacharacters(info, i) == true)
			i = rest_of_the_cases(info, i, token);
		if (info->trigger == 1)
			(*token) = triggered(info, (*token), i);
		i++;
	}
	if (ft_check_speacials(info->readline, i) == 0)
		(*token)->token = ft_substr(info->readline, info->start, i - info->start);
	(*token) = temp_token;
}
