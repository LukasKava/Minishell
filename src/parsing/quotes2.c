/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 16:49:20 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/21 03:27:21 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	final_decision(t_token **t)
{
	if ((*t)->next->ignore == false && \
		space_check((*t)->next) == -1)
		(*t)->ignore = true;
	(*t) = (*t)->next;
}

int	remaking_node(t_token **t, t_token *delete, int ignore)
{
	(*t)->t = ft_strjoin((*t)->t, (*t)->next->t);
	if ((*t)->next->d_quotes == true)
		(*t)->d_quotes = true;
	else
		(*t)->d_quotes = false;
	if ((*t)->next->s_quotes == true)
		(*t)->s_quotes = true;
	else
		(*t)->s_quotes = false;
	delete = (*t)->next;
	(*t)->next = (*t)->next->next;
	free(delete->t);
	free(delete);
	ignore = 1;
	return (ignore);
}

int	first_group(t_token **t, t_token *delete, int ignore)
{
	if (((*t)->d_quotes == false && (*t)->s_quotes == false) && \
		((*t)->next->d_quotes == true || (*t)->next->s_quotes == true))
	{
	//	printf("first if!\n");
		if ((*t)->ignore == false && (*t)->next->ignore == false)
			ignore = remaking_node(t, delete, ignore);
	}
	if ((*t) != NULL && (*t)->next != NULL && ((*t)->d_quotes == true || \
		(*t)->s_quotes == true) && ((*t)->next->d_quotes == false && \
		(*t)->next->s_quotes == false))
	{
	//	printf("second if!\n");
		if ((*t)->next->ignore == false)
			ignore = remaking_node(t, delete, ignore);
	}
	if ((*t) != NULL && (*t)->next != NULL && ((*t)->d_quotes == true || \
		(*t)->s_quotes == true) && ((*t)->next->d_quotes == true || \
		(*t)->next->s_quotes == true))
	{
	//	printf("three!\n");
		ignore = remaking_node(t, delete, ignore);
	}
	ignore = full_qoute_empty_qoute(t, delete, ignore);
	return (ignore);
}

int	full_qoute_empty_qoute(t_token **t, t_token *delete, int ignore)
{
	if ((*t) != NULL && (*t)->next != NULL && space_check(*t) == 1 && \
		space_check((*t)->next) == -1)
	{
		if ((*t)->next->ignore == false)
			ignore = remaking_node(t, delete, ignore);
	}
	return (ignore);
}

int	full_quote_space_quote(t_token **t, t_token *delete, int ignore)
{
	if ((*t) != NULL && (*t)->next != NULL && space_check(*t) == 1 && \
		space_check((*t)->next) == 0)
	{
		if ((*t)->next->ignore == false)
			ignore = remaking_node(t, delete, ignore);
	}
	return (ignore);
}
