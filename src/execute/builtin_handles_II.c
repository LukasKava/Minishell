/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handles_II.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 18:57:52 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/19 16:30:29 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/minishell.h"

void	unset_handle(t_env **exp_l, t_env **env_l, t_chunk	**salt)
{
	t_chunk	*element;

	element = *salt;
	if (element->indentifier == BUILT_IN_BLOCK)
	{
		if (strncmp(element->arguments[0], "unset", strlen("unset")) == 0)
		{
			builtins_unset(exp_l, env_l, element->arguments);
		}
	}
}

void	exit_handle(t_data *hive, t_chunk **salt)
{
	t_chunk	*element;

	element = *salt;
	if (element->indentifier == BUILT_IN_BLOCK)
	{
		if (strncmp(element->arguments[0], "exit", strlen("exit")) == 0)
		{
			g_errors.g_exit_status = builtins_exit(hive, element->arguments);
		}
	}
}
