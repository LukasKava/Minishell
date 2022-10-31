/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:15 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 15:05:12 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void initialize_info(t_info *info)
{
	info->exit_status = 0;
	info->s_quotes = 0;
	info->d_quotes = 0;
	info->pipes = 0;
	info->redirect_input = 0;
	info->redirect_output = 0;
	info->redirect_input_append = 0;
	info->redirect_output_append = 0;
	info->dollar_question_sign = 0;
	info->dollar_sign = 0;
	info->start = 0;
	info->trigger = 0;
	info->error = false;
}

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
