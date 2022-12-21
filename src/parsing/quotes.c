/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:08 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/21 03:01:02 by lkavalia         ###   ########.fr       */
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
	while (info->r[i] != '\0')
	{
		if (info->r[i] == 34)
			double_quotes++;
		else if (info->r[i] == 39)
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
	while (info->r[i] != '\0')
	{
		if (info->r[i] == '"' || info->r[i] == 39)
		{
			quote = info->r[i];
			i = skip_quotes(info->r, quote, i + 1);
			if (info->r[i] != quote)
				return (1);
		}
		i++;
	}
	return (0);
}

void	norminette_hell(t_token **token, t_token *delete)
{
	(void)token;
	free(delete->t);
	free(delete);
}

void	connecting_quotes(t_token **t)
{
	t_token	*temp;
	t_token	*delete;
	int		ignore;

	delete = NULL;
	temp = (*t);
	ignore = 0;
	while ((*t) != NULL)
	{
		while ((*t) != NULL && (*t)->next != NULL)
		{
			ignore = first_group(t, delete, ignore);
			ignore = full_quote_space_quote(t, delete, ignore);
			if ((*t) != NULL && (*t)->next != NULL && \
				((space_check(*t) == -1 && (*t)->ignore == false) && \
				(*t)->next->ignore == false))
				ignore = remaking_node(t, delete, ignore);
			if ((*t) != NULL && ignore != 1)
				(*t) = (*t)->next;
			ignore = 0;
		}
		if ((*t) != NULL)
			(*t) = (*t)->next;
	}
	(*t) = temp;
}

void	check_for_deleting_spaces(t_token **t)
{
	t_token	*temp;

	temp = (*t);
	first_token(t);
	while ((*t) != NULL)
	{
		if (space_check(*t) == -1 && (*t)->ignore == true)
		{
			if ((*t)->next != NULL)
			{
				(*t) = (*t)->next;
				if (space_check(*t) == -1)
				{
					if ((*t)->next != NULL)
						final_decision(t);
				}
			}
		}
		if ((*t) != NULL)
			(*t) = (*t)->next;
	}
	(*t) = temp;
}
