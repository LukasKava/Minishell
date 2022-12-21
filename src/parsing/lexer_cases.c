/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cases.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:06:55 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/20 16:37:46 by lkavalia         ###   ########.fr       */
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
int	space(t_info *info, int i, t_token **token)
{
	(*token)->t = ft_substr(info->r, info->f, i - info->f);
	i = skip_white_sp(info->r, i) - 1;
	(*token)->next = NULL;
	info->f = i + 1;
	info->trigger = 1;
	(*token)->d_quotes = false;
	(*token)->s_quotes = false;
	return (i);
}

/* else if (i != 0 && (ft_isalpha(info->readline[i - 1]) == 1 || \
 ft_isalnum(info->readline[i - 1]) == 0 || info->readline[i - 1] == '=' |
 info->readline[i - 1] == '<' || info->readline[i - 1] == '>') &&
		 (info->readline[i - 1] != 34 &&
		  info->readline[i - 1] != 39))
{
	(*token) = attach_token_end(*token, info);
	(*token)->token = ft_substr(info->readline, info->f, i - info->f);
	(*token) = (*token)->next;
	//info->f = skip_white_sp(info->readline, i + 1);
	//printf("info->f: %d [%c]\n", info->f, info->readline[info->f]);
} */

static int	checking_before_cases(t_token **token, t_info *info, int i, char q)
{
	if (i != 0 && info->r[i - 1] == ' ')
	{
		(*token) = attach_token_end(*token, info);
		(*token)->t = ft_calloc(1, sizeof(char));
		(*token)->t[0] = '\0';
		(*token)->ignore = true;
		(*token) = (*token)->next;
	}
	else if (i != 0 && ft_isascii(info->r[i - 1]) == 1 && \
			(info->r[i - 1] != 34 && info->r[i - 1] != 39))
	{
		(*token) = attach_token_end(*token, info);
		(*token)->t = ft_substr(info->r, info->f, i - info->f);
		(*token) = (*token)->next;
	}
	info->f = i + 1;
	i = skip_quotes(info->r, q, i + 1);
	return (i);
}

int	quotes(t_info *info, int i, t_token **token)
{
	char	quote;
	int		a;

	quote = info->r[i];
	a = 0;
	i = checking_before_cases(token, info, i, quote);
	(*token)->t = ft_substr(info->r, info->f, i - (info->f));
	(*token)->next = NULL;
	if (quote == '"')
		(*token)->d_quotes = true;
	else
		(*token)->s_quotes = true;
	info->f = i + 1;
	info->trigger = 1;
	if (info->r[i + 1] == ' ')
	{
		a = skip_white_sp(info->r, i + 1);
		if ((info->r[a] == '"' || info->r[a] == 39) || info->r[a] == '\0')
		{
			i = a - 1;
			info->f = a;
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
int	rest_of_the_cases(t_info *info, int i, t_token **token)
{
	char	sign;

	sign = info->r[i];
	if (i != 0 && ft_check_speacials(info->r, i) == 0)
	{
		(*token)->t = ft_substr(info->r, info->f, i - info->f);
		(*token) = attach_token_end(*token, info);
		(*token) = (*token)->next;
	}
	(*token)->t = malloc(sizeof(char) * 2);
	(*token)->t[0] = sign;
	(*token)->t[1] = '\0';
	if (info->r[i + 1] == ' ')
		i = skip_white_sp(info->r, i + 1) - 1;
	info->f = i + 1;
	info->trigger = 1;
	return (i);
}

int	doubles(t_info *info, int i, t_token **token)
{
	char	sign;

	sign = info->r[i];
	if (i != 0 && ft_check_speacials(info->r, i) == 0)
	{
		(*token)->t = ft_substr(info->r, info->f, i - info->f);
		(*token) = attach_token_end(*token, info);
		(*token) = (*token)->next;
	}
	(*token)->t = malloc(sizeof(char) * 3);
	(*token)->t[0] = sign;
	(*token)->t[1] = info->r[i + 1];
	(*token)->t[2] = '\0';
	if (info->r[i + 2] == ' ')
		i = skip_white_sp(info->r, i + 2) - 1;
	else
		i++;
	info->f = i + 1;
	info->trigger = 1;
	return (i);
}
