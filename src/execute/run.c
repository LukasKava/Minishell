/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:49:51 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/14 08:41:45 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	run(t_chunk	*salt, char	**envp)
// void	run(t_chunk	*salt, char	**envp, t_data *hive, t_vars *vars)
{
	if (execve(salt->command_path, salt->arguments, envp) < 0)
	{
		// freeing_tokens(hive->token);
		// freeing_chunks(&hive->c_arr);
		// close(vars->save_stdin);
		// close(vars->save_stdout);
		// free(vars);
		// free(hive);
		// write(2, salt->arguments[0], ft_strlen(salt->arguments[0]));
		perror(" ");
		g_exit_status = 127;
		exit(127);
	}
}