/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:49:51 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/11 14:00:26 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	run(t_chunk	*salt, t_info *info, char	**envp, t_vars *vars)
{
	if (execve(salt->command_path, salt->arguments, envp) < 0)
	{
		freeing_chunks(&salt, info);
		free(vars);
		printf("Error when attempting to execute programme\n");
		g_exit_status = 126;
		return ;
	}
}