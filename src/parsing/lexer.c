/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:18 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/06 13:58:59 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_check_speacials(char *str, int i)
{
	if (str[i - 1] == '<' || str[i - 1] == '>')
		return (1);
	if (str[i - 1] == '|' || str[i - 1] == ' ' || str[i - 1] == 34 || str[i - 1] == 39)
		return (1);
	if (str[i - 1] == '&' || str[i - 1] == ';' || str[i - 1] == '(')
		return (1);
	return (0);
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

/**
 * FUNCTION: (possible_doubles) checks for the possible groups of characters.
 * 				While a lot of them are included  some of them are not used.
 * 				For example: "&&" ";;"
 * RETURN_VALUE:
 * x	True if any of the cases excist.
 * x	False if any of them do not excist.
 */
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
	if (info->readline[i] == '$' && info->readline[i + 1] == '?')
		return (true);
	return (false);
}

/**
 * FUNCTION: (possible_metacharacters) checks for the possible
 * 				metacharacters. Some metacharacters are not required
 * 				by the manditory but are still included in a check.
 * 				'(' ')' '|' '&' ';'
 * RETURN_VALUES:
 * 	x	True if any of ("<" ">" "(" ")" "|" "&" ";") excist.
 * 	x	False if they do not excist.
 */
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
