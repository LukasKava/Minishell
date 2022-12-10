/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:49:51 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/10 19:51:19 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	run(t_chunk	*salt, t_info *info, char	**envp)
{
	if (execve(salt->command_path, salt->arguments, envp) < 0)
	{
		freeing_chunks(&salt, info);
		printf("Error when attempting to execute programme\n");
		return ;
	}
}