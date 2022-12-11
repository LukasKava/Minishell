/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 13:32:36 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/11 17:45:41 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	check_for_executables(t_chunk **chunk)
{
	t_chunk	*temp;

	temp = (*chunk);
	while((*chunk) != NULL)
	{
		if ((*chunk)->command_path == NULL && (*chunk)->arguments == NULL && (*chunk)->indentifier == -1)
			(*chunk)->indentifier = ELSE_BLOCK;
		if ((*chunk)->indentifier == CMD_BLOCK && (*chunk)->command_path == NULL)
		{
			if (access((*chunk)->arguments[0], F_OK) == 0 &&\
					access((*chunk)->arguments[0], X_OK) == 0)
				(*chunk)->command_path = ft_strdup((*chunk)->arguments[0]);
		}
		(*chunk) = (*chunk)->next;
	}

	(*chunk) = temp;
}
