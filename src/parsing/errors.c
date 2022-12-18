/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 14:56:46 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 03:23:15 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	simple_err_message(t_info *info, char *message, int exit_status)
{
	printf("\033[0;31m%s\033[0m\n", message);
	info->error = true;
	g_errors.g_exit_status = exit_status;
}

t_chunk	*error_initialise(t_info *info)
{
	printf("\033[0;Allocation failed!\033[0m\n");
	g_errors.g_exit_status = 5;
	info->error = true;
	return (NULL);
}

void	errors_before(t_info *info)
{
	count_quotes(info);
	if (info->d_quotes != 0 || info->s_quotes != 0)
	{
		if (check_quotes(info) == 1)
		{
			info->error = true;
			printf("\033[0;31mQuotes are not closed!\033[0m\n");
			g_errors.g_exit_status = 2;
			return ;
		}
	}
	if (pipe_excistence(info) == 0)
	{
		if (pipe_cases(info) == 1)
		{
			info->error = true;
			printf("\033[0;31mSyntax error | \033[0m\n");
			g_errors.g_exit_status = 2;
			return ;
		}
	}
	//	check_non_generic(info);
	check_dollar_signs(info);
}

int	readline_err(t_data hive)
{
	if (!hive.info.readline)
	{
		write(1, "\033[0;31mCtrl-D was activated!\033[0m\n", 34);
		return (1);
	}
	return (0);
}
