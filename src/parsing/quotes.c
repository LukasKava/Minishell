/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:08 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/19 13:50:44 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * FUNCTION (count_quotes) counts the quotes that are in the
 * 				input.
 */
void	count_quotes(t_info *info)
{
	int	i;
	int	double_quotes;
	int	single_quotes;

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

int	check_quotes(t_info *info)
{
	size_t	i;
	char	quote;

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

static int	connecting_quotes_cases(t_token *token)
{
	if ((token->name != SPC || token->name != EMPTY) && \
		(token->s_quotes == true || token->d_quotes == true))
	{
		if ((token->next->name != SPC || token->next->name != EMPTY) && \
									(token->next->d_quotes == true || \
									token->next->d_quotes == true))
			return (0);
	}
	return (1);
}

void	norminette_hell(t_token **token, t_token *delete)
{
	connecting_quotes(token);
	free(delete->t);
	free(delete);
	recognise_builtins(token);
}

void	connecting_quotes(t_token **token)
{
	t_token	*temp;
	t_token	*delete;
	char	*temp_ptr;

	delete = NULL;
	temp_ptr = NULL;
	temp = (*token);
	while ((*token) != NULL)
	{
		if ((*token)->next != NULL)
		{
			if (connecting_quotes_cases(*token) == 0)
			{
				temp_ptr = ft_strjoin((*token)->t, (*token)->next->t);
				(*token)->t = temp_ptr;
				delete = (*token)->next;
				(*token)->next = (*token)->next->next;
				norminette_hell(token, delete);
			}
		}
		(*token) = (*token)->next;
	}
	(*token) = temp;
}
