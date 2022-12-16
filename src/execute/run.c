/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:49:51 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/16 14:28:57 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	run(t_chunk	*salt, char	**envp)
{
	if (execve(salt->command_path, salt->arguments, envp) < 0)
	{
		// freeing_tokens(hive->token);
		// freeing_chunks(&hive->c_arr);
		// close(vars->save_stdin);
		// close(vars->save_stdout);
		// free(vars);
		// free(hive);
		write(2, salt->arguments[0], ft_strlen(salt->arguments[0]));
		perror(" ");
		g_exit_status = 127;
		exit(127);
	}
}