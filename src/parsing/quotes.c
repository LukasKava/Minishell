/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:08 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/01 09:45:00 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * FUNCTION (count_quotes) counts the quotes that are in the
 * 				input.
 */
void count_quotes(t_info *info)
{
	int i;
	int double_quotes;
	int single_quotes;

	i = 0;
	double_quotes = 0;
	single_quotes = 0;
	while (info->readline[i] != '\0')
	{
		if (info->readline[i] == 34)
			double_quotes++;
		else if (info->readline[i] == 39)
			single_quotes++;
		i++;
	}
	info->d_quotes = double_quotes;
	info->s_quotes = single_quotes;
}

int check_quotes(t_info *info)
{
	size_t i;
	char quote;

	i = 0;
	quote = '1';
	while (info->readline[i] != '\0')
	{
		if (info->readline[i] == '"' || info->readline[i] == 39)
		{
			quote = info->readline[i];
			i = skip_quotes(info->readline, quote, i + 1);
			if (info->readline[i] != quote)
				return (1);
		}
		i++;
	}
	return (0);
}

void	check_quote_type(t_token **token, char c)
{
	if (c == '"')
		(*token)->double_quotes = true;
	else
		(*token)->single_quotes = true;
}

static int	connecting_quotes_cases(t_token *token)
{
	if (token->name != SPace && token->next->name != SPace)
	{
		if (token->name != SPace && (token->next->single_quotes == true || token->next->double_quotes == true))
			return (0);
		if (((token->single_quotes == true || token->double_quotes == true) && token->next->name != SPace))
			return (0); 
	}
	return (1);
}

void	connecting_quotes(t_token **token)
{
	t_token	*temp;
	t_token *delete = NULL;
	char *temp_ptr = NULL;

	temp = (*token);
	while ((*token) != NULL)
	{
		if ((*token)->next != NULL)
		{
			if (connecting_quotes_cases(*token) == 0)
			{
				temp_ptr = ft_strjoin((*token)->token, (*token)->next->token);
				(*token)->token = temp_ptr;
				delete = (*token)->next;
				(*token)->next = (*token)->next->next;
				connecting_quotes(token);
				free(delete->token);
				free(delete);
				recognise_builtins(token);
			}
		}
		(*token) = (*token)->next;
	}
	(*token) = temp;
}
